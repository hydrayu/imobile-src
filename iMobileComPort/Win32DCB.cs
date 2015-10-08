using System;
using System.Text;
using System.Runtime.InteropServices;

namespace iMobileComPort
{
	/// <summary>
	/// Wrapper class controlling access to the DCB structure and
	/// kernel32.dll functions: GetCommState(), SetCommState().
	/// </summary>
	internal class Win32DCB
	{
		#region Constants
		/*********************************************************************/
		/***************** DTR CONTROL CONSTANTS - WINBASE.H *****************/
		/*********************************************************************/
		/// <summary>
		/// Disables the DTR line when the device is opened and leaves it disabled.
		/// </summary>
		internal const UInt32 DTR_CONTROL_DISABLE	= 0x00;
		/// <summary>
		/// Enables the DTR line when the device is opened and leaves it on.
		/// </summary>
		internal const UInt32 DTR_CONTROL_ENABLE	= 0x01;
		/// <summary>
		/// Enables DTR handshaking. If handshaking is enabled, it is an error for the 
		/// application to adjust the line by using the EscapeCommFunction function.
		/// </summary>
		internal const UInt32 DTR_CONTROL_HANDSHAKE = 0x02;
		/*********************************************************************/
		/***************** RTS CONTROL CONSTANTS - WINBASE.H *****************/
		/*********************************************************************/
		/// <summary>
		/// Disables the RTS line when the device is opened and leaves it disabled.
		/// </summary>
		internal const UInt32 RTS_CONTROL_DISABLE	= 0x00;
		/// <summary>
		/// Enables the RTS line when the device is opened and leaves it on.
		/// </summary>
		internal const UInt32 RTS_CONTROL_ENABLE	= 0x01;
		/// <summary>
		/// Enables RTS handshaking. The driver raises the RTS line when the 
		/// "type-ahead" (input) buffer is less than one-half full and lowers the 
		/// RTS line when the buffer is more than three-quarters full. If handshaking 
		/// is enabled, it is an error for the application to adjust the line by using 
		/// the EscapeCommFunction function.
		/// </summary>
		internal const UInt32 RTS_CONTROL_HANDSHAKE	= 0x02;
		/// <summary>
		/// Windows NT/2000/XP: Specifies that the RTS line will be high if bytes are 
		/// available for transmission. After all buffered bytes have been sent, the 
		/// RTS line will be low.
		/// </summary>
		internal const UInt32 RTS_CONTROL_TOGGLE	= 0x03;
		#endregion

