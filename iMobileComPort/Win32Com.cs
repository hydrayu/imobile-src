using System;
using System.IO;
using System.Text;
using System.Threading;
using System.Runtime.InteropServices;

namespace iMobileComPort
{
	/// <summary>
	/// Base com port class. Contains methods for the most basic
	/// operations - i.e. opening, closing, reading, writing
	/// </summary>
	internal class Win32Com
	{
		#region Constants

		// Constants taken from WINERROR.H
		internal const UInt32	ERROR_FILE_NOT_FOUND = 2;
		internal const UInt32	ERROR_ACCESS_DENIED = 5;
		internal const UInt32	ERROR_IO_PENDING = 997;

		// Constants taken from WINBASE.H
		internal const Int32	INVALID_HANDLE_VALUE = -1;
		internal const UInt32	FILE_FLAG_OVERLAPPED = 0x40000000;	//dwFlagsAndAttributes
		internal const UInt32	OPEN_EXISTING = 3;					//dwCreationDisposition

		// Constants taken from WINNT.H
		internal const UInt32 GENERIC_READ = 0x80000000;			//dwDesiredAccess
		internal const UInt32 GENERIC_WRITE	= 0x40000000;
		internal const UInt32 READ_WRITE = GENERIC_READ | GENERIC_WRITE;


		/*********************************************************************/
		/******************** PURGE CONSTANTS - WINBASE.H ********************/
		/*********************************************************************/
		/// <summary>
		/// Terminates all outstanding overlapped write operations and returns 
		/// immediately, even if the write operations have not been completed.
		/// </summary>
		internal const UInt32 PURGE_TXABORT = 0x0001;
		/// <summary>
		/// Terminates all outstanding overlapped read operations and returns 
		/// immediately, even if the read operations have not been completed.
		/// </summary>
		internal const UInt32 PURGE_RXABORT = 0x0002;
		/// <summary>
		/// Clears the output buffer (if the device driver has one).
		/// </summary>
		internal const UInt32 PURGE_TXCLEAR = 0x0004;
		/// <summary>
		/// Clears the input buffer (if the device driver has one).
		/// </summary>
		internal const UInt32 PURGE_RXCLEAR = 0x0008;
		#endregion

		#region Imports
		/// <summary>
		/// The CreateFile function creates or opens any of the following 
		/// objects and returns a handle that can be used to access the object: 
		/// Consoles, Communications resources, Directories (open only), 
		/// Disk devices, Files, Mailslots, Pipes 
		/// </summary>
		[DllImport("kernel32.dll", SetLastError=true)]
		private static extern IntPtr CreateFile
			(
			String lpFileName, 
			UInt32 dwDesiredAccess, 
			UInt32 dwShareMode,
			IntPtr lpSecurityAttributes, 
			UInt32 dwCreationDisposition, 
			UInt32 dwFlagsAndAttributes,
			IntPtr hTemplateFile
			);

		/// <summary>
		/// The CloseHandle function closes an open object handle.
		/// </summary>
		[DllImport("kernel32.dll")]
		private static extern Boolean CloseHandle
			(
			IntPtr hObject
			);

		/// <summary>
		/// The ReadFile function reads data from a file, starting at the 
		/// position indicated by the file pointer. After the read operation 
		/// has been completed, the file pointer is adjusted by the number 
		/// of bytes actually read, unless the file handle is created with the 
		/// overlapped attribute. If the file handle is created for overlapped 
		/// input and output (I/O), the application must adjust the position of 
		/// the file pointer after the read operation. 
		/// This function is designed for both synchronous and asynchronous 
		/// operation. The ReadFileEx function is designed solely for asynchronous 
		/// operation. It lets an application perform other processing during a 
		/// file read operation.
		/// </summary>
		[DllImport("kernel32.dll", SetLastError=true)]
		private static extern unsafe Boolean ReadFile
			(
			IntPtr hFile, 
			[Out] Byte[] lpBuffer, 
			UInt32 nNumberOfBytesToRead,
            int* nNumberOfBytesRead,
			IntPtr lpOverlapped
			);

