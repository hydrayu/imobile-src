using System;
using System.Text;
using System.Runtime.InteropServices;

namespace iMobileBarcode
{
	/// <summary>
	/// Wrapper class controlling access to the COMMSTAT and flag structures and
	/// kernel32.dll function: ClearCommError()
	/// </summary>
	internal class Win32Status 
	{
		#region Constants
		/*********************************************************************/
		/******************** ERROR CONSTANTS - WINBASE.H ********************/
		/*********************************************************************/
		/// <summary>
		/// An input buffer overflow has occurred. 
		/// There is either no room in the input buffer, 
		/// or a character was received after the EOF character.
		/// </summary>
		internal const UInt32 CE_RXOVER = 0x0001;
		/// <summary>
		/// A character-buffer overrun has occurred. 
		/// The next character is lost.
		/// </summary>
		internal const UInt32 CE_OVERRUN = 0x0002;
		/// <summary>
		/// The hardware detected a parity error.
		/// </summary>
		internal const UInt32 CE_RXPARITY = 0x0004;
		/// <summary>
		/// The hardware detected a framing error.
		/// </summary>
		internal const UInt32 CE_FRAME = 0x0008;
		/// <summary>
		/// The hardware detected a break condition
		/// </summary>
		internal const UInt32 CE_BREAK = 0x0010;
		/// <summary>
		/// The application tried to transmit a 
		/// character, but the output buffer was full.
		/// </summary>
		internal const UInt32 CE_TXFULL = 0x0100;
		/// <summary>
		/// Windows 95/98/Me: A time-out occurred on a parallel device.
		/// </summary>
		internal const UInt32 CE_PTO = 0x0200;
		/// <summary>
		/// An I/O error occurred during communications with the device.
		/// </summary>
		internal const UInt32 CE_IOE = 0x0400;
		/// <summary>
		/// Windows 95/98/Me: A parallel device is not selected.
		/// </summary>
		internal const UInt32 CE_DNS = 0x0800;
		/// <summary>
		/// Windows 95/98/Me: A parallel device signaled that it is out of paper.
		/// </summary>
		internal const UInt32 CE_OOP = 0x1000;
		/// <summary>
		/// The requested mode is not supported, or the file handle 
		/// parameter is invalid. If this value is specified, it is the only valid error.
		/// </summary>
		internal const UInt32 CE_MODE = 0x8000;

		/*********************************************************************/
		/******************** COMSTAT BITFIELD CONSTANTS *********************/
		/*********************************************************************/

		/// <summary>
		/// Indicates whether transmission is waiting 
		/// for the CTS (clear-to-send) signal to be sent. 
		/// If this member is TRUE, transmission is waiting.
		/// </summary>
		internal const uint CTS_HOLD_BIT = 0x1;
		/// <summary>
		/// Indicates whether transmission is waiting 
		/// for the DSR (data-set-ready) signal to be sent. 
		/// If this member is TRUE, transmission is waiting.
		/// </summary>
		internal const uint DSR_HOLD_BIT = 0x2;
		/// <summary>
		/// Indicates whether transmission is waiting for 
		/// the RLSD (receive-line-signal-detect) signal 
		/// to be sent. If this member is TRUE, transmission is waiting.
		/// </summary>
		internal const uint RLSD_HOLD_BIT = 0x4;
		/// <summary>
		/// Indicates whether transmission is waiting 
		/// because the XOFF character was received. 
		/// If this member is TRUE, transmission is waiting. 
		/// </summary>
		internal const uint XOFF_HOLD_BIT = 0x8;
		/// <summary>
		/// Indicates whether transmission is waiting 
		/// because the XOFF character was transmitted. 
		/// If this member is TRUE, transmission is waiting. 
		/// Transmission halts when the XOFF character is 
		/// transmitted to a system that takes the next 
		/// character as XON, regardless of the actual character.
		/// </summary>
		internal const uint XOFF_SENT_BIT = 0x10;
		/// <summary>
		/// Indicates whether the end-of-file (EOF) character 
		/// has been received. If this member is TRUE, the 
		/// EOF character has been received.
		/// </summary>
		internal const uint EOF_BIT = 0x20;
		/// <summary>
		/// If this member is TRUE, there is a character 
		/// queued for transmission that has come to the 
		/// communications device by way of the TransmitCommChar 
		/// function. The communications device transmits such a 
		/// character ahead of other characters in the device's output buffer.
		/// </summary>
		internal const uint TRIM_BIT = 0x40;
		#endregion

