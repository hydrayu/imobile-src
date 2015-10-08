using System;
using System.Text;
using System.Threading;

namespace iMobileBarcode
{
	/// <summary>
	/// Serial communications base interface class. This class handles all 
	/// internal data manipulation concerning the serial device(s) and the
	/// associated Win32 API calls. 
	/// </summary>
	public abstract class SerialComm
	{
		#region Members
		/// <summary>
		/// Device control block.
		/// </summary>
		private Win32DCB dcb;
		/// <summary>
		/// Standard I/O methods.
		/// </summary>
		private Win32Com port;
		/// <summary>
		/// Comm status and errors.
		/// </summary>
		private Win32Status	stats;
		/// <summary>
		/// Com properties.
		/// </summary>
		private Win32Props props;
		/// <summary>
		/// Comm timeouts.
		/// </summary>
		private Win32Tmout tmout;
		/// <summary>
		/// Transmitter overlapping.
		/// </summary>
		private Win32Ovrlap	txOvr;
		/// <summary>
		/// Receiver overlapping.
		/// </summary>
		private Win32Ovrlap	rxOvr;
		/// <summary>
		/// Comm escape method.
		/// </summary>
		private Win32Escape	escape;
		/// <summary>
		/// Receiver event handling.
		/// </summary>
		private Win32Events events;
		/// <summary>
		/// Comm port modem signals.
		/// </summary>
		private Win32Modem modem;
		/// <summary>
		/// Port open flag.
		/// </summary>
		private bool portOpen;
		/// <summary>
		/// Copy of port name.
		/// </summary>
		private string portName;
		/// <summary>
		/// RX immediate handling.
		/// </summary>
		private bool immediate;
		/// <summary>
		/// Copy of last fault returned locally
		/// or by any supporting wrapper class.
		/// </summary>
		private string fault;
		/// <summary>
		/// The receiver thread handle.
		/// </summary>
		private Thread rxThread;
		/// <summary>
		/// Write completion signal.
		/// </summary>
		private ManualResetEvent writeEvent;
		/// <summary>
		/// Receiver thread started signal.
		/// </summary>
		private ManualResetEvent recvrEvent;
		#endregion

		#region Destructor
		/// <summary>
		/// Destructor
		/// </summary>
		~SerialComm()
		{
			this.Destroy();
		}
		#endregion

		#region Methods
		/// <summary>
		/// Create & opens the com port and configures it with the required settings.
		/// </summary>
		/// <param name="cfg">Reference to port user config.</param>
		/// <returns>True if port opened successfully.</returns>
		protected bool Create(SerialCnfg cfg) 
		{
			// If port already open, return.
			if(portOpen)
			{
				this.CommError("Com Port Already Open.");
				return false;
			}

			// Init members.
			this.fault = "";
			this.rxThread = null;
			this.writeEvent = new ManualResetEvent(false);
			this.recvrEvent = new ManualResetEvent(false);

			// Copy config to DCB.
			this.dcb = new Win32DCB(cfg);

			// Create handle to comm port.
			this.port = new Win32Com();
			if((this.portOpen = this.port.Open(cfg.PortName, true)) == false)
			{
				this.CommError(this.port.Fault);
				return false;
			}
			this.portName = cfg.PortName;

			// Instantiate support classes.
			this.stats  = new Win32Status();
			this.tmout  = new Win32Tmout();
			this.props  = new Win32Props();
			this.escape = new Win32Escape(port.Handle);

			// Set read/write timeouts.
			this.tmout.WriteConstant = cfg.TxTmoConst;
			this.tmout.WriteMultiplier = cfg.TxTmoMulti;
			if(this.tmout.Set(this.port.Handle) == false)
			{
				this.Destroy();
				this.CommError(this.tmout.Fault);
				return false;
			}

			// Overide OS default queue sizes.
			if((cfg.RxQueLen != 0) || (cfg.TxQueLen != 0))
			{
				if(this.props.Set(this.port.Handle, (uint)cfg.RxQueLen, (uint)cfg.TxQueLen) == false)
				{
					this.Destroy();
					this.CommError(this.props.Fault);
					return false;
				}
			}

			// Get the current properties.
			if(this.props.Get(this.port.Handle) == false)
			{
				this.Destroy();
				this.CommError(this.props.Fault);
				return false;
			}

			// Set flow control limits.
			this.dcb.Limits(cfg, this.props.RxCurSize);

			// Update the port settings.
			if(this.dcb.Set(this.port.Handle) == false)
			{
				this.Destroy();
				this.CommError(this.dcb.Fault);
				return false;
			}

			// XON/OFF extended functionality.
			this.escape.XOFFavailable = true;

			// RTS extended functionality.
			if(cfg.RtsControl == PinState.Disable)
				this.escape.RTS = false;
			else if(cfg.RtsControl == PinState.Enable)
				this.escape.RTS = true;
			else if(cfg.RtsControl == PinState.Handshake)
				this.escape.RTSavailable = false;

			// DTR extended functionality.
			if(cfg.DtrControl == PinState.Disable)
				this.escape.DTR = false;
			else if(cfg.DtrControl == PinState.Enable)
				this.escape.DTR = true;
			else if(cfg.DtrControl == PinState.Toggle)
				this.escape.DTR = false;
			else if(cfg.DtrControl == PinState.Handshake)
				this.escape.DTRavailable = false;

			// Create TX overlap memory pointer.
			this.txOvr = new Win32Ovrlap(this.port.Handle, this.writeEvent.Handle);

			// Set the receiver mode.
			this.immediate = cfg.ReceiveMode;

			// Start the receiver thread.
			this.rxThread = new Thread(new ThreadStart(ReceiveThread));
			this.rxThread.Name = "COMReceiver";
			this.rxThread.Priority = ThreadPriority.AboveNormal;
			this.rxThread.Start();

			// Wait for receive thread to start.
			this.recvrEvent.WaitOne(500, false);

			// Port opened OK.
			return true;
		}