		#region Structures
		/// <summary>
		/// The DCB structure defines the control setting for a serial communications device. 
		/// </summary>
		[StructLayout( LayoutKind.Sequential )]
		internal struct DCB 
		{
			/// <summary>
			/// Length, in bytes, of the DCB structure
			/// </summary>
			internal Int32	dcbLength;
			/// <summary>
			/// Baud rate at which the communications device operates.
			/// Supported Rates: 110, 300, 600, 1200, 2400, 4800, 9600
			/// 14400, 19200, 38400, 56000, 57600, 115200, 128000, 256000
			/// </summary>
			internal Int32	baudRate;
			/// <summary>
			/// Packed bitfield from win32 struct. 
			/// </summary>
			/// fBinary:1 - Indicates whether binary mode is enabled. 
			/// Windows does not support nonbinary mode transfers, so this member must be TRUE.
			///   
			/// fParity:1 - Indicates whether parity checking is enabled. 
			/// If this member is TRUE, parity checking is performed and errors are reported. 
			/// 
			/// fOutxCtsFlow:1 - Indicates whether the CTS (clear-to-send) signal is monitored 
			/// for output flow control. If this member is TRUE and CTS is turned off, output 
			/// is suspended until CTS is sent again. 
			/// 
			/// fOutxDsrFlow:1 - Indicates whether the DSR (data-set-ready) signal is monitored 
			/// for output flow control. If this member is TRUE and DSR is turned off, output is 
			/// suspended until DSR is sent again. 
			/// 
			/// fDtrControl:2 - DTR (data-terminal-ready) flow control. This member can be one 
			/// of the following values. 
			///		DTR_CONTROL_DISABLE		Disables the DTR line when the device is opened and
			///								leaves it disabled. 
			///		DTR_CONTROL_ENABLE		Enables the DTR line when the device is opened and
			///								leaves it on. 
			///		DTR_CONTROL_HANDSHAKE	Enables DTR handshaking. If handshaking is enabled,
			///								it is an error for the application to adjust the line
			///								by using the EscapeCommFunction function. 
			/// 
			/// fDsrSensitivity:1 - Indicates whether the communications driver is sensitive to 
			/// the state of the DSR signal. If this member is TRUE, the driver ignores any bytes 
			/// received, unless the DSR modem input line is high. 
			/// 
			/// fTXContinueOnXoff:1 - Indicates whether transmission stops when the input buffer 
			/// is full and the driver has transmitted the XoffChar character. If this member is 
			/// TRUE, transmission continues after the input buffer has come within XoffLim bytes 
			/// of being full and the driver has transmitted the XoffChar character to stop 
			/// receiving bytes. If this member is FALSE, transmission does not continue until 
			/// the input buffer is within XonLim bytes of being empty and the driver has 
			/// transmitted the XonChar character to resume reception. 
			/// 
			/// fOutX:1 - Indicates whether XON/XOFF flow control is used during transmission. 
			/// If this member is TRUE, transmission stops when the XoffChar character is received 
			/// and starts again when the XonChar character is received.
			/// 
			/// fInX:1 -  Indicates whether XON/XOFF flow control is used during reception. 
			/// If this member is TRUE, the XoffChar character is sent when the input buffer 
			/// comes within XoffLim bytes of being full, and the XonChar character is sent 
			/// when the input buffer comes within XonLim bytes of being empty. 
			/// 
			/// fErrorChar: 1 -  Indicates whether bytes received with parity errors are replaced 
			/// with the character specified by the ErrorChar member. If this member is TRUE and 
			/// the fParity member is TRUE, replacement occurs.
			/// 
			/// fNull:1 - Indicates whether null bytes are discarded. If this member is TRUE, 
			/// null bytes are discarded when received.
			/// 
			/// fRtsControl:2 - RTS (request-to-send) flow control. This member can be one of the 
			/// following values.
			///		RTS_CONTROL_DISABLE		Disables the RTS line when the device is opened and 
			///								leaves it disabled. 
			///		RTS_CONTROL_ENABLE		Enables the RTS line when the device is opened and 
			///								leaves it on. 
			///		RTS_CONTROL_HANDSHAKE	Enables RTS handshaking. The driver raises the RTS line
			///								when the "type-ahead" (input) buffer is less than 1/2 
			///								full and lowers the RTS line when the buffer is more than
			///								three-quarters full. If handshaking is enabled, it is 
			///								an error for the application to adjust the line by using
			///								the EscapeCommFunction function.
			///		RTS_CONTROL_TOGGLE		Windows NT/2000/XP: Specifies that the RTS line will be
			///								high if bytes are available for transmission. After all
			///								buffered bytes have been sent, the RTS line will be low. 
			/// 
			/// fAbortOnError:1 - Indicates whether read and write operations are terminated if an 
			/// error occurs. If this member is TRUE, the driver terminates all read and write 
			/// operations with an error status if an error occurs. The driver will not accept 
			/// any further communications operations until the application has acknowledged the 
			/// error by calling the ClearCommError function.
			/// 
			/// fDummy2:17 - Reserved; do not use. 
			internal Int32	bitfield;
			/// <summary>
			/// Reserved; must be zero.
			/// </summary>
			internal Int16	wReserved;
			/// <summary>
			/// Minimum number of bytes allowed in the input buffer before flow control 
			/// is activated to inhibit the sender. Note that the sender may transmit 
			/// characters after the flow control signal has been activated, so this value 
			/// should never be zero. This assumes that either XON/XOFF, RTS, or DTR input 
			/// flow control is specified in fInX, fRtsControl, or fDtrControl.
			/// </summary>
			internal Int16	xonLim;
			/// <summary>
			/// Maximum number of bytes allowed in the input buffer before flow control 
			/// is activated to allow transmission by the sender. This assumes that either 
			/// XON/XOFF, RTS, or DTR input flow control is specified in fInX, fRtsControl, 
			/// or fDtrControl. The maximum number of bytes allowed is calculated by 
			/// subtracting this value from the size, in bytes, of the input buffer.
			/// </summary>
			internal Int16	xoffLim;
			/// <summary>
			/// Number of bits in the bytes transmitted and received. 
			/// </summary>
			internal Byte	byteSize;
			/// <summary>
			/// Parity scheme to be used. This member can be one of the following values.
			/// Even, Mark, None, Odd, Space 
			/// </summary>
			internal Byte	prtyByte;
			/// <summary>
			/// Number of stop bits to be used. This member can be 1, 1.5, or 2 stop bits.
			/// </summary>
			internal Byte	stopBits;
			/// <summary>
			/// Value of the XON character for both transmission and reception. 
			/// </summary>
			internal Byte	xonChar;
			/// <summary>
			/// Value of the XOFF character for both transmission and reception. 
			/// </summary>
			internal Byte	xoffChar;
			/// <summary>
			/// Value of the character used to replace bytes received with a parity error.
			/// </summary>
			internal Byte	errorChar;
			/// <summary>
			/// Value of the character used to signal the end of data.
			/// </summary>
			internal Byte	eofChar;
			/// <summary>
			/// Value of the character used to signal an event.
			/// </summary>
			internal Byte	evtChar;
			/// <summary>
			/// Reserved; do not use.
			/// </summary>
			internal Int16	wReserved1;
		}
		#endregion

