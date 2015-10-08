using System;
using System.Runtime.InteropServices;

namespace iMobileBarcode
{
	/// <summary>
	/// Wrapper class controlling access to the COMMTIMEOUTS structure and
	/// kernel32.dll functions: GetCommTimeouts(...), SetCommTimeouts(...)
	/// </summary>
	internal class Win32Tmout
	{
		#region Structures
		/// <summary>
		/// The COMMTIMEOUTS structure is used in the SetCommTimeouts and GetCommTimeouts 
		/// functions to set and query the time-out parameters for a communications device. 
		/// The parameters determine the behavior of ReadFile, WriteFile, ReadFileEx, and 
		/// WriteFileEx operations on the device.
		/// </summary>
		[StructLayout( LayoutKind.Sequential )]
		protected internal struct COMMTIMEOUTS 
		{
			internal UInt32 readIntervalTimeout;
			internal UInt32 readTotalTimeoutMultiplier;
			internal UInt32 readTotalTimeoutConstant;
			internal UInt32 writeTotalTimeoutMultiplier;
			internal UInt32 writeTotalTimeoutConstant;
		}
		#endregion

		#region Imports
		/// <summary>
		/// The GetCommTimeouts function retrieves the time-out parameters for
		/// all read and write operations on a specified communications device.
		/// </summary>
		[DllImport("kernel32.dll")]
		private static extern Boolean GetCommTimeouts
			(
			IntPtr hFile, 
			out COMMTIMEOUTS lpCommTimeouts
			);
		/// <summary>
		/// The SetCommTimeouts function sets the time-out parameters for all read and 
		/// write operations on a specified communications device.
		/// </summary>
		[DllImport("kernel32.dll")]
		internal static extern Boolean SetCommTimeouts
			(
			IntPtr hFile, 
			[In] ref COMMTIMEOUTS lpCommTimeouts
			);
		#endregion

		#region Members
		private string fault;
		private COMMTIMEOUTS ct;
		#endregion

		#region Constructor
		/// <summary>
		/// Timeouts constructor. Creates and initializes the class structure.
		/// </summary>
		internal Win32Tmout()
		{
			this.ct = new COMMTIMEOUTS();
			this.ct.readIntervalTimeout = 1; // wade, UInt32.MaxValue;	// instant return on ReadFile
			this.ct.readTotalTimeoutConstant = 0;
			this.ct.readTotalTimeoutMultiplier = 0;
			this.ct.writeTotalTimeoutMultiplier = 0;
			this.ct.writeTotalTimeoutConstant = 0;
			return;
		}
		/// <summary>
		/// Timeouts constructor. Creates and initializes the class structure.
		/// </summary>
		internal Win32Tmout(uint wttc, uint wttm)
		{
			this.ct = new COMMTIMEOUTS();
			this.ct.readIntervalTimeout = 1; // wade, UInt32.MaxValue;	// instant return on ReadFile
			this.ct.readTotalTimeoutConstant = 0;
			this.ct.readTotalTimeoutMultiplier = 0;
			if(wttm == 0)
			{
				if(System.Environment.OSVersion.Platform == System.PlatformID.Win32NT)
					this.ct.writeTotalTimeoutMultiplier = 0;
				else
					this.ct.writeTotalTimeoutMultiplier = 10000;
			}
			else
			{
				this.ct.writeTotalTimeoutMultiplier = wttm;
			}
			this.ct.writeTotalTimeoutConstant = wttc;
			return;
		}
		#endregion

		#region Methods
		/// <summary>
		/// Update the class timeout structure for this port instance.
		/// </summary>
		/// <param name="handle">Com port handle.</param>
		/// <returns>True if read update successful.</returns>
		internal bool Get(IntPtr handle)
		{
			if(GetCommTimeouts(handle, out this.ct) == false)
			{
				this.SetFault("GetCommTimeouts()");
				return false;
			}
			return true;
		}
		/// <summary>
		/// Update the port timeouts from this instance's current timeout structure.
		/// </summary>
		/// <param name="handle">Com port handle.</param>
		/// <returns>True if write update successful.</returns>
		internal bool Set(IntPtr handle)
		{
			if(SetCommTimeouts(handle, ref this.ct) == false)
			{
				this.SetFault("SetCommTimeouts()");
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
			this.fault = who + "Failed. System Returned Error Code: " + 
				Marshal.GetLastWin32Error().ToString();
			return;
		}
		#endregion

		#region Properties
		/// <summary>
		/// Get/Set the readIntervalTimeout member.
		/// </summary>
		internal uint ReadInterval
		{
			get	{	return this.ct.readIntervalTimeout;		}
			set	{	this.ct.readIntervalTimeout = value;	}
		}
		/// <summary>
		/// Get/Set the readTotalTimeoutConstant member.
		/// </summary>
		internal uint ReadConstant
		{
			get	{	return this.ct.readTotalTimeoutConstant;	}
			set	{	this.ct.readTotalTimeoutConstant = value;	}
		}
		/// <summary>
		/// Get/Set the readTotalTimeoutMultiplier member.
		/// </summary>
		internal uint ReadMultiplier
		{
			get	{	return this.ct.readTotalTimeoutMultiplier;	}
			set	{	this.ct.readTotalTimeoutMultiplier = value;	}
		}
		/// <summary>
		/// Get/Set the writeTotalTimeoutConstant member.
		/// </summary>
		internal uint WriteConstant
		{
			get	{	return this.ct.writeTotalTimeoutConstant;	}
			set	{	this.ct.writeTotalTimeoutConstant = value;	}
		}
		/// <summary>
		/// Get/Set the writeTotalTimeoutMultiplier member.
		/// </summary>
		internal uint WriteMultiplier
		{
			get	{	return this.ct.writeTotalTimeoutMultiplier;	}
			set	{	this.ct.writeTotalTimeoutMultiplier = value;}
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
