using System;
using System.IO;
using System.IO.Ports;
using System.Text;
using System.Drawing;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms;
using System.Threading;
using System.Runtime.InteropServices;
using System.Management;

namespace iMobileBarcode
{
	/// <summary>
	/// Summary description for Terminal.
	/// </summary>
	public class Terminal : System.Windows.Forms.Form
	{
		private System.Windows.Forms.ComboBox ComPort;
		private System.Windows.Forms.Label LblModem;
		private System.Windows.Forms.Label LblRING;
		private System.Windows.Forms.Label LblRLSD;
		private System.Windows.Forms.Label LblDSR;
		private System.Windows.Forms.Label LblCTS;
		private System.Windows.Forms.Label LblStatus;
		private System.Windows.Forms.Label LblPort;
		private System.Windows.Forms.Label LblTxData;
		private System.Windows.Forms.Label LblRxData;
		private System.Windows.Forms.Label LblXmit;
		private System.Windows.Forms.Label LblSettings;
		private System.Windows.Forms.Label LblTxCnt;
		private System.Windows.Forms.Label LblRxCnt;
		private System.Windows.Forms.Label LblReadRate;
		private System.Windows.Forms.Label LblMsec1;
		private System.Windows.Forms.Label LblHandshake;
		private System.Windows.Forms.Label LblForces;

		private System.Windows.Forms.Panel PnlCom;
		private System.Windows.Forms.Panel PnlHandshake;
		private System.Windows.Forms.Panel PnlModem;
		private System.Windows.Forms.Panel PnlTxData;
		private System.Windows.Forms.Panel PnlRxData;
		private System.Windows.Forms.Panel PnlRING;
		private System.Windows.Forms.Panel PnlRLSD;
		private System.Windows.Forms.Panel PnlDSR;
		private System.Windows.Forms.Panel PnlCTS;

		private System.Windows.Forms.Button CmdOpen;
		private System.Windows.Forms.Button CmdQuit;
		private System.Windows.Forms.Button CmdConfig;
		private System.Windows.Forms.Button CmdCont;
		private System.Windows.Forms.Button CmdSend;
		private System.Windows.Forms.Button CmdClear;
		private System.Windows.Forms.Button CmdBreak;
		private System.Windows.Forms.Button CmdRts;
		private System.Windows.Forms.Button CmdDtr;
        private System.Windows.Forms.Button CmdXoff;
		private System.Windows.Forms.CheckBox ChkUpdate;

		private System.Windows.Forms.Timer RecvTimer;

		internal System.Windows.Forms.TextBox TxStr;
		internal System.Windows.Forms.TextBox TxData;
		internal System.Windows.Forms.TextBox RxData;
		internal System.Windows.Forms.TextBox TxCount;
		internal System.Windows.Forms.TextBox RxCount;
		internal System.Windows.Forms.TextBox Settings;
		internal System.Windows.Forms.TextBox Status;
		internal System.Windows.Forms.TextBox RecvRate;
		internal System.Windows.Forms.TextBox HndShake;

		private System.ComponentModel.IContainer components;

		#region Members
		// Terminal app constants.
		private const int	MAX_PORTS  = 32;
		private const int	RANDOM_LO  = 32;
		private const int	RANDOM_HI  = 127;
		private const int	RANDOM_CNT = 80;

		// Terminal app locals.
		private int			TxBytes = 0;
		private int			RxBytes = 0;
		private string		TxString = "";
		private string		RxString = "";
		private Thread		TxThread;
		private bool		TxThreadStop;
        private byte CodeType = 0;
        private string Code = "";
        private bool decodeEv = false;

		// Terminal interface port.
		private SerialPort	Port;
        private string dev = "MOTO";

		// Terminal functions that 
		// handle base class events.
		private WithEvents	Func;
		#endregion