		#region Imports
		/// <summary>
		/// The GetCommState function retrieves the current control settings for 
		/// a specified communications device.
		/// </summary>
		[DllImport("kernel32.dll")]
		private static extern Boolean GetCommState
			(
			IntPtr hFile, 
			ref DCB lpDCB
			);
		/// <summary>
		/// The SetCommState function configures a communications device according to the 
		/// specifications in a device control block (a DCB structure). The function 
		/// reinitializes all hardware and control settings, but it does not empty output 
		/// or input queues.
		/// </summary>
		[DllImport("kernel32.dll")]
		private static extern Boolean SetCommState
			(
			IntPtr hFile, 
			[In] ref DCB lpDCB
			);
		#endregion

		#region Members
		private DCB dcb;
		private string fault;
		#endregion

		#region Constructor
		/// <summary>
		/// Create the device control block (DCB) for the associated comm port.
		/// Sets the DCB fields to match the passed configuration.
		/// </summary>
		/// <param name="cfg">Reference to user defined port config.</param>
		internal Win32DCB(SerialCnfg cfg)
		{
			this.dcb = new DCB();

			this.dcb.dcbLength = Marshal.SizeOf(this.dcb);
			this.dcb.baudRate = (int) cfg.BaudRate;
			this.dcb.bitfield = 0x8001;
				
			if((cfg.Parity == Parity.Odd) || 
				(cfg.Parity == Parity.Even))
				this.dcb.bitfield |= 0x0002;
			if (cfg.TxFlowCTS)
				this.dcb.bitfield |= 0x0004;
			if (cfg.TxFlowDSR)
				this.dcb.bitfield |= 0x0008;
			this.dcb.bitfield |= ( ((int) cfg.DtrControl  &  0x0003) << 4);

			if (cfg.RxDSRsense)
				this.dcb.bitfield |= 0x0040;
			if (cfg.TxContinue)
				this.dcb.bitfield |= 0x0080;

			if (cfg.TxFlowXoff)
				this.dcb.bitfield |= 0x0100;
			if (cfg.RxFlowXoff)
				this.dcb.bitfield |= 0x0200;
			this.dcb.bitfield |= ( ((int) cfg.RtsControl  &  0x0003) << 12);

			this.dcb.byteSize = (byte) cfg.DataBits;
			this.dcb.prtyByte = (byte) cfg.Parity;
			this.dcb.stopBits = (byte) cfg.StopBits;
			this.dcb.xoffChar = (byte) cfg.XoffChar;
			this.dcb.xonChar  = (byte) cfg.XonChar;
			return;
		}
		#endregion

		#region Methods
		/// <summary>
		/// Read the device control settings to the class DCB structure.
		/// </summary>
		/// <param name="handle">Comm port handle.</param>
		/// <returns>True if successful.</returns>
		internal bool Get(IntPtr handle)
		{
			if(GetCommState(handle, ref this.dcb) == false)
			{
				this.SetFault("GetCommState()");
				return false;
			}
			return true;
		}

