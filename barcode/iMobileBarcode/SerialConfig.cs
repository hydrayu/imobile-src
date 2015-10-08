using System;
using System.IO;
using System.Text;

namespace iMobileBarcode
{
	/// <summary>
	/// User defined serial port configuration settings.
	/// </summary>
	public class SerialCnfg
	{
		#region Members
		private string		portName;
		private LineSpeed	baudRate;
		private bool		txFlowCTS;
		private bool		txFlowDSR;
		private PinState	dtrControl;
		private bool		rxDSRsense;
		private bool		txContinue;
		private bool		txFlowXoff;
		private bool		rxFlowXoff;
		private bool		errReplace;
		private bool		nulDiscard;
		private PinState	rtsControl;
		private bool		abortOnErr;
		private short		xonLimit;
		private short		xoffLimit;
		private ByteSize	dataBits;
		private Parity		parity;
		private StopBits	stopBits;
		private byte		xonChar;
		private byte		xoffChar;
		private byte		errChar;
		private byte		eofChar;
		private byte		evtChar;
		private Handshake	handshake;
		private int			rxQueLen;
		private int			txQueLen;
		private uint		txTmoMulti;
		private uint		txTmoConst;
		private bool		receiveMode;
        public uint         startup;
		#endregion

		#region Constructor
		/// <summary>
		/// Default constructor. Initializes the class.
		/// </summary>
		public SerialCnfg(int index)
		{
			this.Initialize(index);
			return;
		}
		/// <summary>
		/// Overload constructor. Init and load the class.
		/// </summary>
		public SerialCnfg(int index, string fName)
		{
			this.Initialize(index);
			this.Load(fName);
			return;
		}
		#endregion

		#region Methods
		/// <summary>
		/// Initialize this instance with standard settings. 
		/// 9600:N:8:1, handshaking disabled.
		/// </summary>
		/// <param name="index">The port index - 1,2,...</param>
		public void Initialize(int index)
		{
			portName   = "COM" + index.ToString() + ":";
			baudRate   = LineSpeed.Baud_9600;
			txFlowCTS  = false;
			txFlowDSR  = false;
			dtrControl = PinState.Disable;
			rxDSRsense = false;
			txContinue = true;
			txFlowXoff = false;
			rxFlowXoff = false;
			errReplace = false;
			nulDiscard = false;
			rtsControl = PinState.Disable;
			abortOnErr = false;
			xonLimit   = 0;	// 0=OS managed
			xoffLimit  = 0; // 0=OS managed
			dataBits   = ByteSize.Eight;
			parity     = Parity.None;
			stopBits   = StopBits.One;
			xonChar    = (byte) CtrlChar.DC1;
			xoffChar   = (byte) CtrlChar.DC3;
			errChar    = (byte) '?';
			eofChar    = (byte) CtrlChar.SUB;
			evtChar    = (byte) CtrlChar.NULL;
			handshake  = Handshake.None;
			rxQueLen   = 0; // 0=OS managed
			txQueLen   = 0; // 0=OS managed
			txTmoMulti = 0;
			txTmoConst = 0;
			receiveMode = false;
            startup = 0;
			return;
		}

		/// <summary>
		/// Setup the device/handshake settings.
		/// </summary>
		public void SetFlowControl()
		{
			switch(this.handshake)
			{
				case(Handshake.None):
					this.txFlowCTS  = false;
					this.txFlowDSR  = false;
					this.txFlowXoff = false;
					this.rxFlowXoff = false;
					this.rtsControl = PinState.Disable;
					this.dtrControl = PinState.Disable;
					break;
				case(Handshake.CtsRts):
					this.txFlowCTS  = true;
					this.txFlowDSR  = false;
					this.txFlowXoff = false;
					this.rxFlowXoff = false;
					this.rtsControl = PinState.Handshake;
					this.dtrControl = PinState.Disable;
					break;
				case(Handshake.DsrDtr):
					this.txFlowCTS  = false;
					this.txFlowDSR  = true;
					this.txFlowXoff = false;
					this.rxFlowXoff = false;
					this.rtsControl = PinState.Disable;
					this.dtrControl = PinState.Handshake;
					break;
				case(Handshake.XonXoff):
					this.txFlowCTS  = false;
					this.txFlowDSR  = false;
					this.txFlowXoff = true;
					this.rxFlowXoff = true;
					this.rtsControl = PinState.Enable;
					this.dtrControl = PinState.Enable;
					break;
			}
		}