		#region Constructor
		/// <summary>
		/// Terminal constructor. Initialization.
		/// </summary>
		public Terminal()
		{
			InitializeComponent();
            
			TxBytes = 0;
			this.TxString = "";
			this.TxCount.Text = "0";
			this.TxData.Text = "";
			this.TxStr.Text = "";

			RxBytes = 0;
			this.RxString = "";
			this.RxCount.Text = "0";
			this.RxData.Text = "";

			this.Status.Text = "";
			this.Settings.Text = "";
			this.HndShake.Text = "";
			this.RecvRate.Text = this.RecvTimer.Interval.ToString();
            
			// Fill com port list.
            this.FillAvailable();

			// Instantiate base class event handlers.
			this.Func = new WithEvents();
			this.Func.Error   = new StrnFunc(this.OnError);
			this.Func.RxChar  = new ByteFunc(this.OnRecvI);
			this.Func.CtsSig  = new BoolFunc(this.OnCts);
			this.Func.DsrSig  = new BoolFunc(this.OnDsr);
			this.Func.RlsdSig = new BoolFunc(this.OnRlsd);
			this.Func.RingSig = new BoolFunc(this.OnRing);

			// Instantiate the terminal port.
            // wade
			this.Port = new SerialPort(this.Func, true); // load iMobileBarcode.cfg
            connectMOTO();
            if (this.Port.cnfg.startup == 1)
            {
                this.WindowState = FormWindowState.Minimized;
                this.Show();
            }
            else if (this.Port.cnfg.startup == 2) this.Hide();
            else
                this.Show();
            return;
		}
		#endregion

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.components = new System.ComponentModel.Container();
            this.PnlCom = new System.Windows.Forms.Panel();
            this.HndShake = new System.Windows.Forms.TextBox();
            this.LblHandshake = new System.Windows.Forms.Label();
            this.ComPort = new System.Windows.Forms.ComboBox();
            this.Settings = new System.Windows.Forms.TextBox();
            this.LblSettings = new System.Windows.Forms.Label();
            this.LblPort = new System.Windows.Forms.Label();
            this.Status = new System.Windows.Forms.TextBox();
            this.LblStatus = new System.Windows.Forms.Label();
            this.PnlHandshake = new System.Windows.Forms.Panel();
            this.CmdXoff = new System.Windows.Forms.Button();
            this.CmdDtr = new System.Windows.Forms.Button();
            this.CmdRts = new System.Windows.Forms.Button();
            this.LblForces = new System.Windows.Forms.Label();
            this.CmdBreak = new System.Windows.Forms.Button();
            this.PnlModem = new System.Windows.Forms.Panel();
            this.PnlRING = new System.Windows.Forms.Panel();
            this.PnlRLSD = new System.Windows.Forms.Panel();
            this.PnlDSR = new System.Windows.Forms.Panel();
            this.LblRING = new System.Windows.Forms.Label();
            this.LblRLSD = new System.Windows.Forms.Label();
            this.LblDSR = new System.Windows.Forms.Label();
            this.LblCTS = new System.Windows.Forms.Label();
            this.PnlCTS = new System.Windows.Forms.Panel();
            this.LblModem = new System.Windows.Forms.Label();
            this.PnlTxData = new System.Windows.Forms.Panel();
            this.TxData = new System.Windows.Forms.TextBox();
            this.ChkUpdate = new System.Windows.Forms.CheckBox();
            this.TxCount = new System.Windows.Forms.TextBox();
            this.LblTxCnt = new System.Windows.Forms.Label();
            this.LblXmit = new System.Windows.Forms.Label();
            this.TxStr = new System.Windows.Forms.TextBox();
            this.LblTxData = new System.Windows.Forms.Label();
            this.PnlRxData = new System.Windows.Forms.Panel();
            this.LblMsec1 = new System.Windows.Forms.Label();
            this.RecvRate = new System.Windows.Forms.TextBox();
            this.LblReadRate = new System.Windows.Forms.Label();
            this.RxCount = new System.Windows.Forms.TextBox();
            this.LblRxCnt = new System.Windows.Forms.Label();
            this.RxData = new System.Windows.Forms.TextBox();
            this.LblRxData = new System.Windows.Forms.Label();
            this.CmdOpen = new System.Windows.Forms.Button();
            this.CmdQuit = new System.Windows.Forms.Button();
            this.CmdConfig = new System.Windows.Forms.Button();
            this.CmdCont = new System.Windows.Forms.Button();
            this.CmdClear = new System.Windows.Forms.Button();
            this.CmdSend = new System.Windows.Forms.Button();
            this.RecvTimer = new System.Windows.Forms.Timer(this.components);
            this.PnlCom.SuspendLayout();
            this.PnlHandshake.SuspendLayout();
            this.PnlModem.SuspendLayout();
            this.PnlTxData.SuspendLayout();
            this.PnlRxData.SuspendLayout();
            this.SuspendLayout();
            // 
            // PnlCom
            // 
            this.PnlCom.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.PnlCom.Controls.Add(this.HndShake);
            this.PnlCom.Controls.Add(this.LblHandshake);
            this.PnlCom.Controls.Add(this.ComPort);
            this.PnlCom.Controls.Add(this.Settings);
            this.PnlCom.Controls.Add(this.LblSettings);
            this.PnlCom.Controls.Add(this.LblPort);
            this.PnlCom.Location = new System.Drawing.Point(7, 8);
            this.PnlCom.Name = "PnlCom";
            this.PnlCom.Size = new System.Drawing.Size(360, 64);
            this.PnlCom.TabIndex = 0;
            // 
            // HndShake
            // 
            this.HndShake.Font = new System.Drawing.Font("Courier New", 8.861538F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.HndShake.Location = new System.Drawing.Point(100, 32);
            this.HndShake.Name = "HndShake";
            this.HndShake.Size = new System.Drawing.Size(107, 21);
            this.HndShake.TabIndex = 54;
            this.HndShake.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // LblHandshake
            // 
            this.LblHandshake.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.861538F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LblHandshake.ForeColor = System.Drawing.Color.Teal;
            this.LblHandshake.Location = new System.Drawing.Point(100, 0);
            this.LblHandshake.Name = "LblHandshake";
            this.LblHandshake.Size = new System.Drawing.Size(107, 24);
            this.LblHandshake.TabIndex = 53;
            this.LblHandshake.Text = "Handshake";
            this.LblHandshake.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // ComPort
            // 
            this.ComPort.Font = new System.Drawing.Font("Courier New", 8.861538F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ComPort.Location = new System.Drawing.Point(7, 32);
            this.ComPort.MaxDropDownItems = 16;
            this.ComPort.Name = "ComPort";
            this.ComPort.Size = new System.Drawing.Size(80, 23);
            this.ComPort.TabIndex = 51;
            // 
            // Settings
            // 
            this.Settings.Font = new System.Drawing.Font("Courier New", 8.861538F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Settings.Location = new System.Drawing.Point(220, 32);
            this.Settings.Name = "Settings";
            this.Settings.Size = new System.Drawing.Size(133, 21);
            this.Settings.TabIndex = 41;
            this.Settings.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // LblSettings
            // 
            this.LblSettings.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.861538F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LblSettings.ForeColor = System.Drawing.Color.Teal;
            this.LblSettings.Location = new System.Drawing.Point(220, 0);
            this.LblSettings.Name = "LblSettings";
            this.LblSettings.Size = new System.Drawing.Size(133, 24);
            this.LblSettings.TabIndex = 40;
            this.LblSettings.Text = "Settings";
            this.LblSettings.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // LblPort
            // 
            this.LblPort.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.861538F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LblPort.ForeColor = System.Drawing.Color.Teal;
            this.LblPort.Location = new System.Drawing.Point(7, 0);
            this.LblPort.Name = "LblPort";
            this.LblPort.Size = new System.Drawing.Size(80, 24);
            this.LblPort.TabIndex = 39;
            this.LblPort.Text = "Active Port";
            this.LblPort.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // Status
            // 
            this.Status.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.753846F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Status.Location = new System.Drawing.Point(53, 472);
            this.Status.Name = "Status";
            this.Status.Size = new System.Drawing.Size(787, 19);
            this.Status.TabIndex = 11;
            // 
            // LblStatus
            // 
            this.LblStatus.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.753846F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LblStatus.Location = new System.Drawing.Point(7, 472);
            this.LblStatus.Name = "LblStatus";
            this.LblStatus.Size = new System.Drawing.Size(46, 24);
            this.LblStatus.TabIndex = 12;
            this.LblStatus.Text = "Status:";
            this.LblStatus.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // PnlHandshake
            // 
            this.PnlHandshake.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.PnlHandshake.Controls.Add(this.CmdXoff);
            this.PnlHandshake.Controls.Add(this.CmdDtr);
            this.PnlHandshake.Controls.Add(this.CmdRts);
            this.PnlHandshake.Controls.Add(this.LblForces);
            this.PnlHandshake.Controls.Add(this.CmdBreak);
            this.PnlHandshake.Location = new System.Drawing.Point(373, 8);
            this.PnlHandshake.Name = "PnlHandshake";
            this.PnlHandshake.Size = new System.Drawing.Size(287, 64);
            this.PnlHandshake.TabIndex = 34;
            // 
            // CmdXoff
            // 
            this.CmdXoff.Location = new System.Drawing.Point(7, 32);
            this.CmdXoff.Name = "CmdXoff";
            this.CmdXoff.Size = new System.Drawing.Size(66, 24);
            this.CmdXoff.TabIndex = 67;
            this.CmdXoff.Text = "Set XOFF";
            this.CmdXoff.Click += new System.EventHandler(this.CmdXoff_Click);
            // 
            // CmdDtr
            // 
            this.CmdDtr.Location = new System.Drawing.Point(147, 32);
            this.CmdDtr.Name = "CmdDtr";
            this.CmdDtr.Size = new System.Drawing.Size(66, 24);
            this.CmdDtr.TabIndex = 66;
            this.CmdDtr.Text = "Set DTR";
            this.CmdDtr.Click += new System.EventHandler(this.CmdDtr_Click);
            // 
            // CmdRts
            // 
            this.CmdRts.Location = new System.Drawing.Point(73, 32);
            this.CmdRts.Name = "CmdRts";
            this.CmdRts.Size = new System.Drawing.Size(67, 24);
            this.CmdRts.TabIndex = 65;
            this.CmdRts.Text = "Set RTS";
            this.CmdRts.Click += new System.EventHandler(this.CmdRts_Click);
            // 
            // LblForces
            // 
            this.LblForces.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.861538F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LblForces.ForeColor = System.Drawing.Color.Teal;
            this.LblForces.Location = new System.Drawing.Point(13, 0);
            this.LblForces.Name = "LblForces";
            this.LblForces.Size = new System.Drawing.Size(267, 24);
            this.LblForces.TabIndex = 41;
            this.LblForces.Text = "Force Signals";
            this.LblForces.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // CmdBreak
            // 
            this.CmdBreak.Location = new System.Drawing.Point(213, 32);
            this.CmdBreak.Name = "CmdBreak";
            this.CmdBreak.Size = new System.Drawing.Size(67, 24);
            this.CmdBreak.TabIndex = 64;
            this.CmdBreak.Text = "Set BRK";
            this.CmdBreak.Click += new System.EventHandler(this.CmdBreak_Click);
            // 
            // PnlModem
            // 
            this.PnlModem.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.PnlModem.Controls.Add(this.PnlRING);
            this.PnlModem.Controls.Add(this.PnlRLSD);
            this.PnlModem.Controls.Add(this.PnlDSR);
            this.PnlModem.Controls.Add(this.LblRING);
            this.PnlModem.Controls.Add(this.LblRLSD);
            this.PnlModem.Controls.Add(this.LblDSR);
            this.PnlModem.Controls.Add(this.LblCTS);
            this.PnlModem.Controls.Add(this.PnlCTS);
            this.PnlModem.Controls.Add(this.LblModem);
            this.PnlModem.Location = new System.Drawing.Point(667, 8);
            this.PnlModem.Name = "PnlModem";
            this.PnlModem.Size = new System.Drawing.Size(173, 64);
            this.PnlModem.TabIndex = 35;
            // 
            // PnlRING
            // 
            this.PnlRING.BackColor = System.Drawing.Color.Red;
            this.PnlRING.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.PnlRING.Enabled = false;
            this.PnlRING.Location = new System.Drawing.Point(133, 40);
            this.PnlRING.Name = "PnlRING";
            this.PnlRING.Size = new System.Drawing.Size(27, 16);
            this.PnlRING.TabIndex = 47;
            // 
            // PnlRLSD
            // 
            this.PnlRLSD.BackColor = System.Drawing.Color.Red;
            this.PnlRLSD.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.PnlRLSD.Enabled = false;
            this.PnlRLSD.Location = new System.Drawing.Point(93, 40);
            this.PnlRLSD.Name = "PnlRLSD";
            this.PnlRLSD.Size = new System.Drawing.Size(27, 16);
            this.PnlRLSD.TabIndex = 46;
            // 
            // PnlDSR
            // 
            this.PnlDSR.BackColor = System.Drawing.Color.Red;
            this.PnlDSR.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.PnlDSR.Enabled = false;
            this.PnlDSR.Location = new System.Drawing.Point(53, 40);
            this.PnlDSR.Name = "PnlDSR";
            this.PnlDSR.Size = new System.Drawing.Size(27, 16);
            this.PnlDSR.TabIndex = 45;
            // 
            // LblRING
            // 
            this.LblRING.Font = new System.Drawing.Font("Courier New", 8.861538F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LblRING.Location = new System.Drawing.Point(127, 24);
            this.LblRING.Name = "LblRING";
            this.LblRING.Size = new System.Drawing.Size(40, 16);
            this.LblRING.TabIndex = 44;
            this.LblRING.Text = "RNG";
            this.LblRING.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // LblRLSD
            // 
            this.LblRLSD.Font = new System.Drawing.Font("Courier New", 8.861538F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LblRLSD.Location = new System.Drawing.Point(87, 24);
            this.LblRLSD.Name = "LblRLSD";
            this.LblRLSD.Size = new System.Drawing.Size(40, 16);
            this.LblRLSD.TabIndex = 43;
            this.LblRLSD.Text = "RLSD";
            this.LblRLSD.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // LblDSR
            // 
            this.LblDSR.Font = new System.Drawing.Font("Courier New", 8.861538F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LblDSR.Location = new System.Drawing.Point(47, 24);
            this.LblDSR.Name = "LblDSR";
            this.LblDSR.Size = new System.Drawing.Size(40, 16);
            this.LblDSR.TabIndex = 42;
            this.LblDSR.Text = "DSR";
            this.LblDSR.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // LblCTS
            // 
            this.LblCTS.Font = new System.Drawing.Font("Courier New", 8.861538F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LblCTS.Location = new System.Drawing.Point(7, 24);
            this.LblCTS.Name = "LblCTS";
            this.LblCTS.Size = new System.Drawing.Size(40, 16);
            this.LblCTS.TabIndex = 41;
            this.LblCTS.Text = "CTS";
            this.LblCTS.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // PnlCTS
            // 
            this.PnlCTS.BackColor = System.Drawing.Color.Red;
            this.PnlCTS.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.PnlCTS.Enabled = false;
            this.PnlCTS.Location = new System.Drawing.Point(13, 40);
            this.PnlCTS.Name = "PnlCTS";
            this.PnlCTS.Size = new System.Drawing.Size(27, 16);
            this.PnlCTS.TabIndex = 40;
            // 
            // LblModem
            // 
            this.LblModem.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.861538F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LblModem.ForeColor = System.Drawing.Color.Teal;
            this.LblModem.Location = new System.Drawing.Point(7, 0);
            this.LblModem.Name = "LblModem";
            this.LblModem.Size = new System.Drawing.Size(160, 24);
            this.LblModem.TabIndex = 39;
            this.LblModem.Text = "Modem Signals";
            this.LblModem.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // PnlTxData
            // 
            this.PnlTxData.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.PnlTxData.Controls.Add(this.TxData);
            this.PnlTxData.Controls.Add(this.ChkUpdate);
            this.PnlTxData.Controls.Add(this.TxCount);
            this.PnlTxData.Controls.Add(this.LblTxCnt);
            this.PnlTxData.Controls.Add(this.LblXmit);
            this.PnlTxData.Controls.Add(this.TxStr);
            this.PnlTxData.Controls.Add(this.LblTxData);
            this.PnlTxData.Location = new System.Drawing.Point(7, 256);
            this.PnlTxData.Name = "PnlTxData";
            this.PnlTxData.Size = new System.Drawing.Size(833, 168);
            this.PnlTxData.TabIndex = 38;
            // 
            // TxData
            // 
            this.TxData.Font = new System.Drawing.Font("Courier New", 8.861538F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TxData.Location = new System.Drawing.Point(7, 24);
            this.TxData.Multiline = true;
            this.TxData.Name = "TxData";
            this.TxData.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.TxData.Size = new System.Drawing.Size(820, 114);
            this.TxData.TabIndex = 39;
            // 
            // ChkUpdate
            // 
            this.ChkUpdate.Location = new System.Drawing.Point(620, 140);
            this.ChkUpdate.Name = "ChkUpdate";
            this.ChkUpdate.Size = new System.Drawing.Size(100, 24);
            this.ChkUpdate.TabIndex = 64;
            this.ChkUpdate.Text = "Pause Updates";
            this.ChkUpdate.CheckedChanged += new System.EventHandler(this.ChkUpdate_CheckedChanged);
            // 
            // TxCount
            // 
            this.TxCount.Font = new System.Drawing.Font("Courier New", 8.861538F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TxCount.Location = new System.Drawing.Point(73, 140);
            this.TxCount.Name = "TxCount";
            this.TxCount.Size = new System.Drawing.Size(67, 21);
            this.TxCount.TabIndex = 60;
            this.TxCount.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // LblTxCnt
            // 
            this.LblTxCnt.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.753846F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LblTxCnt.Location = new System.Drawing.Point(7, 140);
            this.LblTxCnt.Name = "LblTxCnt";
            this.LblTxCnt.Size = new System.Drawing.Size(60, 24);
            this.LblTxCnt.TabIndex = 59;
            this.LblTxCnt.Text = "TX Count:";
            this.LblTxCnt.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // LblXmit
            // 
            this.LblXmit.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.753846F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LblXmit.Location = new System.Drawing.Point(153, 140);
            this.LblXmit.Name = "LblXmit";
            this.LblXmit.Size = new System.Drawing.Size(60, 24);
            this.LblXmit.TabIndex = 41;
            this.LblXmit.Text = "Transmit:";
            this.LblXmit.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // TxStr
            // 
            this.TxStr.Font = new System.Drawing.Font("Courier New", 8.861538F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TxStr.Location = new System.Drawing.Point(213, 140);
            this.TxStr.Name = "TxStr";
            this.TxStr.Size = new System.Drawing.Size(400, 21);
            this.TxStr.TabIndex = 40;
            // 
            // LblTxData
            // 
            this.LblTxData.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.861538F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LblTxData.ForeColor = System.Drawing.Color.Teal;
            this.LblTxData.Location = new System.Drawing.Point(13, 0);
            this.LblTxData.Name = "LblTxData";
            this.LblTxData.Size = new System.Drawing.Size(814, 24);
            this.LblTxData.TabIndex = 38;
            this.LblTxData.Text = "Transmitted Data";
            this.LblTxData.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // PnlRxData
            // 
            this.PnlRxData.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.PnlRxData.Controls.Add(this.LblMsec1);
            this.PnlRxData.Controls.Add(this.RecvRate);
            this.PnlRxData.Controls.Add(this.LblReadRate);
            this.PnlRxData.Controls.Add(this.RxCount);
            this.PnlRxData.Controls.Add(this.LblRxCnt);
            this.PnlRxData.Controls.Add(this.RxData);
            this.PnlRxData.Controls.Add(this.LblRxData);
            this.PnlRxData.Location = new System.Drawing.Point(7, 72);
            this.PnlRxData.Name = "PnlRxData";
            this.PnlRxData.Size = new System.Drawing.Size(833, 186);
            this.PnlRxData.TabIndex = 40;
            // 
            // LblMsec1
            // 
            this.LblMsec1.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.753846F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LblMsec1.Location = new System.Drawing.Point(280, 156);
            this.LblMsec1.Name = "LblMsec1";
            this.LblMsec1.Size = new System.Drawing.Size(40, 24);
            this.LblMsec1.TabIndex = 51;
            this.LblMsec1.Text = "msec";
            this.LblMsec1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // RecvRate
            // 
            this.RecvRate.Font = new System.Drawing.Font("Courier New", 8.861538F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.RecvRate.Location = new System.Drawing.Point(213, 156);
            this.RecvRate.Name = "RecvRate";
            this.RecvRate.Size = new System.Drawing.Size(67, 21);
            this.RecvRate.TabIndex = 50;
            this.RecvRate.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.RecvRate.KeyUp += new System.Windows.Forms.KeyEventHandler(this.RecvRate_KeyUp);
            // 
            // LblReadRate
            // 
            this.LblReadRate.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.753846F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LblReadRate.Location = new System.Drawing.Point(147, 156);
            this.LblReadRate.Name = "LblReadRate";
            this.LblReadRate.Size = new System.Drawing.Size(66, 24);
            this.LblReadRate.TabIndex = 49;
            this.LblReadRate.Text = "Recv Rate:";
            this.LblReadRate.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // RxCount
            // 
            this.RxCount.Font = new System.Drawing.Font("Courier New", 8.861538F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.RxCount.Location = new System.Drawing.Point(67, 156);
            this.RxCount.Name = "RxCount";
            this.RxCount.Size = new System.Drawing.Size(66, 21);
            this.RxCount.TabIndex = 48;
            this.RxCount.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // LblRxCnt
            // 
            this.LblRxCnt.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.753846F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LblRxCnt.Location = new System.Drawing.Point(7, 156);
            this.LblRxCnt.Name = "LblRxCnt";
            this.LblRxCnt.Size = new System.Drawing.Size(60, 24);
            this.LblRxCnt.TabIndex = 47;
            this.LblRxCnt.Text = "RX Count:";
            this.LblRxCnt.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // RxData
            // 
            this.RxData.Font = new System.Drawing.Font("Courier New", 8.861538F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.RxData.Location = new System.Drawing.Point(7, 24);
            this.RxData.Multiline = true;
            this.RxData.Name = "RxData";
            this.RxData.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.RxData.Size = new System.Drawing.Size(820, 132);
            this.RxData.TabIndex = 39;
            // 
            // LblRxData
            // 
            this.LblRxData.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.861538F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LblRxData.ForeColor = System.Drawing.Color.Teal;
            this.LblRxData.Location = new System.Drawing.Point(7, 0);
            this.LblRxData.Name = "LblRxData";
            this.LblRxData.Size = new System.Drawing.Size(820, 24);
            this.LblRxData.TabIndex = 38;
            this.LblRxData.Text = "Received Data";
            this.LblRxData.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // CmdOpen
            // 
            this.CmdOpen.Location = new System.Drawing.Point(7, 424);
            this.CmdOpen.Name = "CmdOpen";
            this.CmdOpen.Size = new System.Drawing.Size(100, 40);
            this.CmdOpen.TabIndex = 59;
            this.CmdOpen.Text = "OnLine";
            this.CmdOpen.Click += new System.EventHandler(this.CmdOpen_Click);
            // 
            // CmdQuit
            // 
            this.CmdQuit.Location = new System.Drawing.Point(740, 424);
            this.CmdQuit.Name = "CmdQuit";
            this.CmdQuit.Size = new System.Drawing.Size(100, 40);
            this.CmdQuit.TabIndex = 58;
            this.CmdQuit.Text = "Terminate";
            this.CmdQuit.Click += new System.EventHandler(this.CmdQuit_Click);
            // 
            // CmdConfig
            // 
            this.CmdConfig.Location = new System.Drawing.Point(640, 424);
            this.CmdConfig.Name = "CmdConfig";
            this.CmdConfig.Size = new System.Drawing.Size(100, 40);
            this.CmdConfig.TabIndex = 57;
            this.CmdConfig.Text = "Configure";
            this.CmdConfig.Click += new System.EventHandler(this.CmdConfig_Click);
            // 
            // CmdCont
            // 
            this.CmdCont.Location = new System.Drawing.Point(207, 424);
            this.CmdCont.Name = "CmdCont";
            this.CmdCont.Size = new System.Drawing.Size(100, 40);
            this.CmdCont.TabIndex = 64;
            this.CmdCont.Text = "Continuous";
            this.CmdCont.Click += new System.EventHandler(this.CmdCont_Click);
            // 
            // CmdClear
            // 
            this.CmdClear.Location = new System.Drawing.Point(307, 424);
            this.CmdClear.Name = "CmdClear";
            this.CmdClear.Size = new System.Drawing.Size(100, 40);
            this.CmdClear.TabIndex = 62;
            this.CmdClear.Text = "Clear";
            this.CmdClear.Click += new System.EventHandler(this.CmdClear_Click);
            // 
            // CmdSend
            // 
            this.CmdSend.Location = new System.Drawing.Point(107, 424);
            this.CmdSend.Name = "CmdSend";
            this.CmdSend.Size = new System.Drawing.Size(100, 40);
            this.CmdSend.TabIndex = 61;
            this.CmdSend.Text = "Transmit";
            this.CmdSend.Click += new System.EventHandler(this.CmdSend_Click);
            // 
            // RecvTimer
            // 
            this.RecvTimer.Tick += new System.EventHandler(this.RecvTimer_Tick);
            // 
            // Terminal
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 15);
            this.ClientSize = new System.Drawing.Size(853, 496);
            this.Controls.Add(this.CmdCont);
            this.Controls.Add(this.CmdClear);
            this.Controls.Add(this.CmdSend);
            this.Controls.Add(this.CmdOpen);
            this.Controls.Add(this.CmdQuit);
            this.Controls.Add(this.CmdConfig);
            this.Controls.Add(this.PnlRxData);
            this.Controls.Add(this.PnlTxData);
            this.Controls.Add(this.PnlModem);
            this.Controls.Add(this.LblStatus);
            this.Controls.Add(this.Status);
            this.Controls.Add(this.PnlCom);
            this.Controls.Add(this.PnlHandshake);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "Terminal";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Serial Test & Debug Terminal";
            this.Closed += new System.EventHandler(this.TermForm_Closed);
            this.PnlCom.ResumeLayout(false);
            this.PnlCom.PerformLayout();
            this.PnlHandshake.ResumeLayout(false);
            this.PnlModem.ResumeLayout(false);
            this.PnlTxData.ResumeLayout(false);
            this.PnlTxData.PerformLayout();
            this.PnlRxData.ResumeLayout(false);
            this.PnlRxData.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		#endregion

		#region Methods
        private void connectMOTO()
        {
            if (this.Port.IsOpen == true) this.PortControl();
            for (int i = 0; i < this.ComPort.Items.Count; i++)
            {
                if (this.Port.cnfg.PortName == (string)this.ComPort.Items[i])
                {
                    this.ComPort.SelectedIndex = i;
                    break;
                }
            }
            this.PortControl();
        }
        private void connectFAVE()
        {
            if (this.Port.IsOpen == true) this.PortControl();

        }
		/// <summary>
		/// Updates the TX/RX data text boxes if continuous is off.
		/// </summary>
		private void ChkUpdate_CheckedChanged(object sender, System.EventArgs e)
		{
			if((this.ChkUpdate.Checked == false) && (this.CmdCont.Text == "Continuous"))
			{
				this.TxDataUpdate("", 0);
				this.RxDataUpdate("", "", 0);
			}
		}

		/// <summary>
		/// Force the XON/XOFF signals.
		/// </summary>
		private void CmdXoff_Click(object sender, System.EventArgs e)
		{
            if(this.CmdXoff.Text == "Set XOFF")
			{
				if(this.Port.SendE(iMobileBarcode.ExtCodes.SetXOFF) == false)
					this.Status.Text = "Set XOFF Extended Function Failed.";
				else
					this.CmdXoff.Text = "Set XON";
			}
			else
			{
				if(this.Port.SendE(iMobileBarcode.ExtCodes.SetXON) == false)
					this.Status.Text = "Set XON Extended Function Failed.";
				else
					this.CmdXoff.Text = "Set XOFF";
			}
		}

		/// <summary>
		/// Force the RTS signal.
		/// </summary>
		private void CmdRts_Click(object sender, System.EventArgs e)
		{
			if(this.CmdRts.Text == "Set RTS")
			{
				if(this.Port.SendE(iMobileBarcode.ExtCodes.SetRTS) == false)
					this.Status.Text = "Set RTS Extended Function Failed.";
				else
					this.CmdRts.Text = "Clr RTS";
			}
			else
			{
				if(this.Port.SendE(iMobileBarcode.ExtCodes.ClrRTS) == false)
					this.Status.Text = "Clear RTS Extended Function Failed.";
				else
					this.CmdRts.Text = "Set RTS";
			}
		}

		/// <summary>
		/// Force the DTR signal.
		/// </summary>
		private void CmdDtr_Click(object sender, System.EventArgs e)
		{
			if(this.CmdDtr.Text == "Set DTR")
			{
				if(this.Port.SendE(iMobileBarcode.ExtCodes.SetDTR) == false)
					this.Status.Text = "Set DTR Extended Function Failed.";
				else
					this.CmdDtr.Text = "Clr DTR";
			}
			else
			{
				if(this.Port.SendE(iMobileBarcode.ExtCodes.ClrDTR) == false)
					this.Status.Text = "Clear DTR Extended Function Failed.";
				else
					this.CmdDtr.Text = "Set DTR";
			}
		}

		/// <summary>
		/// Force the line BREAK.
		/// </summary>
		private void CmdBreak_Click(object sender, System.EventArgs e)
		{
			if(this.CmdBreak.Text == "Set BRK")
			{
				if(this.Port.SendE(iMobileBarcode.ExtCodes.SetBreak) == false)
					this.Status.Text = "Set BREAK Extended Function Failed.";
				else
					this.CmdBreak.Text = "Clr BRK";
			}
			else
			{
				if(this.Port.SendE(iMobileBarcode.ExtCodes.ClrBreak) == false)
					this.Status.Text = "Clear BREAK Extended Function Failed.";
				else
					this.CmdBreak.Text = "Set BRK";
			}
		}

		/// <summary>
		/// Open/close the com port.
		/// </summary>
		private void CmdOpen_Click(object sender, System.EventArgs e)
		{
			this.PortControl();
		}
        public static byte[] pack(byte op, byte src, byte status, byte[] data, int len)
        {
            byte[] buf = new byte[len + 6];
            buf[0] = Convert.ToByte(len+4);
            buf[1] = op;
            buf[2] = src;
            buf[3] = status;
            System.Buffer.BlockCopy(data, 0, buf, 4, len);
            UInt16 sum = 0;
            for (int i = 0; i < len + 4; i++) sum += buf[i];
            UInt16 twoscompl = (UInt16)(~sum + 1); // (UInt16)(0x10000 - sum);
            byte[] twoscomplBA = BitConverter.GetBytes(twoscompl);
            buf[len + 4] = twoscomplBA[1];
            buf[len + 5] = twoscomplBA[0];
            //System.Buffer.BlockCopy(BitConverter.GetBytes(twoscompl), 0, buf, len + 4, 2);
            return buf;
        }
        // There is a AtoX() convert digital string to Hex string
        public static string ByteArrayToString(byte[] data)
        {
            StringBuilder hex = new StringBuilder(data.Length * 2);
            foreach (byte b in data) hex.AppendFormat("{0:X2} ", b);
            return hex.ToString();
        }
        // There is a AtoX() convert digital string to Hex string
        public static string ByteToString(byte data)
        {
            string hex = "";
            hex += new string(Convert.ToChar(data), 1);
            return hex;
        }
        public static byte[] StringToByteArray(string hex)
        {
            string str = hex;
            str.Replace(" ", String.Empty);
            str.Replace("\t", String.Empty);
            str.Replace(",", String.Empty);
            str.Replace("0x", String.Empty);
            int NumberChars = str.Length;
            byte[] bytes = new byte[NumberChars / 2];
            for (int i = 0; i < NumberChars; i += 2)
                bytes[i / 2] = Convert.ToByte(hex.Substring(i, 2), 16);
            return bytes;
        }

        private void parseForMoto(string cmdStr)
        {
            string cmd;
            int cmdIdx = cmdStr.IndexOf(' ');
            byte op = 0x00;
            int opIdx;
            string argStr = "";
            byte[] data = new byte[10];
            int len = 0;

            if (cmdIdx <= 2) len = -999;
            else
            {
                cmd = cmdStr.Substring(0, cmdIdx);
                opIdx = cmdStr.IndexOf(' ', cmdIdx + 1);
                op = StringToByteArray(cmdStr.Substring(cmdIdx + 1, 2))[0];

                if (opIdx < 0)
                {
                    if (cmd == "/s0")
                    {
                        switch (op)
                        {
                            case SSI.OP_ABORT_MACRO_PDF: // no data field
                            case SSI.OP_AIM_OFF:
                            case SSI.OP_AIM_ON:
                            case SSI.OP_CAPABILITIES_REQUEST:
                            case SSI.OP_CMD_ACK:
                            case SSI.OP_FLUSH_MACRO_PDF:
                            case SSI.OP_FLUSH_QUEUE:
                            case SSI.OP_PARAM_DEFAULTS:
                            case SSI.OP_REQUEST_REVISION:
                            case SSI.OP_SCAN_DISABLE:
                            case SSI.OP_SCAN_ENABLE:
                            case SSI.OP_SLEEP:
                            case SSI.OP_START_SESSION:
                            case SSI.OP_STOP_SESSION:
                                len = 0;
                                break;
                            default:
                                len = -1;
                                break;
                        }
                    }
                }
                else if (opIdx == cmdIdx + 3)
                {
                    argStr = cmdStr.Substring(opIdx + 1, cmdStr.Length - opIdx - 1);

                    if (cmd == "/s1")
                    {
                        switch (op)
                        {
                            case SSI.OP_BEEP: // need (INT) beep code
                            case SSI.OP_LED_OFF:
                            case SSI.OP_LED_ON:
                            case SSI.OP_CMD_NAK:
                            case SSI.OP_PARAM_REQUEST:
                            case SSI.OP_IMAGER_MODE:
                                if (argStr.Length > 0)
                                {
                                    data[0] = Convert.ToByte(argStr, 16);
                                    len = 1;
                                }
                                else len = -2;
                                break;
                            default:
                                len = -998;
                                break;
                        }
                    }
                    else len = -998;
                }
                else
                {
                    len = -999;
                }
                if (len >= 0)
                {
                    byte[] package = pack((byte)op, (byte)0x04, (byte)0, data, len);
                    SendBuf(package, 1);
                    this.Status.Text = String.Format("Sent [[[ {0} ]]]", ByteArrayToString(package));
                }
                else if (len == -1) this.Status.Text = String.Format("[E1] Usage({0:X2}): /s0 OP", op);
                else if (len == -2) this.Status.Text = String.Format("[E2] Usage: /s1 OP ARG, require ARG");
                else if (len == -998) this.Status.Text = String.Format("[E998] Usage: /CMD({0}) OP({1:X2}) ARG({2})", cmd, op, argStr);
                else if (len == -999) this.Status.Text = String.Format("[E999] Usage({0}/{1}): /CMD OP ARG", cmdIdx, cmd);
            }
        }

        private void parseForFave(string cmdStr)
        {
        }

        /// <summary>
		/// Sends the TX line (or makes one up if empty).
		/// </summary>
		private void CmdSend_Click(object sender, System.EventArgs e)
		{
			if(this.Port.IsOpen)
			{
                if (this.TxStr.Text != "")
                {
                    string cmdStr = this.TxStr.Text;
                    if (cmdStr == "/MOTO")
                    {
                        dev = "MOTO";
                    }
                    else if (cmdStr == "/FAVE")
                    {
                        dev = "FAVE";
                    }
                    else
                    {
                        if (dev == "MOTO") parseForMoto(cmdStr);
                        else if (dev == "FAVE") parseForFave(cmdStr);
                        else this.Status.Text = "ERROR - Please use /dev to set Device";
                    }
                }
                else
                {
                    this.Status.Text = "ERROR - Please input command to send.";
                }
			}
			else
			{
				this.Status.Text = "ERROR - Cannot Send: The Serial Port Is Closed.";
			}
			return;
		}

		/// <summary>
		/// Start/stop continuous TX.
		/// </summary>
		private void CmdCont_Click(object sender, System.EventArgs e)
		{
			if(this.Port.IsOpen)
			{
				if(this.CmdCont.Text == "Continuous")
				{
					this.CmdCont.Text = "Stop";

					// Start transmitter thread.
					TxThread = new Thread(new ThreadStart(TransmitThread));
					TxThread.Name = "TermXmit";
					TxThread.Priority = ThreadPriority.AboveNormal;
					TxThread.Start();
					TxThreadStop = false;
					this.Status.Text = "Starting Continuous Transmit Thread...";
				}
				else
				{
					this.CmdCont.Text = "Continuous";
					TxThreadStop = true;
				}
			}
			else
			{
				this.Status.Text = "ERROR - Cannot Send: The Serial Port Is Closed.";
			}
		}

		/// <summary>
		/// Clear display fields.
		/// </summary>
		private void CmdClear_Click(object sender, System.EventArgs e)
		{
			TxBytes = 0;
			this.TxString = "";
			this.TxCount.Text = "0";
			this.TxData.Text = "";
			this.TxStr.Text = "";

			RxBytes = 0;
			this.RxString = "";
			this.RxCount.Text = "0";
			this.RxData.Text = "";

			this.Status.Text = "";
		}

		/// <summary>
		/// Open the port settings dialog.
		/// </summary>
		private void CmdConfig_Click(object sender, System.EventArgs e)
		{
			Settings frm = new Settings(this.Port.ConfigFileName, this.Port.Cnfg);
			frm.ShowDialog();
		}

		/// <summary>
		/// Quit the terminal app.
		/// </summary>
		private void CmdQuit_Click(object sender, System.EventArgs e)
		{
			this.Port.Close();
			this.Port = null;
			Application.Exit();
		}

		/// <summary>
		/// Quit terminal app.
		/// </summary>
		private void TermForm_Closed(object sender, System.EventArgs e)
		{
			this.CmdQuit_Click(sender, e);
		}

		/// <summary>
		/// Changes RX data display rate on CR.
		/// </summary>
		private void RecvRate_KeyUp(object sender, System.Windows.Forms.KeyEventArgs e)
		{
			if(e.KeyValue == 13)
			{
				try
				{
					this.RecvTimer.Interval = Convert.ToInt32(this.RecvRate.Text, 10);
				}
				catch
				{
					this.RecvTimer.Interval = 100;
				}
			}
		}

		/// <summary>
		/// Pulls data from the driver and updates the RX data text box.
		/// </summary>
		private void RecvTimer_Tick(object sender, System.EventArgs e)
		{
			byte[] b;
			uint nBytes = this.Port.Recv(out b);
            string ext = "";
            
			if(nBytes > 0)
			{
                if (!decodeEv || b[3] == 0)
                {
                    this.SendBuf(SSI.ACK);
                    switch (b[1])
                    {
                        case SSI.OP_DECODE_DATA:
                            CodeType = b[4];
                            string codeTypeStr = SSI.codeTypeToStr(CodeType);

                            Code = "";
                            this.Status.Text += String.Format(" - ");
                            for (int i = 0; i < b[0] - 5; i++)
                            {
                                if (b[i + 5] >= 0x20) Code += ByteToString(b[i + 5]);
                            }
                            ext = String.Format("{0} '{1}'", codeTypeStr, Code);
                            this.Status.Text += ext;
                            keybdEvent.sendToKeyboard(Code);
                            break;
                        case SSI.OP_EVENT:
                            if (b[4] == 0x01) decodeEv = true;
                            ext = "EVENT";
                            break;
                        case SSI.OP_CMD_ACK:
                            ext = "ACK";
                            break;
                        case SSI.OP_CMD_NAK:
                            ext = "[[NAK]]";
                            break;
                        default:
                            ext = String.Format("OP {0:X2}", b[1]);
                            break;
                    }
                }
                else
                {
                    this.Status.Text += String.Format(" - ");
                    Code = "";
                    for (int i = 0; i < nBytes; i++)
                    {
                        if (b[i] >= 0x20) Code += ByteToString(b[i]);
                    }
                    ext = String.Format("'{0}'", Code);
                    this.Status.Text += ext;
                    keybdEvent.sendToKeyboard(Code);
                    decodeEv = false;
                }
                this.RxDataUpdate(ByteArrayToString(b), ext, b.Length);
            }
		}

		/// <summary>
		/// Send data ASAP.
		/// </summary>
		private void TransmitThread()
		{
			int cnt = 80;
			byte[] b = new byte[cnt];
			Random rand = new Random();
			long start = DateTime.Now.Ticks;

			try
			{
				// Send til stopped.
				while(true)
				{
					// Encode random 80 byte buffer.
					for(int i = 0; i < cnt-2; ++i) 
					{
						b[i] = (byte) rand.Next(RANDOM_LO, RANDOM_HI);
					}
					b[78] = 0xD;
					b[79] = 0xA;

					// Send buffer.
					if(this.SendBuf(b, 1) != b.Length)
					{
						this.Status.Text = this.Port.Fault;
						this.CmdCont.Text = "Continuous";
						break;
					}

					// Quit on stop.
					if(TxThreadStop)
						break;
				}
			}
			catch(Exception e)
			{
				this.Status.Text = e.Message;
			}
			finally
			{
				TxThread = null;
				if(TxThreadStop)
				{
					string s="";
					double max = (int) this.Port.Cnfg.BaudRate / 
						((int) this.Port.Cnfg.DataBits + 
						(int) this.Port.Cnfg.StopBits + 1);
					long nTicks = DateTime.Now.Ticks - start;
					long nBytes = long.Parse(this.TxCount.Text);
					long nMsecs = nTicks / 10000;
					long average = (long) ((float)(nBytes*10000000) / ((float) nTicks));
					s = s + "Stopped Continuous Transmit Thread. ";
					s = s + "Sent " + this.TxCount.Text + " Bytes In ";
					s = s +  nMsecs.ToString() + "ms. Average Rate = ";
					s = s +  average.ToString() + " Bytes/Second. ";
					s = s + "Max Possible = " + max.ToString();
					this.Status.Text = s;
				}
			}
			return;
		}

		/// <summary>
		/// Fill the com port selector with a list of available ports.
		/// </summary>
		private void FillAvailable()
		{
			string s;
            
			SerialPort p = new SerialPort(this.Func, false);
			for(int i = 1; i <= MAX_PORTS; i++)
			{
				s = "COM" + i.ToString() + ":";
				if(p.Available(s))
				{
					this.ComPort.Items.Add(s);
				}
			}
			this.ComPort.SelectedIndex = 0;
			return;
		}

		/// <summary>
		/// Converts an ASCII string to hex formatted lines.
		/// </summary>
		private string AtoX(string asc)
		{
            /***
			int nLines;
			int nChars;
			int offset;
			string hex = "";

			// Compute number of hex lines.
			if((asc.Length % 16) > 0)
				nLines = asc.Length/16+1;
			else
				nLines = asc.Length/16;

			// Convert into hex lines.
			for(int i = 0; i < nLines; i++)
			{
				offset = i * 16;
				if((asc.Length - offset) > 16)
					nChars = 16;
				else
					nChars = asc.Length - offset;
				hex += this.HexLine(i, asc.Substring(offset, nChars)) + "\r\n";
			}
             ***/
            string hex = "";
            for (int i = 0; i < asc.Length; i++)
            {
                hex += String.Format("{0}", asc[i]);
            }
			return hex;
		}

		/// <summary>
		/// Converts a 16 byte ASCII string into one hex formatted line.
		/// </summary>
		private string HexLine(int lNum, string asc)
		{
			string hex = "";

			// Copy line to char buffer.
			char[] c = new char[16];
			asc.CopyTo(0, c, 0, asc.Length);

			// Create offset prefix.
			hex += String.Format("{0:X8} - {1:X8}", lNum*16, (lNum+1)*16-1);
			hex += "    ";

			// Convert chars to hex representation.
			for(int i = 0; i < asc.Length; i++)
			{
				if((i != 0) && ((i % 4) == 0))
					hex += " ";
				hex += String.Format("{0:X2}", (byte) c[i]);
			}

			// Add padding.
			int nSpaces = 62 - hex.Length;
			for(int i = 0; i < nSpaces; i++)
				hex += " ";

			// Add ASCII to end of line.
			for(int i = 0; i < asc.Length; i++)
			{
				if(((int) c[i] < 32) || ((int) c[i] > 126))
					hex += ".";
				else
					hex += c[i].ToString();
			}

			// Return hex dump line.
			return hex;
		}

		/// <summary>
		/// Updates the TX data info.
		/// </summary>
		private void TxDataUpdate(string s, int len)
		{
			TxBytes += len;
            TxString = TxString + s + "\r\n";
			this.TxCount.Text = TxBytes.ToString();
			if(this.ChkUpdate.Checked == false)
			{
                this.TxData.Text = TxString;
			}
			return;
		}

		/// <summary>
		/// Updates the RX data info.
		/// </summary>
		private void RxDataUpdate(string s, string ext, int len)
		{
			RxBytes += len;
            RxString = RxString + s + "(" + ext + ")\r\n";
			this.RxCount.Text = RxBytes.ToString();
			if(this.ChkUpdate.Checked == false)
			{
				this.RxData.Text = RxString;
			}
			return;
		}

		/// <summary>
		/// Gets the current port index.
		/// </summary>
		private int PortIndex
		{
			get	
			{
				if(this.Port == null)
					return -1;
				string s = (string) this.ComPort.SelectedItem;
				return Convert.ToInt32(s.Substring(3, s.Length-4), 10);
			}
		}

		/// <summary>
		/// Controls opening/closing the port.
		/// </summary>
		private void PortControl()
		{
			if(this.Port.IsOpen == false)
			{
				if(this.Port.Open(this.PortIndex) == false)
				{
					this.Status.Text = this.Port.Fault;
				}
				else
				{
					this.CmdOpen.Text = "OffLine";
					this.Settings.Text = PortSettings;
					this.HndShake.Text = HsType;
					this.RecvTimer.Enabled = true;
					this.ComPort.Enabled = false;
					this.Status.Text = "Selected Port Opened OK.";
				}
			}
			else
			{
				if(this.Port.IsOpen)
				{
					this.Port.Close();
				}
				this.Settings.Text = "";
				this.HndShake.Text = "";
				this.CmdOpen.Text = "OnLine";
				this.RecvTimer.Enabled = false;
				this.ComPort.Enabled = true;
				this.Status.Text = "Selected Port Closed.";
				this.Port.Signals();
			}
			return;
		}

		/// <summary>
		/// Transmit a string.
		/// </summary>
        private uint SendBuf(string s)
		{
			uint nSent=0;
			if(s != "")
			{
				byte[] b = new byte[s.Length];
				b = Encoding.Default.GetBytes(s);
				nSent = this.SendBuf(b, 0);
			}
			return nSent;
		}

		/// <summary>
		/// Transmit a buffer.
		/// </summary>
		private uint SendBuf(byte[] b, int bShow=0)
		{
			uint nSent=0;
			if(b.Length > 0)
			{
				nSent = this.Port.Send(b);
				if(nSent != b.Length)
				{
					this.Status.Text = this.Port.Fault;
				}
				if (bShow > 0) this.TxDataUpdate(ByteArrayToString(b), b.Length);
			}
			return nSent;
		}
		#endregion

		#region Properties
		/// <summary>
		/// Gets the current port settings.
		/// </summary>
		private string PortSettings
		{
			get	
			{
				string temp =       this.Port.Cnfg.BaudRate.ToString().Substring(5);
				temp = temp + ":" + this.Port.Cnfg.Parity.ToString().Substring(0,1);
				temp = temp + ":" + ((int)this.Port.Cnfg.DataBits).ToString();
				if((int)this.Port.Cnfg.StopBits == 0)
					temp = temp + ":1";
				else if((int)this.Port.Cnfg.StopBits == 1)
					temp = temp + ":1.5";
				else if((int)this.Port.Cnfg.StopBits == 2)
					temp = temp + ":2";
				return temp;
			}
		}

		/// <summary>
		/// Gets the current port handshake description.
		/// </summary>
		private string HsType
		{
			get
			{
				if(this.Port.Cnfg.FlowCtrl == Handshake.None)
					return "None";
				if(this.Port.Cnfg.FlowCtrl == Handshake.CtsRts)
					return "CTS/RTS";
				if(this.Port.Cnfg.FlowCtrl == Handshake.DsrDtr)
					return "DSR/DTR";
				if(this.Port.Cnfg.FlowCtrl == Handshake.XonXoff)
					return "XON/XOFF";
				else
					return "Unknown";
			}
		}
		#endregion

		/********************************************************************************/
		/*******************************   Delegate Hooks   *****************************/
		/********************************************************************************/
		#region Hooks
		/// <summary>
		/// Handles error events.
		/// </summary>
		internal void OnError(string fault)
		{
			this.Status.Text += fault;
			this.PortControl();
		}

		/// <summary>
		/// Set the modem state displays.
		/// </summary>
		internal void OnCts(bool cts)
		{
			this.PnlCTS.BackColor  = cts  ? Color.Lime : Color.Red;
		}

		/// <summary>
		/// Set the modem state displays.
		/// </summary>
		internal void OnDsr(bool dsr)
		{
			this.PnlDSR.BackColor  = dsr  ? Color.Lime : Color.Red;
		}

		/// <summary>
		/// Set the modem state displays.
		/// </summary>
		internal void OnRlsd(bool rlsd)
		{
			this.PnlRLSD.BackColor = rlsd ? Color.Lime : Color.Red;
		}

		/// <summary>
		/// Set the modem state displays.
		/// </summary>
		internal void OnRing(bool ring)
		{
			this.PnlRING.BackColor = ring ? Color.Lime : Color.Red;
		}

		/// <summary>
		/// Immediate byte received.
		/// </summary>
		internal void OnRecvI(byte[] b)
		{
			this.RxDataUpdate(ByteArrayToString(b), "I", b.Length);
		}
		#endregion
	}
    /// <summary>
    /// Used to launch a stand-alone terminal app.
    /// </summary>
    public class TerminalMain
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Terminal mf = new Terminal();
            Application.Run();
            return;
        }
    }
}