		/// <summary>
		/// Closes and destroys the com port.
		/// </summary>
		protected void Destroy()
		{
			// Cancel I/O, kill receiver & close port.
			if(this.portOpen)
			{
				this.port.Cancel();
				if(this.rxThread != null)
				{
					this.rxThread.Abort();
					this.rxThread = null;
				}
				this.port.Close();
				this.portOpen = false;
			}

			// Reinit resources.
			this.dcb    = null;
			this.port   = null;
			this.stats  = null;
			this.props  = null;
			this.tmout  = null;
			this.rxOvr  = null;
			this.txOvr  = null;
			this.escape = null;
			this.writeEvent = null;
			this.recvrEvent = null;
			return;
		}
		
		/// <summary>
		/// Check to see if the named com port is available.
		/// </summary>
		/// <returns>True if port available.</returns>
		public bool Available(string portName) 
		{
			Win32Com temp = new Win32Com();
			bool status = temp.Open(portName, false);
			if(status)
			{
				temp.Close();
			}
			temp = null;
			return status;
		}

		/// <summary>
		/// Flush all data in TX/RX queues.
		/// </summary>
		public void Flush()
		{
			if(this.portOpen == false)
			{
				this.CommError("Flush() Failed. " + this.portName + " Is Closed.");
			}
			else
			{
				if(this.port.Flush() == false)
				{
					this.CommError(this.port.Fault);
				}
			}
			return;
		}

		/// <summary>
		/// Return count of data available from the RX queue.
		/// </summary>
		public uint Ready()
		{
			uint nReady = 0;
			if(this.portOpen == false)
			{
				this.CommError("Ready() Failed. " + this.portName + " Is Closed.");
			}
			else
			{
				if(this.stats.Clear(this.port.Handle) == false)
				{
					this.CommError(this.stats.Fault);
				}
				else
				{
					nReady = this.stats.Ready;
				}
			}
			return nReady;
		}

		/// <summary>
		/// Read available data from the RX queue.
		/// </summary>
		/// <param name="b">Buffer into which bytes are read.</param>
		/// <returns>Number of bytes actually read.</returns>
		public uint Recv(out byte[] b)
		{
			uint nBytes = 0;
			if(this.portOpen == false)
			{
				this.CommError("Recv() Failed. " + this.portName + " Is Closed.");
				b = new Byte[1];
			}
			else
			{
				uint nReady = this.Ready();
				if(nReady > 0)
				{
					b = new Byte[nReady];
                    if (this.port.Read(b, nReady, out nBytes, rxOvr.MemPtr) == false)
                    {
                        this.CommError(this.port.Fault);
                    }
                    nBytes = nReady;
                }
				else
				{
					b = new Byte[1];
				}
			}
			return nBytes;
		}