		/// <summary> 
		/// Save the message definition list to a text file.
		/// </summary>
		public void Save(string fName)
		{
			// Open text definitions file in write-only mode.
			FileStream fs = new FileStream(fName, FileMode.Create, FileAccess.Write);
			StreamWriter sw = new StreamWriter(fs);

			// Set file pointer to beginning of file.
			sw.BaseStream.Seek(0, SeekOrigin.Begin);

			// Write all config members.
			sw.WriteLine("<Port Name>      :{0}",	this.portName);
			sw.WriteLine("<Baud Rate>      :{0}",	((int) this.baudRate).ToString());
			sw.WriteLine("<CTS Flow>       :{0}",	this.txFlowCTS.ToString());
			sw.WriteLine("<DSR Flow>       :{0}",	this.txFlowDSR.ToString());
			sw.WriteLine("<DTR Ctrl>       :{0}",	((byte) this.dtrControl).ToString());
			sw.WriteLine("<DSR Sense>      :{0}",	this.rxDSRsense.ToString());
			sw.WriteLine("<TX Continue>    :{0}",	this.txContinue.ToString());
			sw.WriteLine("<TX Xoff Flow>   :{0}",	this.txFlowXoff.ToString());
			sw.WriteLine("<RX Xoff Flow>   :{0}",	this.rxFlowXoff.ToString());
			sw.WriteLine("<Error Replace>  :{0}",	this.errReplace.ToString());
			sw.WriteLine("<Null Discard>   :{0}",	this.nulDiscard.ToString());
			sw.WriteLine("<RTS Control>    :{0}",	((byte) this.rtsControl).ToString());
			sw.WriteLine("<Abort On Error> :{0}",	this.abortOnErr.ToString());
			sw.WriteLine("<Xon Limit>      :{0}",	this.xonLimit.ToString());
			sw.WriteLine("<Xoff Limit>     :{0}",	this.xoffLimit.ToString());
			sw.WriteLine("<Bits/Byte>      :{0}",	((byte) this.dataBits).ToString());
			sw.WriteLine("<Parity>         :{0}",	((byte) this.parity).ToString());
			sw.WriteLine("<Stop Bits>      :{0}",	((byte) this.stopBits).ToString());
			sw.WriteLine("<Xon Char>       :{0:X}",	this.xonChar.ToString());
			sw.WriteLine("<Xoff Char>      :{0:X}",	this.xoffChar.ToString());
			sw.WriteLine("<Error Char>     :{0:X}",	this.errChar.ToString());
			sw.WriteLine("<EOF Char>       :{0:X}",	this.eofChar.ToString());
			sw.WriteLine("<Event Char>     :{0:X}",	this.evtChar.ToString());
			sw.WriteLine("<Handshaking>    :{0}",	((byte) this.handshake).ToString());
			sw.WriteLine("<RX Q Length>    :{0}",	this.rxQueLen.ToString());
			sw.WriteLine("<TX Q Length>    :{0}",	this.txQueLen.ToString());
			sw.WriteLine("<TX Timeout(M)>  :{0}",	this.txTmoMulti.ToString());
			sw.WriteLine("<TX Timeout(C)>  :{0}",	this.txTmoConst.ToString());
			sw.WriteLine("<Receive Mode>   :{0}",	this.receiveMode.ToString());
            sw.WriteLine("<Startup>        :{0}",   this.startup.ToString());

			// Flush to file & close.
			sw.Flush();
			fs.Close();
			return;
		}

