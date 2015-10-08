using System;
using System.Text;
using System.Runtime.InteropServices;

namespace iMobileComPort
{
	/// <summary>
	/// Wrapper class controlling access to the COMMPROP structure and
	/// kernel32.dll functions: SetupComm(...), GetCommProperties(...)
	/// </summary>
	internal class Win32Props
	{
		#region Structures
		[StructLayout( LayoutKind.Sequential )]
		protected internal struct COMMPROP
		{
			/// <summary>
			/// Size, in bytes, of the entire data packet, 
			/// regardless of the amount of data requested.
			/// </summary>
			internal UInt16	wPacketLength;
			/// <summary>
			/// Version of the structure. 
			/// </summary>
			internal UInt16	wPacketVersion; 
			/// <summary>
			/// Bitmask indicating which services are 
			/// implemented by this provider. The SP_SERIALCOMM 
			/// value is always specified for communications 
			/// providers, including modem providers.
			/// </summary>
			internal UInt32	dwServiceMask; 
			/// <summary>
			/// Reserved; do not use.
			/// </summary>
			internal UInt32	dwReserved1; 
			/// <summary>
			/// Maximum size, in bytes, of the driver's protected internal 
			/// output buffer. A value of zero indicates that no 
			/// maximum value is imposed by the serial provider.
			/// </summary>
			internal UInt32	dwMaxTxQueue; 
			/// <summary>
			/// Maximum size, in bytes, of the driver's protected internal 
			/// input buffer. A value of zero indicates that no 
			/// maximum value is imposed by the serial provider.
			/// </summary>
			internal UInt32	dwMaxRxQueue; 
			/// <summary>
			/// Maximum allowable baud rate, in bits per second.
			/// </summary>
			internal UInt32	dwMaxBaud; 
			/// <summary>
			/// Communications-provider type.
			/// </summary>
			internal UInt32	dwProvSubType; 
			/// <summary>
			/// Bitmask indicating the capabilities offered by the provider. 
			/// </summary>
			internal UInt32	dwProvCapabilities; 
			/// <summary>
			/// Bitmask indicating the communications parameter that can be changed. 
			/// </summary>
			internal UInt32	dwSettableParams; 
			/// <summary>
			/// Bitmask indicating the baud rates that can be used. 
			/// </summary>
			internal UInt32	dwSettableBaud; 
			/// <summary>
			/// Bitmask indicating the number of data bits that can be set. 
			/// </summary>
			internal UInt16 wSettableData; 
			/// <summary>
			/// Bitmask indicating the stop bit and parity 
			/// settings that can be selected.  
			/// </summary>
			internal UInt16 wSettableStopParity; 
			/// <summary>
			/// Size, in bytes, of the driver's protected internal output buffer. 
			/// A value of zero indicates that the value is unavailable.
			/// </summary>
			internal UInt32	dwCurrentTxQueue; 
			/// <summary>
			/// Size, in bytes, of the driver's protected internal input buffer. 
			/// A value of zero indicates that the value is unavailable.
			/// </summary>
			internal UInt32	dwCurrentRxQueue; 
			/// <summary>
			/// Provider-specific data. Applications should ignore this 
			/// member unless they have detailed information about the 
			/// format of the data required by the provider. Set this 
			/// member to COMMPROP_INITIALIZED before calling the 
			/// GetCommProperties function to indicate that the 
			/// wPacketLength member is already valid.
			/// </summary>
			internal UInt32	dwProvSpec1; 
			/// <summary>
			/// Provider-specific data. Applications should ignore this 
			/// member unless they have detailed information about the 
			/// format of the data required by the provider.
			/// </summary>
			internal UInt32	dwProvSpec2; 
			/// <summary>
			/// Provider-specific data. Applications should ignore this 
			/// member unless they have detailed information about the 
			/// format of the data required by the provider.
			/// </summary>
			internal Byte	wcProvChar; 
		}
		#endregion

