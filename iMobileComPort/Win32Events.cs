using System;
using System.Threading;
using System.Runtime.InteropServices;

namespace iMobileComPort
{
	/// <summary>
	/// Wrapper class controlling access to the kernel32.dll 
	/// functions: SetCommMask(), WaitCommEvent().
	/// </summary>
	internal class Win32Events
	{
		#region Constants
		/*********************************************************************/
		/******************** EVENT CONSTANTS - WINBASE.H ********************/
		/*********************************************************************/
		/// <summary>
		/// A character was received and placed in the input buffer.
		/// </summary>
		internal const UInt32 EV_RXCHAR = 0x0001;
		/// <summary>
		/// The event character was received and placed in the input buffer. 
		/// The event character is specified in the device's DCB structure, 
		/// which is applied to a serial port by using the SetCommState function.
		/// </summary>
		internal const UInt32 EV_RXFLAG = 0x0002;
		/// <summary>
		/// The last character in the output buffer was sent.
		/// </summary>
		internal const UInt32 EV_TXEMPTY = 0x0004;
		/// <summary>
		/// The CTS (clear-to-send) signal changed state.
		/// </summary>
		internal const UInt32 EV_CTS = 0x0008;
		/// <summary>
		/// The DSR (data-set-ready) signal changed state.
		/// </summary>
		internal const UInt32 EV_DSR = 0x0010;
		/// <summary>
		/// The RLSD (receive-line-signal-detect) signal changed state.
		/// </summary>
		internal const UInt32 EV_RLSD = 0x0020;
		/// <summary>
		/// A break was detected on input.
		/// </summary>
		internal const UInt32 EV_BREAK = 0x0040;
		/// <summary>
		/// A line-status error occurred. Line-status errors are 
		/// CE_FRAME, CE_OVERRUN, CE_IOE, CE_TXFULL, CE_RXOVER and CE_RXPARITY.
		/// </summary>
		internal const UInt32 EV_ERR = 0x0080;
		/// <summary>
		/// A ring indicator was detected.
		/// </summary>
		internal const UInt32 EV_RING = 0x0100;
		/// <summary>
		/// Default mask.
		/// </summary>
		internal const UInt32 EV_DEFAULT = EV_RXCHAR | EV_TXEMPTY | EV_CTS | EV_DSR	| 
											EV_BREAK | EV_RLSD | EV_RING | EV_ERR;
		/// <summary>
		/// Modem signal stat mask.
		/// </summary>
		internal const UInt32 EV_MODEM = EV_CTS | EV_DSR | EV_RLSD | EV_RING;
		#endregion

		#region Imports
		/// <summary>
		/// The GetCommMask function retrieves the value of the event mask 
		/// for a specified communications device.
		/// </summary>
		[DllImport("kernel32.dll")]
		private static extern Boolean GetCommMask
			(
			IntPtr hFile, 
			out IntPtr lpEvtMask
			);

		/// <summary>
		/// The SetCommMask function specifies a set of events to be monitored 
		/// for a communications device.
		/// </summary>
		[DllImport("kernel32.dll")]
		private static extern Boolean SetCommMask
			(
			IntPtr hFile, 
			UInt32 dwEvtMask
			);

		/// <summary>
		/// The WaitCommEvent function waits for an event to occur 
		/// for a specified communications device. The set of events 
		/// that are monitored by this function is contained in the 
		/// event mask associated with the device handle. 
		/// </summary>
		[DllImport("kernel32.dll", SetLastError=true)]
		private static extern Boolean WaitCommEvent
			(
			IntPtr hFile, 
			IntPtr lpEvtMask, 
			IntPtr lpOverlapped
			);
		#endregion

		#region Members
		/// <summary>
		/// Event set mask.
		/// </summary>
		private uint setMask;
		/// <summary>
		/// Event get/wait mask.
		/// </summary>
		private uint getMask;
		/// <summary>
		/// Event memory pointer.
		/// </summary>
		private IntPtr evPtr;
		/// <summary>
		/// Overlapped memory pointer.
		/// </summary>
		private IntPtr olPtr;
		/// <summary>
		/// Class fault string.
		/// </summary>
		private string fault;
		#endregion

		#region Constructor
		/// <summary>
		/// Default constructor. Initializes the class members.
		/// </summary>
		/// <param name="ol">Overlap pointer.</param>
		internal Win32Events(IntPtr ol)
		{
			this.olPtr  = ol;
			this.getMask = 0;
			this.setMask = 0;
			this.evPtr = Marshal.AllocHGlobal(Marshal.SizeOf(this.getMask));
			Marshal.WriteInt32(this.evPtr, 0);
			return;
		}
		/// <summary>
		/// Destructor. Free event pointer memory.
		/// </summary>
		~Win32Events()
		{
			Marshal.FreeHGlobal(this.evPtr);
		}
		#endregion

		#region Methods
		/// <summary>
		/// Get the event mask.
		/// </summary>
		/// <param name="handle">Comm port handle.</param>
		/// <returns>True if successful.</returns>
		internal bool Get(IntPtr handle)
		{
			if(GetCommMask(handle, out this.evPtr) == false)
			{
				this.fault = "GetCommMask() Failed. System Returned Error Code: " + 
					Marshal.GetLastWin32Error().ToString();
				return false;
			}
			this.getMask = (uint) Marshal.ReadInt32(this.evPtr);
			return true;
		}
		/// <summary>
		/// Set the event mask.
		/// </summary>
		/// <param name="handle">Comm port handle.</param>
		/// <param name="watchEvents">Watch event mask.</param>
		/// <returns>True if successful.</returns>
		internal bool Set(IntPtr handle, uint watchEvents)
		{
			if(SetCommMask(handle, watchEvents) == false)
			{
				this.fault = "SetCommMask() Failed. System Returned Error Code: " + 
					Marshal.GetLastWin32Error().ToString();
				return false;
			}
			Marshal.WriteInt32(this.evPtr, 0);
			this.setMask = watchEvents;
			return true;
		}
		/// <summary>
		/// Waits for an event to occur on the comm object.
		/// </summary>
		/// <param name="handle">Comm port handle.</param>
		/// <returns>True if successful.</returns>
		internal bool Wait(IntPtr handle, AutoResetEvent signal)
		{
			if(WaitCommEvent(handle, this.evPtr, this.olPtr) == false) 
			{
				int error = Marshal.GetLastWin32Error();
				
				// Operation is executing in the background
				if(error == Win32Com.ERROR_IO_PENDING) 
				{
					signal.WaitOne();
				}
				else
				{
					this.fault = "WaitCommEvent() Failed. System Returned Error Code: " + 
						error.ToString();
					return false;
				}
			}
			this.getMask = (uint) Marshal.ReadInt32(this.evPtr);
			return true;
		}
		#endregion

		#region Properties
		/// <summary>
		/// Get/Set the get/wait event mask;
		/// </summary>
		internal uint GetMask
		{
			get	{	return this.getMask;	}
			set	{	this.getMask = value;	}
		}
		/// <summary>
		/// Get/Set the set event mask;
		/// </summary>
		internal uint SetMask
		{
			get	{	return this.setMask;	}
			set	{	this.setMask = value;	}
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