		/// <summary> 
		/// Load the message definition list from a text file.
		/// </summary>
		public void Load(string fName)
		{
			// Quit if file doesn't exist.
			FileInfo fi = new FileInfo(fName);
			if(fi.Exists == false)
				return;

				// Open text definitions file in read-only mode.
			FileStream   fs = new FileStream(fName, FileMode.Open, FileAccess.Read);
			StreamReader sr = new StreamReader(fs);

			// Set file pointer to beginning of file.
			sr.BaseStream.Seek(0, SeekOrigin.Begin);

			string line = "";
			string data = "";
			while((line = sr.ReadLine()) != null)
			{
				data = line.Substring(line.IndexOf(":")+1);
				if(line.StartsWith("<Port Name>"))
					this.portName = data;
				else if(line.StartsWith("<Baud Rate>"))
					this.baudRate = (LineSpeed) int.Parse(data);
                else if (line.StartsWith("<CTS Flow>"))
					this.txFlowCTS = bool.Parse(data);
                else if (line.StartsWith("<DSR Flow>"))
					this.txFlowDSR = bool.Parse(data);
                else if (line.StartsWith("<DTR Ctrl>"))
					this.dtrControl = (PinState) byte.Parse(data);
                else if (line.StartsWith("<DSR Sense>"))
					this.rxDSRsense = bool.Parse(data);
                else if (line.StartsWith("<TX Continue>"))
					this.txContinue = bool.Parse(data);
                else if (line.StartsWith("<TX Xoff Flow>"))
					this.txFlowXoff = bool.Parse(data);
                else if (line.StartsWith("<RX Xoff Flow>"))
					this.rxFlowXoff = bool.Parse(data);
                else if (line.StartsWith("<Error Replace>"))
					this.errReplace = bool.Parse(data);
                else if (line.StartsWith("<Null Discard>"))
					this.nulDiscard = bool.Parse(data);
                else if (line.StartsWith("<RTS Control>"))
					this.rtsControl = (PinState) byte.Parse(data);
                else if (line.StartsWith("<Abort On Error>"))
					this.abortOnErr = bool.Parse(data);
                else if (line.StartsWith("<Xon Limit>"))
					this.xonLimit = short.Parse(data);
                else if (line.StartsWith("<Xoff Limit>"))
					this.xoffLimit = short.Parse(data);
                else if (line.StartsWith("<Bits/Byte>"))
					this.dataBits = (ByteSize) byte.Parse(data);
                else if (line.StartsWith("<Parity>"))
					this.parity = (Parity) byte.Parse(data);
                else if (line.StartsWith("<Stop Bits>"))
					this.stopBits = (StopBits) byte.Parse(data);
                else if (line.StartsWith("<Xon Char>"))
					this.xonChar = byte.Parse(data);
                else if (line.StartsWith("<Xoff Char>"))
					this.xoffChar = byte.Parse(data);
                else if (line.StartsWith("<Error Char>"))
					this.errChar = byte.Parse(data);
                else if (line.StartsWith("<EOF Char>"))
					this.eofChar = byte.Parse(data);
                else if (line.StartsWith("<Event Char>"))
					this.evtChar = byte.Parse(data);
                else if (line.StartsWith("<Handshaking>"))
					this.handshake = (Handshake) byte.Parse(data);
                else if (line.StartsWith("<RX Q Length>"))
					this.rxQueLen = int.Parse(data);
                else if (line.StartsWith("<TX Q Length>"))
					this.txQueLen = int.Parse(data);
                else if (line.StartsWith("<TX Timeout(M)>"))
					this.txTmoMulti = uint.Parse(data);
                else if (line.StartsWith("<TX Timeout(C)>"))
					this.txTmoConst = uint.Parse(data);
                else if (line.StartsWith("<Receive Mode>"))
					this.receiveMode = bool.Parse(data);
                else if (line.StartsWith("<Startup>"))
                    this.startup = uint.Parse(data);
            }
			// FClose.
			fs.Close();
			return;
		}
		#endregion

