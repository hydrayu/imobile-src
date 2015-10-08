using System;

namespace iMobileComPort
{
	/// <summary>
	/// Baud rate settings.
	/// Supported Rates: 110, 300, 600, 1200, 2400, 4800, 9600
	/// 14400, 19200, 38400, 56000, 57600, 115200, 128000, 256000
	/// </summary>
	public enum LineSpeed : int
	{
		Baud_110	= 110,
		Baud_300	= 300,
		Baud_600	= 600,
		Baud_1200	= 1200,
		Baud_2400	= 2400,
		Baud_4800	= 4800,
		Baud_9600	= 9600,
		Baud_14400	= 14400,
		Baud_19200	= 19200,
		Baud_38400	= 38400,
		Baud_56000	= 56000,
		Baud_57600	= 57600,
		Baud_115200	= 115200,
		Baud_128000	= 128000,
		Baud_256000	= 256000,
	};

	/// <summary>
	/// Parity settings
	/// </summary>
	public enum Parity : byte
	{
		/// <summary>
		/// Characters do not have a parity bit.
		/// </summary>
		None = 0,
		/// <summary>
		/// Set if there are an odd number of 1s in the character.
		/// </summary>
		Odd = 1,
		/// <summary>
		/// Set if there are an even number of 1s in the character.
		/// </summary>
		Even = 2,
		/// <summary>
		/// The parity bit is always 1.
		/// </summary>
		Mark = 3,
		/// <summary>
		/// The parity bit is always 0.
		/// </summary>
		Space = 4
	};

	/// <summary>
	/// Byte size settings.
	/// </summary>
	public enum ByteSize : byte
	{
		/// <summary>
		/// Five bits per data byte.
		/// </summary>
		Five  = 5,
		/// <summary>
		/// Six bits per data byte.
		/// </summary>
		Six   = 6,
		/// <summary>
		/// Seven bits per data byte.
		/// </summary>
		Seven = 7,
		/// <summary>
		/// Eight bits per data byte.
		/// </summary>
		Eight = 8
	};

	/// <summary>
	/// Stop bit settings
	/// </summary>
	public enum StopBits : byte
	{
		/// <summary>
		/// Line is asserted for 1 bit duration at end of each character
		/// </summary>
		One = 0,
		/// <summary>
		/// Line is asserted for 1.5 bit duration at end of each character
		/// </summary>
		OneFive = 1,
		/// <summary>
		/// Line is asserted for 2 bit duration at end of each character
		/// </summary>
		Two = 2
	};

	/// <summary>
	/// RTS and DTR pin states.
	/// </summary>
	public enum PinState : byte
	{
		/// <summary>
		/// Pin is never asserted.
		/// </summary>
		Disable = 0,
		/// <summary>
		/// Pin is asserted when port is open.
		/// </summary>
		Enable = 1,
		/// <summary>
		/// Pin is asserted when able to receive data.
		/// </summary>
		Handshake = 2,
		/// <summary>
		/// Pin (RTS only) is asserted when transmitting data.
		/// </summary>
		Toggle = 3
	};

	/// <summary>
	/// Standard handshake settings.
	/// </summary>
	public enum Handshake : byte
	{
		/// <summary>
		/// No handshaking
		/// </summary>
		None,
		/// <summary>
		/// Hardware handshaking using CTS(in)/RTS(out) flow control.
		/// </summary>
		CtsRts,
		/// <summary>
		/// Hardware handshaking using DSR(in)/DTR(out) flow control.
		/// </summary>
		DsrDtr,
		/// <summary>
		/// Software handshaking using Xon/Xoff.
		/// </summary>
		XonXoff
	};

	/// <summary>
	/// Extended function codes.
	/// </summary>
	public enum ExtCodes : uint
	{
		/// <summary>
		/// Causes transmission to act as if an XOFF character has been received.
		/// </summary>
		SetXOFF = 1,
		/// <summary>
		/// Causes transmission to act as if an XON character has been received.
		/// </summary>
		SetXON = 2,
		/// <summary>
		/// Sends the RTS (request-to-send) signal.
		/// </summary>
		SetRTS = 3,
		/// <summary>
		/// Clears the RTS (request-to-send) signal.
		/// </summary>
		ClrRTS = 4,
		/// <summary>
		/// Sends the DTR (data-terminal-ready) signal.
		/// </summary>
		SetDTR = 5,
		/// <summary>
		/// Clears the DTR (data-terminal-ready) signal.
		/// </summary>
		ClrDTR = 6,
		/// <summary>
		/// Reset device if possible.
		/// </summary>
		ResetDEV = 7,
		/// <summary>
		/// Suspends character transmission and places the transmission 
		/// line in a break state until the ClearCommBreak function is 
		/// called (or EscapeCommFunction is called with the CLRBREAK 
		/// extended function code). The SETBREAK extended function code 
		/// is identical to the SetCommBreak function. Note that this 
		/// extended function does not flush data that has not been 
		/// transmitted.
		/// </summary>
		SetBreak = 8,
		/// <summary>
		/// Restores character transmission and places the transmission 
		/// line in a nonbreak state. The CLRBREAK extended function code 
		/// is identical to the ClearCommBreak function. 
		/// </summary>
		ClrBreak = 9
	};

	/// <summary>
	/// Byte type with enumeration constants for ASCII control codes.
	/// </summary>
	public enum CtrlChar : byte
	{
	   NULL=0x00,	SOH=0x01,	STX=0x02,	ETX=0x03,	EOT=0x04,	ENQ=0x05,
		ACK=0x06,  BELL=0x07,	 BS=0x08,	 HT=0x09,	 LF=0x0A,	 VT=0x0B,
		 FF=0x0C,	 CR=0x0D,	 SO=0x0E,	 SI=0x0F,	DLE=0x10,	DC1=0x11,	
		DC2=0x12,	DC3=0x13,	DC4=0x14,	NAK=0x15,	SYN=0x16,	ETB=0x17,	
		CAN=0x18,	 EM=0x19,	SUB=0x1A,	ESC=0x1B,	 FS=0x1C,	 GS=0x1D,	 
		RS=0x1E,	 US=0x1F,	 SP=0x20,	DEL=0x7F
	};

}
