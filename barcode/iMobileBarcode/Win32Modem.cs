using System;
using System.Text;
using System.Runtime.InteropServices;

namespace iMobileBarcode
{
	/// <summary>
	/// Wrapper class controlling access to the modem structure and
	/// kernel32.dll function: GetCommModemStatus
	/// </summary>
	internal class Win32Modem
	{
		#region Constants
		/*********************************************************************/
		/******************** MODEM CONSTANTS - WINBASE.H ********************/
		/*********************************************************************/
		/// <summary>
		/// The CTS (clear-to-send) signal is on
		/// </summary>
		internal const UInt32 MS_CTS_ON = 0x0010;
		/// <summary>
		/// The DSR (data-set-ready) signal is on.
		/// </summary>
		internal const UInt32 MS_DSR_ON = 0x0020;
		/// <summary>
		/// The ring indicator signal is on.
		/// </summary>
		internal const UInt32 MS_RING_ON = 0x0040;
		/// <summary>
		/// The RLSD (receive-line-signal-detect) signal is on.
		/// </summary>
		internal const UInt32 MS_RLSD_ON = 0x0080;
		#endregion

		#region Structures
		/// <summary>
		/// Modem signal states (T/F).
		/// </summary>
		protected internal struct MODSIGS
		{
			/// <summary>
			/// The current CTS (clear-to-send) state.
			/// </summary>
			internal bool CTS;
			/// <summary>
			/// The current DSR (data-set-ready) state.
			/// </summary>
			internal bool DSR;
			/// <summary>
			/// The current RLSD (receive-line-signal-detect) state.
			/// </summary>
			internal bool RLSD;
			/// <summary>
			/// The current ring indicator state.
			/// </summary>
			internal bool RING;
		};
		#endregion

		#region Imports
		/// <summary>
		/// Retrieves the modem control-register value.
		/// </summary>
		[DllImport("kernel32.dll")]
		private static extern Boolean GetCommModemStatus
			(
			IntPtr hFile, 
			out UInt32 lpModemStat
			);
		#endregion

		#region Members
		/// <summary>
		/// Modem status register value.
		/// </summary>
		private uint status;
		/// <summary>
		/// Modem signal structure.
		/// </summary>
		private MODSIGS signal;
		/// <summary>
		/// Class fault string.
		/// </summary>
		private string fault;
		#endregion

		#region Constructor
		/// <summary>
		/// Default modem status constructor.
		/// </summary>
		internal Win32Modem()
		{
			this.signal = new MODSIGS();
			return;
		}
		#endregion

		#region Methods
		/// <summary>
		/// Get the modem control register value.
		/// </summary>
		/// <param name="handle">File handle of device.</param>
		/// <returns>True if successful.</returns>
		internal bool Get(IntPtr handle)
		{
			if(GetCommModemStatus(handle, out this.status) == false)
			{
				this.fault = "GetCommModemStatus() Failed. System Returned Error Code: " + 
					Marshal.GetLastWin32Error().ToString();
				return false;
			}
			this.signal.CTS  = ((this.status & MS_CTS_ON)  != 0);
			this.signal.DSR  = ((this.status & MS_DSR_ON)  != 0);
			this.signal.RING = ((this.status & MS_RING_ON) != 0);
			this.signal.RLSD = ((this.status & MS_RLSD_ON) != 0);
			return true;
		}
		#endregion

		#region Properties
		/// <summary>
		/// Get the Clear To Send signal.
		/// </summary>
		internal bool CtsState
		{
			get	{	return this.signal.CTS;		}
		}
		/// <summary>
		/// Get the Data Set Ready signal.
		/// </summary>
		internal bool DsrState
		{
			get	{	return this.signal.DSR;		}
		}
		/// <summary>
		/// Get the Receive Line Status Detection signal.
		/// </summary>
		internal bool RlsdState
		{
			get	{	return this.signal.RLSD;	}
		}
		/// <summary>
		/// Get the Ring Detection signal.
		/// </summary>
		internal bool RingState
		{
			get	{	return this.signal.RING;	}
		}
		/// <summary>
		/// Get the modem status register value.
		/// </summary>
		internal uint Status
		{
			get	{	return this.status;		}
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