		/// <summary>
		/// The WriteFile function writes data to a file and is designed for both 
		/// synchronous and asynchronous operation. The function starts writing data 
		/// to the file at the position indicated by the file pointer. After the write 
		/// operation has been completed, the file pointer is adjusted by the number of 
		/// bytes actually written, except when the file is opened with FILE_FLAG_OVERLAPPED. 
		/// If the file handle was created for overlapped input and output (I/O), the 
		/// application must adjust the position of the file pointer after the write 
		/// operation is finished. 
		/// This function is designed for both synchronous and asynchronous operation. 
		/// The WriteFileEx function is designed solely for asynchronous operation. 
		/// It lets an application perform other processing during a file write operation.
		/// </summary>
		[DllImport("kernel32.dll", SetLastError=true)]
		private static extern Boolean WriteFile
			(
			IntPtr fFile, 
			Byte[] lpBuffer, 
			UInt32 nNumberOfBytesToWrite,
			out UInt32 lpNumberOfBytesWritten, 
			IntPtr lpOverlapped
			);

		/// <summary>
		/// The CancelIo function cancels all pending input and output 
		/// (I/O) operations that were issued by the calling thread for 
		/// the specified file handle. The function does not cancel I/O 
		/// operations issued for the file handle by other threads. 
		/// </summary>
		[DllImport("kernel32.dll")]
		private static extern Boolean CancelIo
			(
			IntPtr hFile
			);
		
		/// <summary>
		/// The PurgeComm function discards all characters from the output or input 
		/// buffer of a specified communications resource. It can also terminate any 
		/// pending read or write operations on the resource. 
		/// </summary>
		[DllImport("kernel32.dll")]
		private static extern Boolean PurgeComm
			(
			IntPtr hFile,
			uint flags
			);

		/// <summary>
		/// The TransmitCommChar function transmits a specified character ahead of 
		/// any pending data in the output buffer of the specified communications device.
		/// </summary>
		[DllImport("kernel32.dll")]
		private static extern Boolean TransmitCommChar
			(
			IntPtr hFile, 
			Byte cChar
			);
		#endregion

		#region Members
		/// <summary>
		/// Com device handle.
		/// </summary>
		private IntPtr handle;
		/// <summary>
		/// Number of bytes left to send.
		/// </summary>
		private uint nUnsent;
		/// <summary>
		/// Class fault string.
		/// </summary>
		private string fault;
		#endregion