		#region Structures
		/// <summary>
		/// The COMMSTAT structure contains information about a communications 
		/// device. This structure is filled by the ClearCommError function.
		/// </summary>
		[StructLayout( LayoutKind.Sequential )]
		protected internal struct COMMSTAT 
		{
			/// <summary>
			/// Packed status bitfield.
			/// </summary>
			internal UInt32	bitfield;
			/// <summary>
			/// Number of bytes received by the serial provider 
			/// but not yet read by a ReadFile operation.
			/// </summary>
			internal UInt32 cbInQue;
			/// <summary>
			/// Number of bytes of user data remaining to be 
			/// transmitted for all write operations. This value 
			/// will be zero for a nonoverlapped write.
			/// </summary>
			internal UInt32 cbOutQue;
		}

		/// <summary>
		/// ClearCommError() lpErrors parameter converted to flags.
		/// </summary>
		protected internal struct COMMERRS
		{
			internal bool overflowError;
			internal bool overrunError;
			internal bool parityError;
			internal bool framingError;
			internal bool breakCondition;
			internal bool txBufFullError;
			internal bool parallelTmoutError;
			internal bool deviceIOError;
			internal bool notSelectedError;
			internal bool outOfPaperError;
			internal bool modeHandleError;
			internal uint status;
		};
		/// <summary>
		/// COMMSTAT structure's bitfield member converted to flags.
		/// </summary>
		protected internal struct COMMHOLD
		{
			internal bool ctsHold;
			internal bool dsrHold;
			internal bool rlsdHold;
			internal bool xoffHold;
			internal bool xoffSent;
			internal bool eof;
			internal bool trim;
		};
		#endregion

		#region Imports
		/// <summary>
		/// The ClearCommError function retrieves information about a 
		/// communications error and reports the current status of a 
		/// communications device. The function is called when a 
		/// communications error occurs, and it clears the device's 
		/// error flag to enable additional input and output (I/O) operations.
		/// </summary>
		[DllImport("kernel32.dll")]
		private static extern Boolean ClearCommError
			(
			IntPtr hFile, 
			out UInt32 lpErrors, 
			IntPtr lpStat
			);
		[DllImport("kernel32.dll")]
		private static extern Boolean ClearCommError
			(
			IntPtr hFile, 
			out UInt32 lpErrors, 
			out COMMSTAT cs
			);
		#endregion

		#region Members
		/// <summary>
		/// COMMSTAT structure.
		/// </summary>
		private COMMSTAT cs;
		/// <summary>
		/// COMMERRS structure.
		/// </summary>
		private COMMERRS error;
		/// <summary>
		/// COMMHOLD structure.
		/// </summary>
		private COMMHOLD state;
		/// <summary>
		/// Number of chars ready to read.
		/// </summary>
		private uint ready;
		/// <summary>
		/// Class fault string.
		/// </summary>
		private string fault;
		#endregion

		#region Constructors
		/// <summary>
		/// Default constructor. Creates the class structures.
		/// </summary>
		protected internal Win32Status()
		{
			this.ready  = 0;
			this.cs		= new COMMSTAT();
			this.state  = new COMMHOLD();
			this.error  = new COMMERRS();
			return;
		}
		#endregion

