using System;
using System.IO;
using System.Text;

namespace iMobileComPort
{
	#region Delegation
	// Delegate class declarations.
	public delegate void VoidFunc();
	public delegate void BoolFunc(bool b);
	public delegate void StrnFunc(string s);
	public delegate void ByteFunc(byte[] b);

	/// <summary>
	/// This structure contains one delegate method for each override event
	/// method in the serial port base class. Each delegate provides a hook
	/// that can be used by any application that wishes to connect to the
	/// asynchronous OnXXX methods.
	/// </summary>
	public struct WithEvents
	{
		public VoidFunc Break;
		public VoidFunc TxDone;
		public StrnFunc Error;
		public ByteFunc RxChar;
		public BoolFunc CtsSig;
		public BoolFunc DsrSig;
		public BoolFunc RlsdSig;
		public BoolFunc RingSig;
	}
	#endregion

	/// <summary>
	/// Serial port interface class. Provides the required inheritance of the base
	/// "SerialComm" abstract class which encapsulates the supporting Win32xxx 
	/// wrapper classes, and provides the override methods (which also must be 
	/// inherited) that handle the base class event methods. The class also
	/// merges with the port configuration class. This class forms the basis 
	/// for using one or more serial ports that can be either instantiated or
	/// inherited as best suits the application.
	/// </summary>
	public class SerialPort : SerialComm
	{
		#region Members
		/// <summary>
		/// The current port index (1-N).
		/// </summary>
		private int index;
		/// <summary>
		/// The port's config file name.
		/// </summary>
		private string file;
		/// <summary>
		/// The port's config file path.
		/// </summary>
		private string path;
		/// <summary>
		/// The port's config file extension.
		/// </summary>
		private string extn;
		/// <summary>
		/// The port's config file name.
		/// </summary>
		private string name;
		/// <summary>
		/// The event method interface hooks.
		/// </summary>
		private WithEvents ev;
		/// <summary>
		/// The port's configuration data.
		/// </summary>
		public SerialCnfg cnfg;
		#endregion

		#region Constructor
		/// <summary>
		/// Default constructor. Create port and setup delegates.
		/// </summary>
		/// <param name="ev">Event handler delegates.</param>
		public SerialPort(WithEvents ev, int idx=1)
		{
            this.ev = ev;
            this.path = "";
            this.file = "Port";
            this.extn = ".cfg";
            this.index = idx;
            this.SetName();
            this.cnfg = new SerialCnfg(this.index, this.name);
            return;
		}
        public SerialPort(WithEvents ev, string port)
        {
            this.ev = ev;
            this.path = "";
            this.file = "Port";
            this.extn = ".cfg";
            this.index = Convert.ToInt32(port.Substring(3, port.Length - 4), 10);
            this.SetName();
            this.cnfg = new SerialCnfg(this.index, this.name);

            return;
        }
        #endregion

		#region Methods
		/// <summary>
		/// Load port settings from file (if it exists), then open.
		/// </summary>
		/// <param name="index">Com port index (1-N).</param>
		public bool Open(int index)
		{
			if(index != this.index)
			{
				this.index = index;
				this.SetName();
				this.cnfg.Load(this.name);
			}
			return this.Create(this.cnfg);
		}

		/// <summary>
		/// Close the port.
		/// </summary>
		public void Close()
		{
			this.Destroy();
			return;
		}

		/// <summary>
		/// Set the config file name based on current member settings.
		/// </summary>
		private void SetName()
		{
            this.name = this.path + this.file + this.index.ToString() + this.extn; 
            return;
		}
		#endregion

		#region Overrides
		/// <summary>
		/// Take action when an error condition occurs.
		/// </summary>
		/// <param name="fault">Fault message.</param>
		protected override void OnError(string fault)
		{
			if(ev.Error != null)
				ev.Error(fault);
			return;
		}

		/// <summary>
		/// Take action when a break condition is detected.
		/// </summary>
		protected override void OnBreak()
		{
			if(ev.Break != null)
				ev.Break();
			return;
		}

		/// <summary>
		/// Take action when TX is complete & queue empty.
		/// </summary>
		protected override void OnTxDone()
		{
			if(ev.TxDone != null)
				ev.TxDone();
			return;
		}

		/// <summary>
		/// Process each received byte immediately.
		/// </summary>
		/// <param name="b">Received byte.</param>
		protected override void OnRxChar(byte[] b)
		{
			if(ev.RxChar != null)
				ev.RxChar(b);
			return;
		}

		/// <summary>
		/// Take action when the CTS modem signal changes.
		/// </summary>
		/// <param name="state">The current signal state.</param>
		protected override void OnCTS(bool state)
		{
			if(ev.CtsSig != null)
				ev.CtsSig(state);
			return;
		}

		/// <summary>
		/// Take action when the DSR modem signal changes.
		/// </summary>
		/// <param name="state">The current signal state.</param>
		protected override void OnDSR(bool state)
		{
			if(ev.DsrSig != null)
				ev.DsrSig(state);
			return;
		}

		/// <summary>
		/// Take action when the RLSD modem signal changes.
		/// </summary>
		/// <param name="state">The current signal state.</param>
		protected override void OnRLSD(bool state)
		{
			if(ev.RlsdSig != null)
				ev.RlsdSig(state);
			return;
		}

		/// <summary>
		/// Take action when the RING modem signal changes.
		/// </summary>
		/// <param name="state">The current signal state.</param>
		protected override void OnRING(bool state)
		{
			if(ev.RingSig != null)
				ev.RingSig(state);
			return;
		}
		#endregion

		#region Properties
		/// <summary>
		/// Get base port reference.
		/// </summary>
		public SerialPort Port
		{
			get	{	return this;		}
		}
		/// <summary>
		/// Get port configuration reference.
		/// </summary>
		public SerialCnfg Cnfg
		{
			get	{	return this.cnfg;	}
		}
		/// <summary>
		/// Get configuration file name.
		/// </summary>
		public string ConfigFileName
		{
			get	{	return this.name;	}
		}
		/// <summary>
		/// Set configuration file path.
		/// </summary>
		public string SetPath
		{
			set
			{
				this.path = value;
				this.SetName();
			}
		}
		/// <summary>
		/// Set configuration file path.
		/// </summary>
		public string SetFile
		{
			set
			{
				this.file = value;
				this.SetName();
			}
		}
		/// <summary>
		/// Set configuration file path.
		/// </summary>
		public string SetExtension
		{
			set
			{
				this.extn = value;
				this.SetName();
			}
		}
		#endregion
	}
}