		#region Imports
		/// <summary>
		/// The SetupComm function initializes the communications parameters for a 
		/// specified communications device.
		/// </summary>
		[DllImport("kernel32.dll")]
		internal static extern Boolean SetupComm
			(
			IntPtr hFile, 
			UInt32 dwInQueue, 
			UInt32 dwOutQueue
			);

		/// <summary>
		/// The GetCommProperties function retrieves information about 
		/// the communications properties for a specified communications device. 
		/// </summary>
		[DllImport("kernel32.dll")]
		private static extern Boolean GetCommProperties
			(
			IntPtr hFile, 
			out COMMPROP cp
			);
		#endregion

		#region Members
		private string fault;
		private COMMPROP cp;
		#endregion

		#region Constructor
		/// <summary>
		/// Default constructor. Creates the comm properties structure.
		/// </summary>
		internal Win32Props()
		{
			this.cp = new COMMPROP();
			return;
		}
		#endregion

		#region Methods
		/// <summary>
		/// Get the comm properties structure.
		/// </summary>
		/// <param name="handle">Com port handle.</param>
		/// <returns>True if successful.</returns>
		internal bool Get(IntPtr handle)
		{
			if(GetCommProperties(handle, out this.cp) == false)
			{
				this.SetFault("GetCommProperties()");
				return false;
			}
			return true;
		}
		/// <summary>
		/// Init the TX/RX queues.
		/// </summary>
		/// <param name="handle">Com port handle.</param>
		/// <param name="rxSize">RX queue size.</param>
		/// <param name="rxSize">TX queue size.</param>
		/// <returns>True if successful.</returns>
		internal bool Set(IntPtr handle, uint rxSize, uint txSize)
		{
			if(SetupComm(handle, rxSize, txSize) == false)
			{
				this.SetFault("SetupComm()");
				return false;
			}
			return true;
		}
		/// <summary>
		/// Returns the TX/RX queue sizes and bytes left.
		/// </summary>
		/// <returns>Queue size string.</returns>
		internal string QueueString()
		{
			StringBuilder s = new StringBuilder("The RX max/cur queue sizes are: ", 80);
			if(this.cp.dwMaxRxQueue == 0)
			{
				s.Append("variable/");
			}
			else
			{
				s.Append(this.cp.dwMaxRxQueue.ToString());
			}
			if(this.cp.dwCurrentRxQueue == 0)
			{
				s.Append("unavailable.");
			}
			else
			{
				s.Append(this.cp.dwCurrentRxQueue.ToString());
			}
			s.Append("\r\n");

			s.Append("The TX max/cur queue sizes are: ");
			if(this.cp.dwMaxTxQueue == 0)
			{
				s.Append("variable/");
			}
			else
			{
				s.Append(this.cp.dwMaxTxQueue.ToString());
			}
			if(this.cp.dwCurrentTxQueue == 0)
			{
				s.Append("unavailable.");
			}
			else
			{
				s.Append(this.cp.dwCurrentTxQueue.ToString());
			}
			s.Append("\r\n");
			return s.ToString();
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
		/// Get the current size of the RX queue.
		/// </summary>
		protected internal uint RxCurSize
		{
			get	{	return this.cp.dwCurrentRxQueue;	}
		}
		/// <summary>
		/// Get the max size of the RX queue.
		/// </summary>
		protected internal uint RxMaxSize
		{
			get	{	return this.cp.dwMaxRxQueue;		}
		}
		/// <summary>
		/// Get the current size of the TX queue.
		/// </summary>
		protected internal uint TxCurSize
		{
			get	{	return this.cp.dwCurrentTxQueue;	}
		}
		/// <summary>
		/// Get the max size of the TX queue.
		/// </summary>
		protected internal uint TxMaxSize
		{
			get	{	return this.cp.dwMaxTxQueue;		}
		}
		/// <summary>
		/// Get the current comm properties structure.
		/// </summary>
		protected internal COMMPROP CommProperties
		{
			get	{	return this.cp;		}
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
