using System;
using System.Runtime.InteropServices;

namespace iMobileComPort
{
	/// <summary>
	/// Wrapper class controlling access to the 
	/// kernel32.dll function: EscapeCommFunction().
	/// </summary>
	internal class Win32Escape
	{
		#region Constants
		/*********************************************************************/
		/******************* ESCAPE CONSTANTS - WINBASE.H ********************/
		/*********************************************************************/
		/// <summary>
		/// Causes transmission to act as if an XOFF character has been received.
		/// </summary>
		internal const UInt32 SETXOFF = 1;
		/// <summary>
		/// Causes transmission to act as if an XON character has been received.
		/// </summary>
		internal const UInt32 SETXON = 2;
		/// <summary>
		/// Sends the RTS (request-to-send) signal.
		/// </summary>
		internal const UInt32 SETRTS = 3;
		/// <summary>
		/// Clears the RTS (request-to-send) signal.
		/// </summary>
		internal const UInt32 CLRRTS = 4;
		/// <summary>
		/// Sends the DTR (data-terminal-ready) signal.
		/// </summary>
		internal const UInt32 SETDTR = 5;
		/// <summary>
		/// Clears the DTR (data-terminal-ready) signal.
		/// </summary>
		internal const UInt32 CLRDTR = 6;
		/// <summary>
		/// Reset device if possible.
		/// </summary>
		internal const UInt32 RESETDEV = 7;
		/// <summary>
		/// Suspends character transmission and places the transmission 
		/// line in a break state until the ClearCommBreak function is 
		/// called (or EscapeCommFunction is called with the CLRBREAK 
		/// extended function code). The SETBREAK extended function code 
		/// is identical to the SetCommBreak function. Note that this 
		/// extended function does not flush data that has not been 
		/// transmitted.
		/// </summary>
		internal const UInt32 SETBREAK = 8;
		/// <summary>
		/// Restores character transmission and places the transmission 
		/// line in a nonbreak state. The CLRBREAK extended function code 
		/// is identical to the ClearCommBreak function. 
		/// </summary>
		internal const UInt32 CLRBREAK = 9;
		#endregion

		#region Imports
		/// <summary>
		/// The EscapeCommFunction function directs a specified communications 
		/// device to perform an extended function.
		/// </summary>
		[DllImport("kernel32.dll")]
		private static extern Boolean EscapeCommFunction
			(
			IntPtr hFile, 
			UInt32 dwFunc
			);
		#endregion

		#region Members
		// The function state bitfield controls extended function availability 
		// and maintains the current state. When both bits of any pair are both 
		// set, both functions in the respective pair are unavailable.
		//
		// The bitfield layout is -
		// bit 0 = SETXOFF,  Causes transmission to act as if XOFF has been received.
		// bit 1 = SETXON,   Causes transmission to act as if XON has been received.
		// bit 2 = SETRTS,   Sends the RTS (request-to-send) signal.
		// bit 3 = CLRRTS,   Clear the RTS (request-to-send) signal.
		// bit 4 = SETDTR,   Sends the DTR (data-terminal-ready) signal.
		// bit 5 = CLRDTR,   Clear the DTR (data-terminal-ready) signal.
		// bit 6 = SETBREAK, Place the transmission line in a break state.
		// bit 7 = CLRBREAK, Place the transmission line in a nonbreak state.
		private byte escState;
		/// <summary>
		/// Com device handle.
		/// </summary>
		private IntPtr handle;
		/// <summary>
		/// Class fault string.
		/// </summary>
		private string fault;
		#endregion

		#region Constructor
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="handle">Com device handle.</param>
		internal Win32Escape(IntPtr handle)
		{
			this.handle = handle;
			this.escState = 0x00; // all funcs enabled.
			return;
		}
		#endregion