		#region Methods
		/// <summary>
		/// Update status and clear errors.
		/// </summary>
		/// <param name="handle">Comm port handle.</param>
		/// <returns>True if successful.</returns>
		protected internal bool Clear(IntPtr handle)
		{
			if(ClearCommError(handle, out this.error.status, out this.cs) == false)
			{
				this.SetFault("ClearCommError(Clear)");
				return false;
			}
			this.error.overflowError = ((this.error.status & CE_RXOVER)!=0);
			this.error.overrunError = ((this.error.status & CE_OVERRUN)!=0);
			this.error.parityError = ((this.error.status & CE_RXPARITY)!=0);
			this.error.framingError = ((this.error.status & CE_FRAME)!=0);
			this.error.breakCondition = ((this.error.status & CE_BREAK)!=0);
			this.error.txBufFullError = ((this.error.status & CE_TXFULL)!=0);
			this.error.parallelTmoutError = ((this.error.status & CE_PTO)!=0);
			this.error.deviceIOError = ((this.error.status & CE_IOE)!=0);
			this.error.notSelectedError = ((this.error.status & CE_DNS)!=0);
			this.error.outOfPaperError = ((this.error.status & CE_OOP)!=0);
			this.error.modeHandleError = ((this.error.status & CE_MODE)!=0);

			this.state.ctsHold = ((this.cs.bitfield & CTS_HOLD_BIT)!=0);
			this.state.dsrHold = ((this.cs.bitfield & DSR_HOLD_BIT)!=0);
			this.state.rlsdHold = ((this.cs.bitfield & RLSD_HOLD_BIT)!=0);
			this.state.xoffHold = ((this.cs.bitfield & XOFF_HOLD_BIT)!=0);
			this.state.xoffSent = ((this.cs.bitfield & XOFF_SENT_BIT)!=0);
			this.state.eof = ((this.cs.bitfield & EOF_BIT)!=0);
			this.state.trim = ((this.cs.bitfield & TRIM_BIT)!=0);

			this.ready = this.cs.cbInQue;
			return true;
		}

		/// <summary>
		/// Update status and clear errors.
		/// </summary>
		/// <param name="handle">Comm port handle.</param>
		/// <returns>True if successful and no error events.</returns>
		internal bool Reset(IntPtr handle)
		{
			UInt32 errors;

			// Get, then clear current error status.
			if(ClearCommError(handle, out errors, IntPtr.Zero) == false)
			{
				this.SetFault("ClearCommError(Reset)");
				return false;
			}
			else
			{
				// If no errors or if CE_BREAK is the only error, ignore it.
				if((errors > 0) && (errors != CE_BREAK))
				{
					this.fault = "COMM Error(s):";
					if((errors & CE_RXOVER) != 0)		this.fault += "\tRX Overflow\r\n";
					if((errors & CE_OVERRUN) != 0)		this.fault += "\tOverrun\r\n";
					if((errors & CE_RXPARITY) != 0)		this.fault += "\tParity\r\n";
					if((errors & CE_FRAME) != 0)		this.fault += "\tFraming\r\n";
					if((errors & CE_BREAK) != 0)		this.fault += "\tBreak\r\n";
					if((errors & CE_TXFULL) != 0)		this.fault += "\tTX Overflow\r\n";
					if((errors & CE_PTO) != 0)			this.fault += "\tLPT Tmout\r\n";
					if((errors & CE_IOE) != 0)			this.fault += "\tIO Error\r\n";
					if((errors & CE_DNS) != 0)			this.fault += "\tDevice Not Selected\r\n";
					if((errors & CE_OOP) != 0)			this.fault += "\tOut-of-Paper\r\n";
					if((errors & CE_MODE) != 0)			this.fault += "\tMode/Handle\r\n";
					return false;
				}
				return true;
			}
		}

		/// <summary>
		/// Returns the TX/RX queue sizes and bytes left.
		/// </summary>
		/// <returns>Queue size string.</returns>
		internal string QueueString()
		{
			StringBuilder s = new StringBuilder("The RX queue ", 80);
			if(this.cs.cbInQue == 0)
			{
				s.Append("is empty.");
			}
			else if(this.cs.cbInQue == 1)
			{
				s.Append("contains 1 byte.");
			}
			else
			{
				s.Append("contains ");
				s.Append(this.cs.cbInQue.ToString());
				s.Append(" bytes.");
			}
			s.Append("\r\n");

			s.Append("The TX queue ");
			if(this.cs.cbOutQue == 0)
			{
				s.Append("is empty.");
			}
			else if(this.cs.cbOutQue == 1)
			{
				s.Append("contains 1 byte.");
			}
			else
			{
				s.Append("contains ");
				s.Append(this.cs.cbOutQue.ToString());
				s.Append(" bytes.");
			}
			s.Append("\r\n");

			s.Append("The immediate buffer is ");
			if(this.state.trim)
				s.Append("full.");
			else
				s.Append("empty.");
			s.Append("\r\n");
			return s.ToString();
		}

