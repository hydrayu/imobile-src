using System;
using System.Runtime.InteropServices;

namespace iMobileComPort
{
	/// <summary>
	/// Wrapper class controlling access to the OVERLAPPED structure and
	/// kernel32.dll function: GetOverlappedResult()
	/// </summary>
	internal class Win32Ovrlap
	{
		#region Structures
		/// <summary>
		/// The OVERLAPPED structure contains information used in asynchronous I/O.
		/// </summary>
		[StructLayout( LayoutKind.Sequential )]
		protected internal struct OVERLAPPED 
		{
			/// <summary>
			/// Reserved for operating system use. 
			/// </summary>
			internal UIntPtr internalLow;
			/// <summary>
			/// Reserved for operating system use.
			/// </summary>
			internal UIntPtr internalHigh;
			/// <summary>
			/// Specifies a file position at which to start the transfer. 
			/// The file position is a byte offset from the start of the file. 
			/// The calling process sets this member before calling the ReadFile 
			/// or WriteFile function. This member is ignored when reading from 
			/// or writing to named pipes and communications devices and should be zero.
			/// </summary>
			internal UInt32 offset;
			/// <summary>
			/// Specifies the high word of the byte offset at which to start the transfer. 
			/// This member is ignored when reading from or writing to named pipes and 
			/// communications devices and should be zero.
			/// </summary>
			internal UInt32 offsetHigh;
			/// <summary>
			/// Handle to an event set to the signaled state when the operation has 
			/// been completed. The calling process must set this member either to 
			/// zero or a valid event handle before calling any overlapped functions. 
			/// To create an event object, use the CreateEvent function. Functions 
			/// such as WriteFile set the event to the nonsignaled state before they 
			/// begin an I/O operation.
			/// </summary>
			internal IntPtr hEvent;
		}
		#endregion

		#region Imports
		/// <summary>
		/// The GetOverlappedResult function retrieves the results 
		/// of an overlapped operation on the specified file, named 
		/// pipe, or communications device.
		/// </summary>
		[DllImport("kernel32.dll", SetLastError=true)]
		private static extern Boolean GetOverlappedResult
			(
			IntPtr hFile, 
			IntPtr lpOverlapped,
			out UInt32 nNumberOfBytesTransferred, 
			Boolean bWait
			);
		#endregion

		#region Members
		private IntPtr handle;
		/// <summary>
		/// Overlap structure.
		/// </summary>
		private OVERLAPPED ol;
		/// <summary>
		/// Pointer to overlap struct in memory.
		/// </summary>
		private IntPtr memPtr;
		private string fault;
		#endregion

		#region Constructor
		internal Win32Ovrlap(IntPtr handle, IntPtr evHandle)
		{
			this.handle = handle;

			// Create, init overlap.
			this.ol = new OVERLAPPED();
			this.ol.offset = 0;
			this.ol.offsetHigh = 0;
			this.ol.hEvent = evHandle;

			// Create memory pointer & copy to unmanaged memory.
			if(evHandle != IntPtr.Zero)
			{
				this.memPtr = Marshal.AllocHGlobal(Marshal.SizeOf(this.ol));
				Marshal.StructureToPtr(this.ol, this.memPtr, false);
			}
			return;
		}
		/// <summary>
		/// Destructor. Free overlap memory.
		/// </summary>
		~Win32Ovrlap()
		{
			if(this.memPtr != IntPtr.Zero)
				Marshal.FreeHGlobal(this.memPtr);
			return;
		}
		#endregion

		#region Methods
		/// <summary>
		/// Updates the class overlap structure (in memory).
		/// </summary>
		/// <returns>True if read update successful.</returns>
		internal bool Get(out uint nSent, bool wait)
		{
			if(GetOverlappedResult(this.handle, this.memPtr, out nSent, wait) == false)
			{
				int error = Marshal.GetLastWin32Error();
				if(error != Win32Com.ERROR_IO_PENDING)
				{
					this.fault = "GetOverlappedResult() Failed. System Returned Error Code: " + 
						error.ToString();
					return false;
				}
			}
			return true;
		}
		#endregion

		#region Properties
		/// <summary>
		/// Get/Set the overlap structure memory pointer.
		/// </summary>
		internal IntPtr MemPtr
		{
			get	{	return this.memPtr;		}
			set	{	this.memPtr = value;	}
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