		/// <summary>
		/// Read a specified amount of data from the RX queue.
		/// </summary>
		/// <param name="b">Buffer into which bytes are read.</param>
		/// <param name="nBytes">Number of bytes to read.</param>
		/// <returns>Number of bytes actually received.</returns>
		public uint Recv(byte[] b, uint nBytes)
		{
			uint nRecvd = 0;
			if(this.portOpen == false)
			{
				this.CommError("Recv() Failed. " + this.portName + " Is Closed.");
			}
			else
			{
				if(this.Ready() >= nBytes)
				{
					if(this.port.Read(b, nBytes, out nRecvd, rxOvr.MemPtr) == false)
					{
						this.CommError(this.port.Fault);
					}
				}
			}
			return nRecvd;
		}

		/// <summary>
		/// Send a byte. 
		/// </summary>
		/// <param name="chr">Byte to be sent.</param>
		/// <returns>Number of bytes actually sent.</returns>
		public uint Send(byte chr) 
		{
			byte[] b = new byte[1];
			b[0] = chr;
			return Send(b);
		}

		/// <summary>
		/// Send a string.
		/// </summary>
		/// <param name="s">String to send.</param>
		/// <returns>Number of bytes actually sent.</returns>
		public uint Send(string s)
		{
			byte[] b = new byte[s.Length];
			b = Encoding.ASCII.GetBytes(s);
			return this.Send(b);
		}

		/// <summary>
		/// Send a buffer. 
		/// </summary>
		/// <param name="buffer">Array of bytes to be sent.</param>
		/// <returns>Number of bytes actually sent.</returns>
		public uint Send(byte[] buffer) 
		{
			uint nSent = 0;
			if(this.portOpen == false)
			{
				this.CommError("Send() Failed. " + this.portName + " Is Closed.");
			}
			else
			{
				if(this.port.Write(buffer, (uint) buffer.Length, out nSent, txOvr) == false)
				{
					this.CommError(this.port.Fault);
				}
			}
			return nSent;
		}

		/// <summary>
		/// Sends a byte immediately - ahead of any queued bytes.
		/// </summary>
		/// <param name="chr">Byte to send</param>
		/// <returns>True if byte sent successfully.</returns>
		public bool SendI(byte chr)
		{
			if(this.portOpen == false)
			{
				this.CommError("SendI() Failed. " + this.portName + " Is Closed.");
			}
			else
			{
				if(this.port.TxChar(chr))
					return true;
				this.CommError(this.port.Fault);
			}
			return false;
		}

		/// <summary>
		/// Sends an extended function.
		/// </summary>
		/// <param name="code">Extended function code.</param>
		/// <returns>True if function exectued successfully.</returns>
		public bool SendE(ExtCodes code)
		{
			if(this.portOpen == false)
			{
				this.CommError("SendE() Failed. " + this.portName + " Is Closed.");
			}
			else
			{
				if(this.escape.ExtFunc((uint) code))
					return true;
				this.CommError(this.escape.Fault);
			}
			return false;
		}

		/// <summary>
		/// Update the modem signal states.
		/// </summary>
		public void Signals()
		{
			if((this.modem != null) && (this.port != null))
			{
				this.modem.Get(this.port.Handle);
				this.OnCTS(this.modem.CtsState);
				this.OnDSR(this.modem.DsrState);
				this.OnRLSD(this.modem.RlsdState);
				this.OnRING(this.modem.RingState);
			}
			else
			{
				this.OnCTS(false);
				this.OnDSR(false);
				this.OnRLSD(false);
				this.OnRING(false);
			}
			return;
		}

		/// <summary>
		/// Take action when an error condition occurs.
		/// </summary>
		/// <param name="fault">Fault message.</param>
		protected virtual void OnError(string fault)
		{}

		/// <summary>
		/// Take action when a break condition is detected.
		/// </summary>
		protected virtual void OnBreak()
		{}

		/// <summary>
		/// Take action when TX is complete & queue empty.
		/// </summary>
		protected virtual void OnTxDone()
		{}

		/// <summary>
		/// Process each received byte immediately.
		/// </summary>
		/// <param name="b">Received byte.</param>
		protected virtual void OnRxChar(byte[] b)
		{}

		/// <summary>
		/// Override this to take action when the CTS modem status input changes.
		/// </summary>
		/// <param name="cts">The current CTS state.</param>
		protected virtual void OnCTS(bool cts)
		{}