		/// <summary>
		/// Returns the TX hold state string.
		/// </summary>
		/// <returns>State string.</returns>
		internal string StateString()
		{
			if(this.cs.cbOutQue > 0)
			{
				StringBuilder s = new StringBuilder("The TX queue is ", 80);
				if(this.state.ctsHold || this.state.dsrHold || 
					this.state.rlsdHold || this.state.xoffHold || state.xoffSent)
				{
					s.Append("holding on ");
					if(this.state.ctsHold)
						s.Append("CTS ");
					if(this.state.dsrHold)
						s.Append("DSR ");
					if(this.state.rlsdHold)
						s.Append("RLSD ");
					if(this.state.xoffHold)
						s.Append("Rx XOff ");
					if(this.state.xoffSent)
						s.Append("Tx XOff ");
				}
				else
				{
					s.Append("sending data.");
				}
				s.Append("\r\n");
				return s.ToString();
			}
			return "";
		}
		/// <summary>
		/// Sets the class fault string.
		/// </summary>
		/// <param name="who">Name of kernel function called.</param>
		private void SetFault(string who)
		{
			this.fault = who + "Failed. System Returned Error Code: " + 
				Marshal.GetLastWin32Error().ToString();
			return;
		}
		#endregion

		#region Properties
		/// <summary>
		/// Output is blocked by CTS handshaking.
		/// </summary>
		internal bool CtsHold
		{
			get	{	return this.state.ctsHold;		}
		}
		/// <summary>
		/// Output is blocked by DRS handshaking.
		/// </summary>
		internal bool DsrHold
		{
			get	{	return this.state.dsrHold;		}
		}
		/// <summary>
		/// Output is blocked by RLSD handshaking.
		/// </summary>
		internal bool RlsdHold
		{
			get	{	return this.state.rlsdHold;		}
		}
		/// <summary>
		/// Output is blocked because software handshaking is enabled and XOFF was received.
		/// </summary>
		internal bool XoffHold
		{
			get	{	return this.state.xoffHold;		}
		}
		/// <summary>
		/// Output was blocked because XOFF was sent and this station is not yet ready to receive.
		/// </summary>
		internal bool XoffSent
		{
			get	{	return this.state.xoffSent;		}
		}
		
		/// <summary>
		/// There is a character waiting for transmission in the immediate buffer.
		/// </summary>
		internal bool ImmediateWaiting
		{
			get	{	return this.state.trim;		}
		}
		/// <summary>
		/// Number of bytes waiting in the receive queue.
		/// </summary>
		internal uint RxQueCount
		{
			get	{	return this.cs.cbInQue;		}
		}
		/// <summary>
		/// Number of bytes waiting for transmission.
		/// </summary>
		internal uint TxQueCount
		{
			get	{	return this.cs.cbOutQue;	}
		}
		/// <summary>
		/// Get a copy of the error status.
		/// </summary>
		internal uint Status
		{
			get	{	return this.error.status;	}
		}
//		/// <summary>
//		/// Get a copy of the state record.
//		/// </summary>
//		internal COMMHOLD State
//		{
//			get	{	return this.state;			}
//		}
		/// <summary>
		/// Get the last class fault description string.
		/// </summary>
		internal string Fault
		{
			get	{	return this.fault;		}
		}
		/// <summary>
		/// RX queue ready count
		/// </summary>
		internal uint Ready
		{
			get	{	return this.ready;		}
		}
		#endregion
	}
}