		/// <summary>
		/// Write the device control settings from the class DCB structure.
		/// </summary>
		/// <param name="handle">Comm port handle.</param>
		/// <returns>True if successful.</returns>
		internal bool Set(IntPtr handle)
		{
			if(SetCommState(handle, ref this.dcb) == false)
			{
				this.SetFault("SetCommState()");
				return false;
			}
			return true;
		}
		/// <summary>
		/// Set the Xon/Xoff limits in the DCB.	NOTE: be very careful when
		/// overriding the default limits. Buffer overflow may result.
		/// </summary>
		/// <param name="cfg">Reference to user defined port config.</param>
		/// <param name="rxQueLen">Receiver queue length.</param>
		/// <returns></returns>
		internal void Limits(SerialCnfg cfg, uint rxQueLen)
		{
			// If the RX queue length is known (>0), default to 10% cushion.
			// If the queue size is unknown, set very low defaults for safety.
			if(cfg.XonLimit == 0)
				this.dcb.xonLim  = (short) ((rxQueLen > 0) ? (rxQueLen/10) : 8);
			else
				this.dcb.xonLim = cfg.XonLimit;

			if(cfg.XoffLimit == 0)
				this.dcb.xoffLim = (short) ((rxQueLen > 0) ? (rxQueLen/10) : 8);
			else
				this.dcb.xoffLim = cfg.XoffLimit;
			return;
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
		/// Get the DCB structure size.
		/// </summary>
		internal int DcbLength
		{
			get	{	return this.dcb.dcbLength;	}
		}
		/// <summary>
		/// Get/Set the line speed in bits/second.
		/// </summary>
		internal int BaudRate
		{
			get	{	return this.dcb.baudRate;	}
			set	{	this.dcb.baudRate = value;	}
		}
		/// <summary>
		/// Get/Set the packed bitfield.
		/// </summary>
		internal int Bitfield
		{
			get	{	return this.dcb.bitfield;	}
			set	{	this.dcb.bitfield = value;	}
		}
		/// <summary>
		/// Get/Set the soft flow control ON limit.
		/// </summary>
		internal short XonLimit
		{
			get	{	return this.dcb.xonLim;		}
			set	{	this.dcb.xonLim = value;	}
		}
		/// <summary>
		/// Get/Set the soft flow control OFF limit.
		/// </summary>
		internal short XoffLimit
		{
			get	{	return this.dcb.xoffLim;	}
			set	{	this.dcb.xoffLim = value;	}
		}
		/// <summary>
		/// Get/Set the data character size in bits.
		/// </summary>
		internal byte ByteSize
		{
			get	{	return this.dcb.byteSize;	}
			set	{	this.dcb.byteSize = value;	}
		}
		/// <summary>
		/// Get/Set the data character parity.
		/// </summary>
		internal byte ParityBits
		{
			get	{	return this.dcb.prtyByte;	}
			set	{	this.dcb.prtyByte = value;	}
		}
		/// <summary>
		/// Get/Set the number of character stop bits.
		/// </summary>
		internal byte StopBits
		{
			get	{	return this.dcb.stopBits;	}
			set	{	this.dcb.stopBits = value;	}
		}
		/// <summary>
		/// Get/Set the XON flow control character.
		/// </summary>
		internal byte XonChar
		{
			get	{	return this.dcb.xonChar;	}
			set	{	this.dcb.xonChar = value;	}
		}
		/// <summary>
		/// Get/Set the XOFF flow control character.
		/// </summary>
		internal byte XoffChar
		{
			get	{	return this.dcb.xoffChar;	}
			set	{	this.dcb.xoffChar = value;	}
		}
		/// <summary>
		/// Get/Set the error character.
		/// </summary>
		internal byte ErrorChar
		{
			get	{	return this.dcb.errorChar;	}
			set	{	this.dcb.errorChar = value;	}
		}
		/// <summary>
		/// Get/Set the end-of-file character.
		/// </summary>
		internal byte EofChar
		{
			get	{	return this.dcb.eofChar;	}
			set	{	this.dcb.eofChar = value;	}
		}
		/// <summary>
		/// Get/Set the event signaling character.
		/// </summary>
		internal byte EventChar
		{
			get	{	return this.dcb.evtChar;	}
			set	{	this.dcb.evtChar = value;	}
		}
		/// <summary>
		/// Get the last class fault description string.
		/// </summary>
		internal string Fault
		{
			get	{	return this.fault;		}
		}
		#endregion
	}
}