		#region Properties
		/// <summary>
		/// Get/Set Comm port identity string - i.e. "COM1:"
		/// </summary>
		public string PortName
		{
			get	{	return this.portName;		}
			set	{	this.portName = value;		}
		}
		/// <summary>
		/// Get/Set line speed baud rate (bits/seconds)
		/// </summary>
		public LineSpeed BaudRate
		{
			get	{	return this.baudRate;		}
			set	{	this.baudRate = value;		}
		}
		/// <summary>
		/// Get/Set CTS(in)/RTS(out) hardware flow control. If true, transmission 
		/// is halted unless CTS is asserted by the remote station (default: false)
		/// </summary>
		public bool TxFlowCTS
		{
			get	{	return this.txFlowCTS;		}
			set	{	this.txFlowCTS = value;		}
		}
		/// <summary>
		/// Get/Set DSR(in)/DTR(out) hardware flow control. If true, transmission 
		/// is halted unless DSR is asserted by the remote station (default: false)
		/// </summary>
		public bool TxFlowDSR
		{
			get	{	return this.txFlowDSR;		}
			set	{	this.txFlowDSR = value;		}
		}
		/// <summary>
		/// Get/Set the DTR flow control mode (default: none)
		/// </summary>
		public PinState DtrControl
		{
			get	{	return this.dtrControl;			}
			set	{	this.dtrControl = value;		}
		}
		/// <summary>
		/// Get/Set the DRS sensitivity flag. If true, received characters are ignored 
		/// unless DSR is asserted by the remote station (default: false)
		/// </summary>
		public bool RxDSRsense
		{
			get	{	return this.rxDSRsense;		}
			set	{	this.rxDSRsense = value;	}
		}
		/// <summary>
		/// Get/Set the TX continue flag. If false, transmission is suspended when this 
		/// station has sent Xoff to the remote station. If false, the remote station
		/// treats any character as an Xon.(default: true)
		/// </summary>
		public bool TxContinue
		{
			get	{	return this.txContinue;		}
			set	{	this.txContinue = value;	}
		}
		/// <summary>
		/// Get/Set the transmitter software flow control flag.(default: false)
		/// </summary>
		public bool TxFlowXoff
		{
			get	{	return this.txFlowXoff;		}
			set	{	this.txFlowXoff = value;	}
		}
		/// <summary>
		/// Get/Set the receiver software flow control flag.(default: false)
		/// </summary>
		public bool RxFlowXoff
		{
			get	{	return this.rxFlowXoff;		}
			set	{	this.rxFlowXoff = value;	}
		}
		/// <summary>
		/// Get/Set the error replace flag. If TRUE, bytes received with parity errors are 
		/// replaced with the character specified by the ErrorChar member. If this member 
		/// is TRUE and the fParity member is TRUE, replacement occurs.
		/// </summary>
		public bool ErrReplace
		{
			get	{	return this.errReplace;		}
			set	{	this.errReplace = value;	}
		}
		/// <summary>
		/// Get/Set the null discard flag. If TRUE, null bytes are discarded when received. 
		/// </summary>
		public bool NullDiscard
		{
			get	{	return this.nulDiscard;		}
			set	{	this.nulDiscard = value;	}
		}
		/// <summary>
		/// Get/Set the RTS flow control mode (default: none)
		/// </summary>
		public PinState RtsControl
		{
			get	{	return this.rtsControl;		}
			set	{	this.rtsControl = value;	}
		}
		/// <summary>
		/// Get/Set the abort on error flag. Read and write operations are terminated on error.
		/// </summary>
		public bool AbortOnErr
		{
			get	{	return this.abortOnErr;		}
			set	{	this.abortOnErr = value;	}
		}
		/// <summary>
		/// Get/Set the minimum number of bytes allowed in the input buffer before flow control
		/// is activated to inhibit the sender.
		/// </summary>
		public short XonLimit
		{
			get	{	return this.xonLimit;		}
			set	{	this.xonLimit = value;	}
		}
		/// <summary>
		/// Get/Set the maximum number of bytes allowed in the input buffer before flow control
		/// is activated to allow transmission by the sender.
		/// </summary>
		public short XoffLimit
		{
			get	{	return this.xoffLimit;		}
			set	{	this.xoffLimit = value;	}
		}