		/// <summary>
		/// Override this to take action when the DSR modem status input changes.
		/// </summary>
		/// <param name="dsr">The current DSR state.</param>
		protected virtual void OnDSR(bool dsr)
		{}

		/// <summary>
		/// Override this to take action when the RLSD modem status input changes.
		/// </summary>
		/// <param name="rlsd">The current RLSD state.</param>
		protected virtual void OnRLSD(bool rlsd)
		{}

		/// <summary>
		/// Override this to take action when the RING modem status input changes.
		/// </summary>
		/// <param name="ring">The current ring indicatior state.</param>
		protected virtual void OnRING(bool ring)
		{}

		/// <summary>
		/// Main receiver thread for this com port instance.
		/// </summary>
		private void ReceiveThread()
		{
			// Create locals.
			uint firedEvent = 0;
			uint nBytes = 0;
			byte[] buf = new Byte[1];

			// Create thread signal (initial state is non-signaled). 
			AutoResetEvent signal = new AutoResetEvent(false);

			// Create overlap memory pointer. Install signal.
			this.rxOvr = new Win32Ovrlap(port.Handle, signal.Handle);

			// Instantiate the event class.
			this.events = new Win32Events(rxOvr.MemPtr);

			// Instantiate the modem signal class.
			this.modem = new Win32Modem();

			// Set initial modem signal states.
			this.Signals();

			// Signal the main thread that 
			// the receive thread is started.
			this.recvrEvent.Set();

			try
			{
				while(true) 
				{
					// Arm the event monitoring mask.
					// if(this.events.Set(this.port.Handle, Win32Events.EV_DEFAULT))
					if(this.events.Set(this.port.Handle, Win32Events.EV_TXEMPTY))
					{
						// Wait for any armed event to occur.
						this.events.Wait(this.port.Handle, signal);

						// Get mask of fired events.
						firedEvent = this.events.GetMask;

						// Error event (override).
						if((firedEvent & Win32Events.EV_ERR) != 0)
						{
							if(this.stats.Reset(this.port.Handle) == false)
							{
								if(this.stats.Status == Win32Status.CE_BREAK)
								{
									firedEvent &= ~Win32Events.EV_BREAK;
								}
								else
								{
									this.CommError(this.stats.Fault);
								}
							}
						}
						else
						{
							// Receive event (override).
							if(((firedEvent & Win32Events.EV_RXCHAR) != 0) && (this.immediate))
							{
								do 
								{
									nBytes = 0;
									if(this.port.Read(buf, 1, out nBytes, this.rxOvr.MemPtr)) 
									{
										if(nBytes == 1)
											this.OnRxChar(buf);
									}
								} while(nBytes > 0);
							}

							// TX queue empty event (override).
							if((firedEvent & Win32Events.EV_TXEMPTY) != 0)
								this.OnTxDone();

							// Line break event (override).
							if((firedEvent & Win32Events.EV_BREAK) != 0)
								this.OnBreak();

							// Modem signal change event(s) (override).
							if((firedEvent & Win32Events.EV_MODEM) > 0)
							{
								this.modem.Get(this.port.Handle);
								if((firedEvent & Win32Events.EV_CTS) > 0)
									this.OnCTS(this.modem.CtsState);
								if((firedEvent & Win32Events.EV_DSR) > 0)
									this.OnDSR(this.modem.DsrState);
								if((firedEvent & Win32Events.EV_RLSD) > 0)
									this.OnRLSD(this.modem.RlsdState);
								if((firedEvent & Win32Events.EV_RING) > 0)
									this.OnRING(this.modem.RingState);
							}
						}
					}					
				}
			}
			catch(Exception e)
			{
				if((e is ThreadAbortException))
				{
					this.modem = null;
					this.events = null;
					this.rxOvr = null;
				}
				else
				{
					this.CommError("Receiver Exception: " + e.Message);
				}
			}
		}
		/// <summary>
		/// Record the last fault and call the fault interface.
		/// </summary>
		/// <param name="fault"></param>
		private void CommError(string fault)
		{
			this.fault = fault;
			this.OnError(fault);
			return;
		}
		#endregion

		#region Properties
		/// <summary>
		/// Get port open status.
		/// </summary>
		public bool IsOpen
		{
			get	{	return this.portOpen; 	}
		}
		/// <summary>
		/// Get the class fault description.
		/// </summary>
		public string Fault
		{
			get	{	return this.fault;	}
		}
		#endregion
	}
}
