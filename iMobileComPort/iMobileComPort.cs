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
using System.Text.RegularExpressions;

namespace iMobileComPort
{
	/// <summary>
	/// Summary description for Terminal.
	/// </summary>
	public class Terminal : System.Windows.Forms.Form
	{
        private System.Windows.Forms.ComboBox ComPort;
		private System.Windows.Forms.Label LblStatus;
        private System.Windows.Forms.Label LblPort;
		private System.Windows.Forms.Label LblTxCnt;
		private System.Windows.Forms.Label LblRxCnt;
		private System.Windows.Forms.Label LblReadRate;
        private System.Windows.Forms.Label LblMsec1;
		private System.Windows.Forms.Panel PnlTxData;
        private System.Windows.Forms.Panel PnlRxData;
		private System.Windows.Forms.Button CmdSend;
        private System.Windows.Forms.Button CmdClear;
		private System.Windows.Forms.CheckBox ChkUpdate;

		private System.Windows.Forms.Timer RecvTimer;

        internal System.Windows.Forms.TextBox TxStr;
		internal System.Windows.Forms.TextBox TxCount;
        internal System.Windows.Forms.TextBox RxCount;
		internal System.Windows.Forms.TextBox Status;
        internal System.Windows.Forms.TextBox RecvRate;

		private System.ComponentModel.IContainer components;

		#region Members
		// Terminal app constants.
		private const int	MAX_PORTS  = 20;
		private const int	RANDOM_LO  = 32;
		private const int	RANDOM_HI  = 127;
		private const int	RANDOM_CNT = 80;

		// Terminal app locals.
		private int			TxBytes = 0;
		private int			RxBytes = 0;
		private string		TxString = "";
		private string		RxString = "";
        private byte CodeType = 0;
        private string Code = "";
        private bool decodeEv = false;

		// Terminal interface port.
        private string dev = "";
        private byte[] favRecv;
        private bool favRecvStart = false;
        private string lastRead = "";
        private bool willSendKB = true;
        private StartCnfg startcnfg;
        public ComPort[] ports = new ComPort[MAX_PORTS];
        public CheckBox[] PortCBs;
        private SerialPort Port;

        private CheckBox PortCB1;
        private CheckBox PortCB2;
        private CheckBox PortCB3;
        private CheckBox PortCB4;
        private CheckBox PortCB5;
        private CheckBox PortCB6;
        private CheckBox PortCB7;
        private CheckBox PortCB8;
        private CheckBox PortCB9;
        private CheckBox PortCB10;
        private CheckBox PortCB11;
        private CheckBox PortCB12;
        private CheckBox PortCB13;
        private CheckBox PortCB14;
        private CheckBox PortCB15;
        private CheckBox PortCB16;
        private CheckBox PortCB17;
        private CheckBox PortCB18;
        private CheckBox PortCB19;
        private CheckBox PortCB20;
        internal TextBox TxRxData;

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
			this.TxRxData.Text = "";
			this.TxStr.Text = "";

			RxBytes = 0;
			this.RxString = "";
			this.RxCount.Text = "0";

			this.Status.Text = "";
			this.RecvRate.Text = this.RecvTimer.Interval.ToString();
            this.PortCBs = new CheckBox[] {
            this.PortCB1, this.PortCB2, this.PortCB3, this.PortCB4, this.PortCB5,
            this.PortCB6, this.PortCB7, this.PortCB8, this.PortCB9, this.PortCB10,
            this.PortCB11, this.PortCB12, this.PortCB13, this.PortCB14, this.PortCB15,
            this.PortCB16, this.PortCB17, this.PortCB18, this.PortCB19, this.PortCB20};
            for (int i = 0; i < PortCBs.Length; i++)
            {
                this.PortCBs[i].Click += new System.EventHandler(this.PortCB_Click);
            }

            // wade
            this.startcnfg = new StartCnfg("iMobileComPort.cfg"); // load iMobileComPort.cfg
            if (this.startcnfg.Startup == 1)
            {
                this.WindowState = FormWindowState.Minimized;
                this.Show();
            }
            else if (this.startcnfg.Startup == 2) this.Hide();
            else
                this.Show();