		#region Methods
		/// <summary>
		/// Create and open a comm file object.
		/// </summary>
		/// <param name="portName">Device name - i.e. "COM1:".</param>
		/// <returns>True if executed successfully.</returns>
		internal bool Open(string portName, bool overlapped)
		{
			if(overlapped)
			{
				this.handle = CreateFile
					(
					portName, READ_WRITE, 0, IntPtr.Zero, 
					OPEN_EXISTING, FILE_FLAG_OVERLAPPED, IntPtr.Zero
					);
			}
			else
			{
				this.handle = CreateFile
					(
					portName, READ_WRITE, 0, IntPtr.Zero, 
					OPEN_EXISTING, 0x0, IntPtr.Zero
					);
			}
			if(this.handle == (IntPtr) INVALID_HANDLE_VALUE)
			{
				int error = Marshal.GetLastWin32Error();
				if(error == ERROR_ACCESS_DENIED)
				{
					this.fault = "CreateFile() Failed. Access Denied To " + portName + ".";
					return false;
				}
				if(error == ERROR_FILE_NOT_FOUND)
				{
					this.fault = "CreateFile() Failed. " + portName + " Is Unavailable Or Invalid.";
					return false;
				}
				else
				{
					this.SetFault("CreateFile()", error);
					return false;
				}
			}
			return true;
		}
		/// <summary>
		/// Closes an open object handle.
		/// </summary>
		/// <returns>True if executed successfully.</returns>
		internal bool Close()
		{
			if(CloseHandle(this.handle) == false)
			{
				this.SetFault("CloseHandle()");
				return false;
			}
			return true;
		}
		/// <summary>
		/// Read data from a comm object. 
		/// </summary>
		/// <param name="buf">Buffer into which data is read.</param>
		/// <param name="nToRead">Number of bytes to read.</param>
		/// <param name="nRead">Number of byutes actually read.</param>
		/// <param name="olMem">Pointer to overlap struct.</param>
		/// <returns>True if executed successfully.</returns>
		internal unsafe bool Read(byte[] buf, uint nToRead, out uint nRead, IntPtr olMem)
		{
            int n = 0;
            if (ReadFile(this.handle, buf, nToRead, &n, olMem) == false)
            {
                this.SetFault("[E] Read() in Win32Com");
            }
            nRead = (uint)n;
			return true;
		}
		/// <summary>
		/// Write data to a comm object.
		/// </summary>
		/// <param name="buf">Buffer to write from</param>
		/// <param name="nToSend">Number to buyes to send.</param>
		/// <param name="nSent">Number of actually bytes sent.</param>
		/// <param name="olMem">Pointer to overlap struct.</param>
		/// <returns>True if executed successfully.</returns>
		internal bool Write(byte[] buf, uint nToSend, out uint nSent, Win32Ovrlap ovlap)
		{
			// Write done pre/post check.
			bool postCheck = true;

			// Wait until last write done.
			if(postCheck == false)
			{
				if(ovlap.MemPtr != IntPtr.Zero)
				{
					while(this.nUnsent > 0)
					{
						if(ovlap.Get(out nSent, true))
						{
							this.nUnsent -= nSent;
						}
					}
	
				}
			}

			// Send data buffer.
			bool status = WriteFile(this.handle, buf, nToSend, out nSent, ovlap.MemPtr);
			this.nUnsent = nToSend-nSent;

			// Return status.
			if(status == false) 
			{
				int error = Marshal.GetLastWin32Error();
				
				// Wait until this write done.
				if(postCheck == true)
				{
					if(error == ERROR_IO_PENDING)
					{
						while(this.nUnsent > 0)
						{
							if(ovlap.Get(out nSent, true))
							{
								this.nUnsent -= nSent;
							}
						}
					}
					else
					{
						this.SetFault("WriteFile()", error);
						return false;
					}
				}
				else
				{
					if(error != ERROR_IO_PENDING)
					{
						this.SetFault("WriteFile()", error);
						return false;
					}
					nSent = nToSend; // assume write will pass.
				}
			}
			return true;
		}
		/// <summary>
		/// Transmit the specified character ahead of any pending data in the 
		/// output buffer of the comm object.
		/// </summary>
		/// <param name="chr"></param>
		/// <returns></returns>
		internal bool TxChar(byte chr)
		{
			if(TransmitCommChar(this.handle, chr) == false) 
			{
				this.SetFault("TransmitCommChar()");
				return false;
			}
			return true;
		}
		/// <summary>
		/// Discards all characters from the comm objects I/O buffers.
		/// </summary>
		/// <returns></returns>
		internal bool Flush()
		{
			if(PurgeComm(this.handle, PURGE_TXABORT | 
				PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR) == false) 
			{
				this.SetFault("PurgeComm()");
				return false;
			}
			return true;
		}
		/// <summary>
		/// Cancel all pending I/O operations issued for the comm object.
		/// </summary>
		/// <returns></returns>
		internal bool Cancel()
		{
			if(CancelIo(this.handle) == false) 
			{
				this.SetFault("CancelIo()");
				return false;
			}
			return true;
		}
		/// <summary>
		/// Sets the class fault string.
		/// </summary>
		/// <param name="who">Name of kernel function called.</param>
		private void SetFault(string who)
		{
			this.fault = who + " Failed. System Returned Error Code: " + 
				Marshal.GetLastWin32Error().ToString();
			return;
		}
		/// <summary>
		/// Sets the class fault string.
		/// </summary>
		/// <param name="who">Name of kernel function called.</param>
		/// <param name="err">Last error returned by system.</param>
		private void SetFault(string who, int err)
		{
			this.fault = who + " Failed. System Returned Error Code: " + 
				err.ToString();
			return;
		}
		#endregion

		#region Properties
		/// <summary>
		/// Get the com port file handle.
		/// </summary>
		internal IntPtr Handle
		{
			get	{	return this.handle;		}
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