		/// <summary>
		/// Get/Set number of data bits/byte
		/// </summary>
		public ByteSize DataBits
		{
			get	{	return this.dataBits;		}
			set	{	this.dataBits = value;		}
		}
		/// <summary>
		/// Get/Set character parity.
		/// </summary>
		public Parity Parity
		{
			get	{	return this.parity;			}
			set	{	this.parity = value;		}
		}
		/// <summary>
		/// Get/Set number of stop bits following each character
		/// </summary>
		public StopBits StopBits
		{
			get	{	return this.stopBits;		}
			set	{	this.stopBits = value;		}
		}
		/// <summary>
		/// Get/Set the Xon for software flow control (default: DC1)
		/// </summary>
		public byte XonChar
		{
			get	{	return this.xonChar;		}
			set	{	this.xonChar = value;		}
		}
		/// <summary>
		/// Get/Set the Xoff for software flow control (default: DC3)
		/// </summary>
		public byte XoffChar
		{
			get	{	return this.xoffChar;		}
			set	{	this.xoffChar = value;		}
		}
		/// <summary>
		/// Get/Set the error replacement character.
		/// </summary>
		public byte ErrChar
		{
			get	{	return this.errChar;		}
			set	{	this.errChar = value;		}
		}
		/// <summary>
		/// Get/Set the end-of-file character.
		/// </summary>
		public byte EofChar
		{
			get	{	return this.eofChar;		}
			set	{	this.eofChar = value;		}
		}
		/// <summary>
		/// Get/Set the event character.
		/// </summary>
		public byte EvtChar
		{
			get	{	return this.evtChar;		}
			set	{	this.evtChar = value;		}
		}
		/// <summary>
		/// Get/Set the flow control style (default: None)
		/// </summary>
		public Handshake FlowCtrl
		{
			get	{	return this.handshake;		}
			set	{	this.handshake = value;		}
		}
		/// <summary>
		/// Get/Set the size of receive queue (default: 0 = use operating system default)
		/// </summary>
		public int RxQueLen
		{
			get	{	return this.rxQueLen;		}
			set	{	this.rxQueLen = value;		}
		}
		/// <summary>
		/// Get/Set the size of transmit queue (default: 0 = use operating system default)
		/// </summary>
		public int TxQueLen
		{
			get	{	return this.txQueLen;		}
			set	{	this.txQueLen = value;		}
		}
		/// <summary>
		/// Get/Set the max time for Send in ms = (Multiplier * Characters) + Constant
		/// (default: 0 = No timeout)
		/// </summary>
		public uint TxTmoMulti
		{
			get	{	return this.txTmoMulti;		}
			set	{	this.txTmoMulti = value;	}
		}
		/// <summary>
		/// Get/Set the max time for Send in ms = (Multiplier * Characters) + Constant (default: 0)
		/// </summary>
		public uint TxTmoConst
		{
			get	{	return this.txTmoConst;		}
			set	{	this.txTmoConst = value;	}
		}
		/// <summary>
		/// Get/Set the receiver mode. If true, every character received is sent to the "OnRxChar"
		/// method immediately. If false, the user must poll periodically to receive any/all chars
		/// currently available in the receive queue.
		/// </summary>
		public bool ReceiveMode
		{
			get	{	return this.receiveMode;	}
			set	{	this.receiveMode = value;	}
		}
		#endregion
	}
}