            // Instantiate base class event handlers.
            this.Func = new WithEvents();
            this.Func.Error = new StrnFunc(this.OnError);
            this.Func.RxChar = new ByteFunc(this.OnRecvI);
            this.Func.CtsSig = new BoolFunc(this.OnCts);
            this.Func.DsrSig = new BoolFunc(this.OnDsr);
            this.Func.RlsdSig = new BoolFunc(this.OnRlsd);
            this.Func.RingSig = new BoolFunc(this.OnRing);
            this.FillAvailable();
            if (this.ComPort.Items.Count > 0) this.RecvTimer.Enabled = true;
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
            this.ComPort = new System.Windows.Forms.ComboBox();
            this.LblPort = new System.Windows.Forms.Label();
            this.Status = new System.Windows.Forms.TextBox();
            this.LblStatus = new System.Windows.Forms.Label();
            this.PnlTxData = new System.Windows.Forms.Panel();
            this.CmdClear = new System.Windows.Forms.Button();
            this.TxRxData = new System.Windows.Forms.TextBox();
            this.CmdSend = new System.Windows.Forms.Button();
            this.TxStr = new System.Windows.Forms.TextBox();
            this.LblMsec1 = new System.Windows.Forms.Label();
            this.ChkUpdate = new System.Windows.Forms.CheckBox();
            this.RecvRate = new System.Windows.Forms.TextBox();
            this.TxCount = new System.Windows.Forms.TextBox();
            this.LblReadRate = new System.Windows.Forms.Label();
            this.RxCount = new System.Windows.Forms.TextBox();
            this.LblTxCnt = new System.Windows.Forms.Label();
            this.LblRxCnt = new System.Windows.Forms.Label();
            this.PnlRxData = new System.Windows.Forms.Panel();
            this.PortCB20 = new System.Windows.Forms.CheckBox();
            this.PortCB19 = new System.Windows.Forms.CheckBox();
            this.PortCB18 = new System.Windows.Forms.CheckBox();
            this.PortCB17 = new System.Windows.Forms.CheckBox();
            this.PortCB16 = new System.Windows.Forms.CheckBox();
            this.PortCB15 = new System.Windows.Forms.CheckBox();
            this.PortCB14 = new System.Windows.Forms.CheckBox();
            this.PortCB13 = new System.Windows.Forms.CheckBox();
            this.PortCB12 = new System.Windows.Forms.CheckBox();
            this.PortCB11 = new System.Windows.Forms.CheckBox();
            this.PortCB10 = new System.Windows.Forms.CheckBox();
            this.PortCB9 = new System.Windows.Forms.CheckBox();
            this.PortCB8 = new System.Windows.Forms.CheckBox();
            this.PortCB7 = new System.Windows.Forms.CheckBox();
            this.PortCB6 = new System.Windows.Forms.CheckBox();
            this.PortCB5 = new System.Windows.Forms.CheckBox();
            this.PortCB4 = new System.Windows.Forms.CheckBox();
            this.PortCB3 = new System.Windows.Forms.CheckBox();
            this.PortCB2 = new System.Windows.Forms.CheckBox();
            this.PortCB1 = new System.Windows.Forms.CheckBox();
            this.RecvTimer = new System.Windows.Forms.Timer(this.components);
            this.PnlTxData.SuspendLayout();
            this.PnlRxData.SuspendLayout();
            this.SuspendLayout();
            // 
            // ComPort
            // 
            this.ComPort.Font = new System.Drawing.Font("Courier New", 8.861538F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ComPort.Location = new System.Drawing.Point(7, 39);
            this.ComPort.MaxDropDownItems = 16;
            this.ComPort.Name = "ComPort";
            this.ComPort.Size = new System.Drawing.Size(80, 23);
            this.ComPort.TabIndex = 51;
            this.ComPort.SelectedValueChanged += new System.EventHandler(this.ComPort_SelectedValueChanged);
            // 
            // LblPort
            // 
            this.LblPort.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.861538F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LblPort.ForeColor = System.Drawing.Color.Teal;
            this.LblPort.Location = new System.Drawing.Point(7, 7);
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
            // PnlTxData
            // 
            this.PnlTxData.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.PnlTxData.Controls.Add(this.CmdClear);
            this.PnlTxData.Controls.Add(this.TxRxData);
            this.PnlTxData.Controls.Add(this.CmdSend);
            this.PnlTxData.Controls.Add(this.TxStr);
            this.PnlTxData.Controls.Add(this.LblMsec1);
            this.PnlTxData.Controls.Add(this.ChkUpdate);
            this.PnlTxData.Controls.Add(this.RecvRate);
            this.PnlTxData.Controls.Add(this.TxCount);
            this.PnlTxData.Controls.Add(this.LblReadRate);
            this.PnlTxData.Controls.Add(this.RxCount);
            this.PnlTxData.Controls.Add(this.LblTxCnt);
            this.PnlTxData.Controls.Add(this.LblRxCnt);
            this.PnlTxData.Location = new System.Drawing.Point(7, 86);
            this.PnlTxData.Name = "PnlTxData";
            this.PnlTxData.Size = new System.Drawing.Size(833, 380);
            this.PnlTxData.TabIndex = 38;
            // 
            // CmdClear
            // 
            this.CmdClear.Location = new System.Drawing.Point(90, 327);
            this.CmdClear.Name = "CmdClear";
            this.CmdClear.Size = new System.Drawing.Size(81, 40);
            this.CmdClear.TabIndex = 62;
            this.CmdClear.Text = "Clear";
            this.CmdClear.Click += new System.EventHandler(this.CmdClear_Click);
            // 
            // TxRxData
            // 
            this.TxRxData.Font = new System.Drawing.Font("Courier New", 8.861538F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TxRxData.Location = new System.Drawing.Point(3, 3);
            this.TxRxData.Multiline = true;
            this.TxRxData.Name = "TxRxData";
            this.TxRxData.Size = new System.Drawing.Size(823, 297);
            this.TxRxData.TabIndex = 65;
            this.TxRxData.TextChanged += new System.EventHandler(this.TxRxData_TextChanged);
            // 
            // CmdSend
            // 
            this.CmdSend.Location = new System.Drawing.Point(3, 327);
            this.CmdSend.Name = "CmdSend";
            this.CmdSend.Size = new System.Drawing.Size(84, 40);
            this.CmdSend.TabIndex = 61;
            this.CmdSend.Text = "Transmit";
            this.CmdSend.Click += new System.EventHandler(this.CmdSend_Click);
            // 
            // TxStr
            // 
            this.TxStr.Font = new System.Drawing.Font("Courier New", 8.861538F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TxStr.Location = new System.Drawing.Point(3, 306);
            this.TxStr.Name = "TxStr";
            this.TxStr.Size = new System.Drawing.Size(823, 21);
            this.TxStr.TabIndex = 40;
            this.TxStr.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.TxStr_KeyPress);
            // 
            // LblMsec1
            // 
            this.LblMsec1.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.753846F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LblMsec1.Location = new System.Drawing.Point(672, 341);
            this.LblMsec1.Name = "LblMsec1";
            this.LblMsec1.Size = new System.Drawing.Size(40, 24);
            this.LblMsec1.TabIndex = 51;
            this.LblMsec1.Text = "msec";
            this.LblMsec1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // ChkUpdate
            // 
            this.ChkUpdate.Location = new System.Drawing.Point(723, 341);
            this.ChkUpdate.Name = "ChkUpdate";
            this.ChkUpdate.Size = new System.Drawing.Size(100, 24);
            this.ChkUpdate.TabIndex = 64;
            this.ChkUpdate.Text = "Pause Updates";
            this.ChkUpdate.CheckedChanged += new System.EventHandler(this.ChkUpdate_CheckedChanged);
            // 
            // RecvRate
            // 
            this.RecvRate.Font = new System.Drawing.Font("Courier New", 8.861538F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.RecvRate.Location = new System.Drawing.Point(605, 341);
            this.RecvRate.Name = "RecvRate";
            this.RecvRate.Size = new System.Drawing.Size(67, 21);
            this.RecvRate.TabIndex = 50;
            this.RecvRate.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.RecvRate.KeyUp += new System.Windows.Forms.KeyEventHandler(this.RecvRate_KeyUp);
            // 
            // TxCount
            // 
            this.TxCount.Font = new System.Drawing.Font("Courier New", 8.861538F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TxCount.Location = new System.Drawing.Point(329, 341);
            this.TxCount.Name = "TxCount";
            this.TxCount.Size = new System.Drawing.Size(67, 21);
            this.TxCount.TabIndex = 60;
            this.TxCount.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // LblReadRate
            // 
            this.LblReadRate.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.753846F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LblReadRate.Location = new System.Drawing.Point(539, 341);
            this.LblReadRate.Name = "LblReadRate";
            this.LblReadRate.Size = new System.Drawing.Size(66, 24);
            this.LblReadRate.TabIndex = 49;
            this.LblReadRate.Text = "Recv Rate:";
            this.LblReadRate.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // RxCount
            // 
            this.RxCount.Font = new System.Drawing.Font("Courier New", 8.861538F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.RxCount.Location = new System.Drawing.Point(459, 341);
            this.RxCount.Name = "RxCount";
            this.RxCount.Size = new System.Drawing.Size(66, 21);
            this.RxCount.TabIndex = 48;
            this.RxCount.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // LblTxCnt
            // 
            this.LblTxCnt.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.753846F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LblTxCnt.Location = new System.Drawing.Point(263, 341);
            this.LblTxCnt.Name = "LblTxCnt";
            this.LblTxCnt.Size = new System.Drawing.Size(60, 24);
            this.LblTxCnt.TabIndex = 59;
            this.LblTxCnt.Text = "TX Count:";
            this.LblTxCnt.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // LblRxCnt
            // 
            this.LblRxCnt.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.753846F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LblRxCnt.Location = new System.Drawing.Point(399, 341);
            this.LblRxCnt.Name = "LblRxCnt";
            this.LblRxCnt.Size = new System.Drawing.Size(60, 24);
            this.LblRxCnt.TabIndex = 47;
            this.LblRxCnt.Text = "RX Count:";
            this.LblRxCnt.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // PnlRxData
            // 
            this.PnlRxData.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.PnlRxData.Controls.Add(this.ComPort);
            this.PnlRxData.Controls.Add(this.PortCB20);
            this.PnlRxData.Controls.Add(this.LblPort);
            this.PnlRxData.Controls.Add(this.PortCB19);
            this.PnlRxData.Controls.Add(this.PortCB18);
            this.PnlRxData.Controls.Add(this.PortCB17);
            this.PnlRxData.Controls.Add(this.PortCB16);
            this.PnlRxData.Controls.Add(this.PortCB15);
            this.PnlRxData.Controls.Add(this.PortCB14);
            this.PnlRxData.Controls.Add(this.PortCB13);
            this.PnlRxData.Controls.Add(this.PortCB12);
            this.PnlRxData.Controls.Add(this.PortCB11);
            this.PnlRxData.Controls.Add(this.PortCB10);
            this.PnlRxData.Controls.Add(this.PortCB9);
            this.PnlRxData.Controls.Add(this.PortCB8);
            this.PnlRxData.Controls.Add(this.PortCB7);
            this.PnlRxData.Controls.Add(this.PortCB6);
            this.PnlRxData.Controls.Add(this.PortCB5);
            this.PnlRxData.Controls.Add(this.PortCB4);
            this.PnlRxData.Controls.Add(this.PortCB3);
            this.PnlRxData.Controls.Add(this.PortCB2);
            this.PnlRxData.Controls.Add(this.PortCB1);
            this.PnlRxData.Location = new System.Drawing.Point(7, 6);
            this.PnlRxData.Name = "PnlRxData";
            this.PnlRxData.Size = new System.Drawing.Size(833, 74);
            this.PnlRxData.TabIndex = 40;
            // 
            // PortCB20
            // 
            this.PortCB20.AutoSize = true;
            this.PortCB20.Location = new System.Drawing.Point(655, 47);
            this.PortCB20.Name = "PortCB20";
            this.PortCB20.Size = new System.Drawing.Size(62, 16);
            this.PortCB20.TabIndex = 19;
            this.PortCB20.Text = "COM20";
            this.PortCB20.UseVisualStyleBackColor = true;
            // 
            // PortCB19
            // 
            this.PortCB19.AutoSize = true;
            this.PortCB19.Location = new System.Drawing.Point(592, 47);
            this.PortCB19.Name = "PortCB19";
            this.PortCB19.Size = new System.Drawing.Size(62, 16);
            this.PortCB19.TabIndex = 18;
            this.PortCB19.Text = "COM19";
            this.PortCB19.UseVisualStyleBackColor = true;
            // 
            // PortCB18
            // 
            this.PortCB18.AutoSize = true;
            this.PortCB18.Location = new System.Drawing.Point(527, 47);
            this.PortCB18.Name = "PortCB18";
            this.PortCB18.Size = new System.Drawing.Size(62, 16);
            this.PortCB18.TabIndex = 17;
            this.PortCB18.Text = "COM18";
            this.PortCB18.UseVisualStyleBackColor = true;
            // 
            // PortCB17
            // 
            this.PortCB17.AutoSize = true;
            this.PortCB17.Location = new System.Drawing.Point(464, 47);
            this.PortCB17.Name = "PortCB17";
            this.PortCB17.Size = new System.Drawing.Size(62, 16);
            this.PortCB17.TabIndex = 16;
            this.PortCB17.Text = "COM17";
            this.PortCB17.UseVisualStyleBackColor = true;
            // 
            // PortCB16
            // 
            this.PortCB16.AutoSize = true;
            this.PortCB16.Location = new System.Drawing.Point(409, 47);
            this.PortCB16.Name = "PortCB16";
            this.PortCB16.Size = new System.Drawing.Size(62, 16);
            this.PortCB16.TabIndex = 15;
            this.PortCB16.Text = "COM16";
            this.PortCB16.UseVisualStyleBackColor = true;
            // 
            // PortCB15
            // 
            this.PortCB15.AutoSize = true;
            this.PortCB15.Location = new System.Drawing.Point(346, 47);
            this.PortCB15.Name = "PortCB15";
            this.PortCB15.Size = new System.Drawing.Size(62, 16);
            this.PortCB15.TabIndex = 14;
            this.PortCB15.Text = "COM15";
            this.PortCB15.UseVisualStyleBackColor = true;
            // 
            // PortCB14
            // 
            this.PortCB14.AutoSize = true;
            this.PortCB14.Location = new System.Drawing.Point(291, 47);
            this.PortCB14.Name = "PortCB14";
            this.PortCB14.Size = new System.Drawing.Size(62, 16);
            this.PortCB14.TabIndex = 13;
            this.PortCB14.Text = "COM14";
            this.PortCB14.UseVisualStyleBackColor = true;
            // 
            // PortCB13
            // 
            this.PortCB13.AutoSize = true;
            this.PortCB13.Location = new System.Drawing.Point(228, 47);
            this.PortCB13.Name = "PortCB13";
            this.PortCB13.Size = new System.Drawing.Size(62, 16);
            this.PortCB13.TabIndex = 12;
            this.PortCB13.Text = "COM13";
            this.PortCB13.UseVisualStyleBackColor = true;
            // 
            // PortCB12
            // 
            this.PortCB12.AutoSize = true;
            this.PortCB12.Location = new System.Drawing.Point(173, 47);
            this.PortCB12.Name = "PortCB12";
            this.PortCB12.Size = new System.Drawing.Size(62, 16);
            this.PortCB12.TabIndex = 11;
            this.PortCB12.Text = "COM12";
            this.PortCB12.UseVisualStyleBackColor = true;
            // 
            // PortCB11
            // 
            this.PortCB11.AutoSize = true;
            this.PortCB11.Location = new System.Drawing.Point(110, 47);
            this.PortCB11.Name = "PortCB11";
            this.PortCB11.Size = new System.Drawing.Size(62, 16);
            this.PortCB11.TabIndex = 10;
            this.PortCB11.Text = "COM11";
            this.PortCB11.UseVisualStyleBackColor = true;
            // 
            // PortCB10
            // 
            this.PortCB10.AutoSize = true;
            this.PortCB10.Location = new System.Drawing.Point(655, 14);
            this.PortCB10.Name = "PortCB10";
            this.PortCB10.Size = new System.Drawing.Size(62, 16);
            this.PortCB10.TabIndex = 9;
            this.PortCB10.Text = "COM10";
            this.PortCB10.UseVisualStyleBackColor = true;
            // 
            // PortCB9
            // 
            this.PortCB9.AutoSize = true;
            this.PortCB9.Location = new System.Drawing.Point(592, 14);
            this.PortCB9.Name = "PortCB9";
            this.PortCB9.Size = new System.Drawing.Size(56, 16);
            this.PortCB9.TabIndex = 8;
            this.PortCB9.Text = "COM9";
            this.PortCB9.UseVisualStyleBackColor = true;
            // 
            // PortCB8
            // 
            this.PortCB8.AutoSize = true;
            this.PortCB8.Location = new System.Drawing.Point(527, 14);
            this.PortCB8.Name = "PortCB8";
            this.PortCB8.Size = new System.Drawing.Size(56, 16);
            this.PortCB8.TabIndex = 7;
            this.PortCB8.Text = "COM8";
            this.PortCB8.UseVisualStyleBackColor = true;
            // 
            // PortCB7
            // 
            this.PortCB7.AutoSize = true;
            this.PortCB7.Location = new System.Drawing.Point(464, 14);
            this.PortCB7.Name = "PortCB7";
            this.PortCB7.Size = new System.Drawing.Size(56, 16);
            this.PortCB7.TabIndex = 6;
            this.PortCB7.Text = "COM7";
            this.PortCB7.UseVisualStyleBackColor = true;
            // 
            // PortCB6
            // 
            this.PortCB6.AutoSize = true;
            this.PortCB6.Location = new System.Drawing.Point(409, 14);
            this.PortCB6.Name = "PortCB6";
            this.PortCB6.Size = new System.Drawing.Size(56, 16);
            this.PortCB6.TabIndex = 5;
            this.PortCB6.Text = "COM6";
            this.PortCB6.UseVisualStyleBackColor = true;
            // 
            // PortCB5
            // 
            this.PortCB5.AutoSize = true;
            this.PortCB5.Location = new System.Drawing.Point(346, 14);
            this.PortCB5.Name = "PortCB5";
            this.PortCB5.Size = new System.Drawing.Size(56, 16);
            this.PortCB5.TabIndex = 4;
            this.PortCB5.Text = "COM5";
            this.PortCB5.UseVisualStyleBackColor = true;
            // 
            // PortCB4
            // 
            this.PortCB4.AutoSize = true;
            this.PortCB4.Location = new System.Drawing.Point(291, 14);
            this.PortCB4.Name = "PortCB4";
            this.PortCB4.Size = new System.Drawing.Size(56, 16);
            this.PortCB4.TabIndex = 3;
            this.PortCB4.Text = "COM4";
            this.PortCB4.UseVisualStyleBackColor = true;
            // 
            // PortCB3
            // 
            this.PortCB3.AutoSize = true;
            this.PortCB3.Location = new System.Drawing.Point(228, 14);
            this.PortCB3.Name = "PortCB3";
            this.PortCB3.Size = new System.Drawing.Size(56, 16);
            this.PortCB3.TabIndex = 2;
            this.PortCB3.Text = "COM3";
            this.PortCB3.UseVisualStyleBackColor = true;
            // 
            // PortCB2
            // 
            this.PortCB2.AutoSize = true;
            this.PortCB2.Location = new System.Drawing.Point(173, 14);
            this.PortCB2.Name = "PortCB2";
            this.PortCB2.Size = new System.Drawing.Size(56, 16);
            this.PortCB2.TabIndex = 1;
            this.PortCB2.Text = "COM2";
            this.PortCB2.UseVisualStyleBackColor = true;
            // 
            // PortCB1
            // 
            this.PortCB1.AutoSize = true;
            this.PortCB1.Location = new System.Drawing.Point(110, 14);
            this.PortCB1.Name = "PortCB1";
            this.PortCB1.Size = new System.Drawing.Size(56, 16);
            this.PortCB1.TabIndex = 0;
            this.PortCB1.Text = "COM1";
            this.PortCB1.UseVisualStyleBackColor = true;
            // 
            // RecvTimer
            // 
            this.RecvTimer.Interval = 200;
            this.RecvTimer.Tick += new System.EventHandler(this.RecvTimer_Tick);
            // 
            // Terminal
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 15);
            this.ClientSize = new System.Drawing.Size(853, 496);
            this.Controls.Add(this.PnlRxData);
            this.Controls.Add(this.PnlTxData);
            this.Controls.Add(this.LblStatus);
            this.Controls.Add(this.Status);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "Terminal";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Serial Test & Debug Terminal";
            this.Closed += new System.EventHandler(this.TermForm_Closed);
            this.PnlTxData.ResumeLayout(false);
            this.PnlTxData.PerformLayout();
            this.PnlRxData.ResumeLayout(false);
            this.PnlRxData.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		#endregion

		#region Methods
		/// <summary>
		/// Updates the TX/RX data text boxes if continuous is off.
		/// </summary>
		private void ChkUpdate_CheckedChanged(object sender, System.EventArgs e)
		{
			if((this.ChkUpdate.Checked == false))
			{
				this.TxDataUpdate("", 0);
				this.RxDataUpdate("", "", 0);
			}
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
            StringBuilder str = new StringBuilder(hex);
            str.Replace(" ", String.Empty);
            str.Replace("\t", String.Empty);
            str.Replace(",", String.Empty);
            str.Replace("0x", String.Empty);
            string res = str.ToString();

            int NumberChars = res.Length;
            byte[] bytes = new byte[NumberChars / 2];
            for (int i = 0; i < NumberChars; i += 2)
                bytes[i / 2] = Convert.ToByte(res.Substring(i, 2), 16);
            return bytes;
        }
        private bool sendRawData(string cmdStr)
        {
            bool ret = false;
            string cmd, op;
            int cmdIdx = cmdStr.IndexOf(' ');
            if (cmdIdx > 0)
            {
                cmd = cmdStr.Substring(0, cmdIdx);
                op = cmdStr.Substring(cmdIdx + 1, cmdStr.Length - cmdIdx - 1);
                if (cmd == "/srh" || cmd == "/SRH")
                {
                    SendBuf(StringToByteArray(op), 1);
                    ret = true;
                }
                else if (cmd == "/SRA" || cmd == "/sra")
                {
                    StringBuilder str = new StringBuilder(op);
                    str.Replace("\\r", "\r");
                    str.Replace("\\n", "\n");
                    str.Replace("\\t", "\t");
                    str.Replace("\\s", " ");
                    op = str.ToString();
                    SendBuf(Encoding.Default.GetBytes(op), 1);
                    ret = true;
                }
            }
            return ret;
        }
        private void parseForUnknown(string cmdStr)
        {
            StringBuilder str = new StringBuilder(cmdStr);
            str.Replace("\\r", "\r");
            str.Replace("\\n", "\n");
            str.Replace("\\t", "\t");
            str.Replace("\\s", " ");
            string op = str.ToString();
            SendBuf(Encoding.Default.GetBytes(op), 1);
        }
        private void parseForMoto(string cmdStr)
        {
            string cmd;
            int cmdIdx;
            byte op;
            int opIdx;
            string argStr;
            byte[] data;
            int len;

            cmdIdx = cmdStr.IndexOf(' ');
            op = 0x00;
            argStr = "";
            data = new byte[10];
            len = 0;

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
                    SendBuf(package, 0);
                    this.Status.Text = String.Format("Sent [[[ {0} ]]]", ByteArrayToString(package));
                }
                else if (len == -1) this.Status.Text = String.Format("[E1] Usage({0:X2}): /s0 OP", op);
                else if (len == -2) this.Status.Text = String.Format("[E2] Usage: /s1 OP ARG, require ARG");
                else if (len == -998) this.Status.Text = String.Format("[E998] Usage: /CMD({0}) OP({1:X2}) ARG({2})", cmd, op, argStr);
                else if (len == -999) this.Status.Text = String.Format("[E999] Usage({0}/{1}): /CMD OP ARG", cmdIdx, cmd);
            }
        }
        private void parseForFav(string cmdStr, int showTx=1)
        {
            if (cmdStr == "/V" || cmdStr == "/v") // Read Firmware Version
            {
                byte[] package = new byte[2];
                package[0] = (byte)'V';
                package[1] = (byte)'\r';
                this.Status.Text = String.Format("Try to send '{0}'", ByteArrayToString(package));
                SendBuf(package, showTx);
            }
            else if (cmdStr == "/S" || cmdStr == "/s") // Reader ID
            {
                byte[] package = new byte[2];
                package[0] = (byte)'S';
                package[1] = (byte)'\r';
                this.Status.Text = String.Format("Try to send '{0}'", ByteArrayToString(package));
                SendBuf(package, showTx);
            }
            else if (cmdStr == "/Q" || cmdStr == "/q") // read tag EPC ID
            {
                byte[] package = new byte[2];
                package[0] = (byte)'Q';
                package[1] = (byte)'\r';
                this.Status.Text = String.Format("Try to send '{0}'", ByteArrayToString(package));
                SendBuf(package, showTx);
            }
            /* need parse cmdStr, for following commands,
             * W<bank>,<address>,<length>,<data>   Write data to bank:address with data
             * K<password>,recom>                  Kill Tag
             * U<........
             */
            else
            {
                return;
            }
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
                    cmdStr.Trim();
                    if (!sendRawData(cmdStr))
                    {
                        if (dev == "MOTO") parseForMoto(cmdStr);
                        else if (dev == "FAV") parseForFav(cmdStr, 1);
                        else parseForUnknown(cmdStr);
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
		/// Clear display fields.
		/// </summary>
		private void CmdClear_Click(object sender, System.EventArgs e)
		{
			TxBytes = 0;
			this.TxString = "";
			this.TxCount.Text = "0";
			this.TxRxData.Text = "";
			this.TxStr.Text = "";

			RxBytes = 0;
			this.RxString = "";
			this.RxCount.Text = "0";

			this.Status.Text = "";
		}
		/// <summary>
		/// Quit terminal app.
		/// </summary>
		private void TermForm_Closed(object sender, System.EventArgs e)
		{
            for (int i = 0; i < MAX_PORTS; i++)
            {
                if (this.ports[i].Port.IsOpen) this.ports[i].Port.Close();
            }
            this.Port = null;
            Application.Exit();
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
					this.RecvTimer.Interval = 1000;
				}
			}
		}
        private void MotoRecv(byte[] b, uint nBytes)
        {
            string ext = "";

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
        private void FavRecv(byte[] b, uint nBytes)
        {
            string recv = "";
            if (b[0] == (byte)'\n') // Start
            {
                favRecvStart = true;
            }
            if (favRecvStart)
            {
                byte[] tmp;
                if (favRecv != null && favRecv.Length > 0)
                {
                    byte[] rv = new byte[b.Length + favRecv.Length];
                    System.Buffer.BlockCopy(favRecv, 0, rv, 0, favRecv.Length);
                    System.Buffer.BlockCopy(b, 0, rv, favRecv.Length, b.Length);
                    tmp = rv;
                }
                else
                {
                    tmp = b;
                }
                int idxA;
                // discard no use part
                for (idxA = tmp.Length - 2; idxA > 0; idxA--)
                {
                    if (tmp[idxA] == (byte)'\n')
                    { // cut it
                        favRecv = new byte[tmp.Length - idxA];
                        System.Buffer.BlockCopy(tmp, idxA, favRecv, 0, tmp.Length - idxA);
                        break;
                    }
                }
                if (idxA == 0) favRecv = tmp;
            }
            if (nBytes >= 2 && !((b[nBytes - 2] == (byte)'\r') && (b[nBytes - 1] == (byte)'\n'))) return; // not end

            this.Status.Text += String.Format(" - ");
            for (int i = 2; i < favRecv.Length; i++)
            {
                if (favRecv[i] >= 0x20) recv += ByteToString(favRecv[i]);
            }
            if (recv != "" && recv != lastRead)
            {
                this.Status.Text += recv;
                if (recv.Length > 0)
                {
                    if (willSendKB) keybdEvent.sendToKeyboard(recv);
                    this.RecvTimer.Interval = 2000;
                }
                else this.RecvTimer.Interval = 200;
                RecvRate.Text = this.RecvTimer.Interval.ToString();
                lastRead = recv;
                this.RxDataUpdate(ByteArrayToString(b), recv, favRecv.Length);
            }
            else
            {
                lastRead = "";
                this.RecvTimer.Interval = 200;
                RecvRate.Text = this.RecvTimer.Interval.ToString();
            }
            favRecvStart = false;
            favRecv = null;
            parseForFav("/Q", 0);
        }
        private void UnknownRecv(byte[] b, uint nBytes)
        {
            this.RxDataUpdate(ByteArrayToString(b), "", (int)nBytes);
        }
        
        /// <summary>
		/// Pulls data from the driver and updates the RX data text box.
		/// </summary>
		private void RecvTimer_Tick(object sender, System.EventArgs e)
		{
            for (int i = 0; i < this.ports.Length; i++)
            {
                if (!this.ports[i].Port.IsOpen) continue;

                byte[] b;
                uint nBytes = this.ports[i].Port.Recv(out b);

                if (nBytes > 0)
                {
                    if (this.ports[i].dev == "FAV")
                        FavRecv(b, nBytes);
                    else if (this.ports[i].dev == "MOTO")
                        MotoRecv(b, nBytes);
                    else UnknownRecv(b, nBytes);
                }
            }
        }

        /// <summary>
        /// Fill the com port selector with a list of available ports.
        /// </summary>
        private void FillAvailable()
		{
			string s;
            bool hasFAV = false;
            
			for(int i = 0; i < MAX_PORTS; i++)
			{
                s = string.Format("COM{0}:", (i+1));
                ports[i] = new ComPort(this.Func, s, this);
                if (ports[i].Port.Available(s))
                {
                    this.ComPort.Items.Add(s);
                    int portStatus;
                    if ((portStatus = ports[i].PortControl()) == 1)
                        PortCBs[i].Checked = true;
                    else if (portStatus == 0) PortCBs[i].Checked = false;
                    else
                    {
                        PortCBs[i].Checked = false;
                        PortCBs[i].Enabled = false;
                    }
                    if (ports[i].Port.Cnfg.PortName == startcnfg.FavPort)
                    {
                        ports[i].dev = "FAV";
                        hasFAV = true;
                    }
                    else if (ports[i].Port.Cnfg.PortName == startcnfg.MotoPort)
                    {
                        ports[i].dev = "MOTO";
                    }
                }
                else this.PortCBs[i].Enabled = false;
			}
            int selectIdx;
            this.Port = null;
            for (selectIdx = 0; selectIdx < this.ComPort.Items.Count; selectIdx++)
            {
                if (this.startcnfg.PortName == (string)this.ComPort.Items[selectIdx])
                {
                    this.ComPort.SelectedIndex = selectIdx;
                    this.Port = ports[PortIndex - 1].Port;
                    this.dev = this.ports[PortIndex - 1].dev;
                    this.Port = this.ports[PortIndex - 1].Port;
                    break;
                }
            }
            if ((this.Port != null) && hasFAV) parseForFav("/Q", 0);
            return;
		}

		/// <summary>
		/// Converts an ASCII string to hex formatted lines.
		/// </summary>
		private string AtoX(string asc)
		{
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
            TxString = TxString + "> " + s + "\r\n";
			this.TxCount.Text = TxBytes.ToString();
			if(this.ChkUpdate.Checked == false)
			{
                this.TxRxData.Text = TxString;
			}
			return;
		}

		/// <summary>
		/// Updates the RX data info.
		/// </summary>
		private void RxDataUpdate(string s, string ext, int len)
		{
			RxBytes += len;
            RxString = RxString + "< "+ s + "(" + ext + ")\r\n";
			this.RxCount.Text = RxBytes.ToString();
			if(this.ChkUpdate.Checked == false)
			{
				this.TxRxData.Text = RxString;
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
				string s = (string) this.ComPort.SelectedItem;
				return Convert.ToInt32(s.Substring(3, s.Length-4), 10);
			}
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
                if (bShow == 1)
                {
                    willSendKB = false;
                    this.TxDataUpdate(ByteArrayToString(b), b.Length);
                }
                else
                {
                    willSendKB = true;
                }
			}
			return nSent;
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
            for (int i=0; i<MAX_PORTS; i++)
			    this.ports[i].PortControl();
		}

		/// <summary>
		/// Set the modem state displays.
		/// </summary>
		internal void OnCts(bool cts)
		{
		}

		/// <summary>
		/// Set the modem state displays.
		/// </summary>
		internal void OnDsr(bool dsr)
		{
		}

		/// <summary>
		/// Set the modem state displays.
		/// </summary>
		internal void OnRlsd(bool rlsd)
		{
		}

		/// <summary>
		/// Set the modem state displays.
		/// </summary>
		internal void OnRing(bool ring)
		{
		}

		/// <summary>
		/// Immediate byte received.
		/// </summary>
		internal void OnRecvI(byte[] b)
		{
			this.RxDataUpdate(ByteArrayToString(b), "I", b.Length);
		}
		#endregion

        private void TxStr_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == '\r' || e.KeyChar == '\n')
            {
                CmdSend_Click(sender, e);
            }
        }
        private void TxRxData_TextChanged(object sender, EventArgs e)
        {
            this.TxRxData.SelectionStart = this.TxRxData.Text.Length;
            this.TxRxData.ScrollToCaret();
            this.TxRxData.Refresh();
        }