		#region Methods
		/// <summary>
		/// Directs the comm object to perform an extended function.
		/// </summary>
		/// <param name="code">Escape function code.</param>
		/// <returns>True if successful.</returns>
		internal bool ExtFunc(uint code)
		{
			// Check disabled functions.
			if(
				(((code == SETXOFF)  || (code == SETXON))	&& ((this.escState & 0x03) == 0x03)) ||
				(((code == SETRTS)   || (code == CLRRTS))	&& ((this.escState & 0x0C) == 0x0C)) ||
				(((code == SETDTR)   || (code == CLRDTR))	&& ((this.escState & 0x30) == 0x30)) ||
				(((code == SETBREAK) || (code == CLRBREAK))	&& ((this.escState & 0xC0) == 0xC0))
				)
				return false;

			// Execute the function
			if(EscapeCommFunction(this.handle, code) == false) 
			{
				this.fault = "EscapeCommFunction() Failed. System Returned Error Code: " + 
					Marshal.GetLastWin32Error().ToString();
				return false;
			}

			// Update the state.
			switch(code)
			{
				case(SETXOFF):
					this.escState = (byte) ((this.escState & 0xFC) | 0x01);	break;
				case(SETXON):
					this.escState = (byte) ((this.escState & 0xFC) | 0x02);	break;
				case(SETRTS):
					this.escState = (byte) ((this.escState & 0xF3) | 0x04);	break;
				case(CLRRTS):
					this.escState = (byte) ((this.escState & 0xF3) | 0x08);	break;
				case(SETDTR):
					this.escState = (byte) ((this.escState & 0xCF) | 0x10);	break;
				case(CLRDTR):
					this.escState = (byte) ((this.escState & 0xCF) | 0x20);	break;
				case(SETBREAK):
					this.escState = (byte) ((this.escState & 0x3F) | 0x40);	break;
				case(CLRBREAK):
					this.escState = (byte) ((this.escState & 0x3F) | 0x80);	break;
				default:
					return false;
			}
			return true;
		}
		#endregion

		#region Properties
		/// <summary>
		/// Get/Set the current escape state bitfield.
		/// </summary>
		public byte EscState
		{
			get	{	return this.escState;	}
			set	{	this.escState = value;	}
		}
		/// <summary>
		/// True if XOFF/XON is controllable.
		/// </summary>
		public bool XOFFavailable
		{
			get	{	return ((this.escState & 0x03) != 0x03);	}
			set	{	
					if(value)
						this.escState &= 0xFC;
					else
						this.escState |= 0x03;
				}
		}
		/// <summary>
		/// Set XOFF/XON or Get XOFF/XON state.
		/// </summary>
		public bool XOFF 
		{
			get	{	return ((this.escState & 0x03) == 0x01);	}
			set	{	this.ExtFunc((value) ? SETXOFF : SETXON);	}
		}
		/// <summary>
		/// True if RTS is controllable.
		/// </summary>
		public bool RTSavailable
		{
			get	{	return ((this.escState & 0x0C) != 0x0C);	}
			set	
			{	
					if(value)
						this.escState &= 0xF3;
					else
						this.escState |= 0x0C;
			}
		}
		/// <summary>
		/// Set RTS ON/OFF or Get RTS state.
		/// </summary>
		public bool RTS 
		{
			get	{	return ((this.escState & 0x0C) == 0x04);	}
			set	{	this.ExtFunc((value) ? SETRTS : CLRRTS);	}
		}
		/// <summary>
		/// True if DTR is controllable.
		/// </summary>
		public bool DTRavailable
		{
			get	{	return ((this.escState & 0x30) != 0x30);	}
			set	
			{	
				if(value)
					this.escState &= 0xCF;
				else
					this.escState |= 0x30;
			}
		}
		/// <summary>
		/// Set DTR ON/OFF or Get DTR state.
		/// </summary>
		public bool DTR
		{
			get	{	return ((this.escState & 0x30) == 0x10);	}
			set	{	this.ExtFunc((value) ? SETDTR : CLRDTR);	}
		}
		/// <summary>
		/// True if BREAK is controllable.
		/// </summary>
		public bool BreakAvailable
		{
			get	{	return ((this.escState & 0xC0) != 0xC0);	}
			set	
			{	
				if(value)
					this.escState &= 0x3F;
				else
					this.escState |= 0xC0;
			}
		}
		/// <summary>
		/// Set break ON/OFF or Get break state.
		/// </summary>
		public bool Break
		{
			get	{	return ((this.escState & 0xC0) == 0x40);	}
			set	{	this.ExtFunc((value) ? SETBREAK : CLRBREAK);}
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