        private void ComPort_SelectedValueChanged(object sender, EventArgs e)
        {
            if (PortIndex > 0)
            {
                this.dev = this.ports[PortIndex - 1].dev;
                this.Port = this.ports[PortIndex - 1].Port;
            }
        }

        private void PortCB_Click(object sender, EventArgs e)
        {
            CheckBox me = sender as CheckBox;
            int idx = Convert.ToInt32(me.Text.Substring(3, me.Text.Length - 3), 10);
            if (idx > 0) {
                int ret = this.ports[idx - 1].PortControl();
                if (ret == 1)
                {
                    me.Checked = true;
                    int selIdx = -1;
                    for (int j = 0; j < this.ComPort.Items.Count; j++)
                    {
                        if (ports[idx - 1].Port.Cnfg.PortName == (string)this.ComPort.Items[j])
                        {
                            selIdx = j;
                        }
                    }
                    if (selIdx >= 0)
                    {
                        this.ComPort.SelectedIndex = selIdx;
                        this.ComPort.Enabled = true;
                    }
                    else this.ComPort.Enabled = false;
                }
                else
                {
                    if (ret == 0) me.Checked = false;
                    else me.Enabled = false;
                    if (PortIndex == idx)
                    {
                        int selIdx = -1;
                        for (int i = 0; i < MAX_PORTS; i++)
                        {
                            if (ports[i].Port.IsOpen)
                            {
                                for (int j = 0; j < this.ComPort.Items.Count; j++)
                                {
                                    if (ports[i].Port.Cnfg.PortName == (string)this.ComPort.Items[j])
                                    {
                                        selIdx = j;
                                    }
                                }
                            }
                        }
                        if (selIdx >= 0) this.ComPort.SelectedIndex = selIdx;
                        else this.ComPort.Enabled = false;
                    }
                }
            }
        }
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

