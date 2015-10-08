using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace iMobileBarcode
{
	/// <summary>
	/// Summary description for Configure.
	/// </summary>
	public class Settings : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button CmdSave;
		private System.Windows.Forms.Button CmdCancel;
		private System.Windows.Forms.Panel panel7;
		private System.Windows.Forms.CheckBox ChkParity;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.ComboBox ErrorBox;
		private System.Windows.Forms.CheckBox ChkDiscard;
		private System.Windows.Forms.Label label13;
		private System.Windows.Forms.ComboBox EventBox;
		private System.Windows.Forms.Label label12;
		private System.Windows.Forms.ComboBox EofBox;
		private System.Windows.Forms.Label label11;
		private System.Windows.Forms.ComboBox XoffBox;
		private System.Windows.Forms.Label label10;
		private System.Windows.Forms.ComboBox XonBox;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.ComboBox HshakeBox;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.ComboBox ParityBox;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.ComboBox StopBox;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.ComboBox DataBox;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.ComboBox BaudBox;
		private System.Windows.Forms.ComboBox PortBox;
		private System.Windows.Forms.CheckBox ChkEvent;
		private System.Windows.Forms.ToolTip toolTip;
		private System.ComponentModel.IContainer components;

		// Configuration app constants.
		private const int	MAX_PORTS  = 32;
		private SerialCnfg	config;
		private string		fileName;
		private bool		changed;

		/// <summary>
		/// Default constructor. Initializes the settings form.
		/// </summary>
		/// <param name="fileName">Name of file the receives changes.</param>
		/// <param name="cfg">Reference to current configuration.</param>
		public Settings(string fileName, SerialCnfg cfg)
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			// Set selected port.
			this.PortBox.Items.Add(cfg.PortName);
			this.PortBox.SelectedIndex = 0;

			// Load basic settings.
			this.EnumLoad(typeof(LineSpeed), this.BaudBox,   cfg.BaudRate);
			this.EnumLoad(typeof(ByteSize),  this.DataBox,   cfg.DataBits);
			this.EnumLoad(typeof(StopBits),  this.StopBox,   cfg.StopBits);
			this.EnumLoad(typeof(Parity),    this.ParityBox, cfg.Parity);
			this.EnumLoad(typeof(Handshake), this.HshakeBox, cfg.FlowCtrl);

			// Load ctrl/char boxes.
			this.LoadCtrl(this.XonBox,   cfg.XonChar);
			this.LoadCtrl(this.XoffBox,  cfg.XoffChar);
			this.LoadChar(this.EofBox,   cfg.EofChar);
			this.LoadChar(this.EventBox, cfg.EvtChar);
			this.LoadChar(this.ErrorBox, cfg.ErrChar);

			// Set check boxes.
			this.ChkEvent.Checked   = cfg.ReceiveMode ? true : false;
			this.ChkParity.Checked  = cfg.ErrReplace  ? true : false;
			this.ChkDiscard.Checked = cfg.NullDiscard ? true : false;

			Application.DoEvents();
			this.Focus();

			// Set local references.
			this.config = cfg;
			this.fileName = fileName;
			this.changed  = false;
		}

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
			this.CmdSave = new System.Windows.Forms.Button();
			this.CmdCancel = new System.Windows.Forms.Button();
			this.panel7 = new System.Windows.Forms.Panel();
			this.ChkParity = new System.Windows.Forms.CheckBox();
			this.label7 = new System.Windows.Forms.Label();
			this.ErrorBox = new System.Windows.Forms.ComboBox();
			this.ChkDiscard = new System.Windows.Forms.CheckBox();
			this.ChkEvent = new System.Windows.Forms.CheckBox();
			this.label13 = new System.Windows.Forms.Label();
			this.EventBox = new System.Windows.Forms.ComboBox();
			this.label12 = new System.Windows.Forms.Label();
			this.EofBox = new System.Windows.Forms.ComboBox();
			this.label11 = new System.Windows.Forms.Label();
			this.XoffBox = new System.Windows.Forms.ComboBox();
			this.label10 = new System.Windows.Forms.Label();
			this.XonBox = new System.Windows.Forms.ComboBox();
			this.label6 = new System.Windows.Forms.Label();
			this.HshakeBox = new System.Windows.Forms.ComboBox();
			this.label5 = new System.Windows.Forms.Label();
			this.ParityBox = new System.Windows.Forms.ComboBox();
			this.label4 = new System.Windows.Forms.Label();
			this.StopBox = new System.Windows.Forms.ComboBox();
			this.label3 = new System.Windows.Forms.Label();
			this.DataBox = new System.Windows.Forms.ComboBox();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.BaudBox = new System.Windows.Forms.ComboBox();
			this.PortBox = new System.Windows.Forms.ComboBox();
			this.toolTip = new System.Windows.Forms.ToolTip(this.components);
			this.panel7.SuspendLayout();
			this.SuspendLayout();
			// 
			// CmdSave
			// 
			this.CmdSave.Location = new System.Drawing.Point(24, 464);
			this.CmdSave.Name = "CmdSave";
			this.CmdSave.Size = new System.Drawing.Size(96, 32);
			this.CmdSave.TabIndex = 1;
			this.CmdSave.Text = "Save";
			this.CmdSave.Click += new System.EventHandler(this.CmdSave_Click);
			// 
			// CmdCancel
			// 
			this.CmdCancel.Location = new System.Drawing.Point(128, 464);
			this.CmdCancel.Name = "CmdCancel";
			this.CmdCancel.Size = new System.Drawing.Size(96, 32);
			this.CmdCancel.TabIndex = 2;
			this.CmdCancel.Text = "Cancel";
			this.CmdCancel.Click += new System.EventHandler(this.CmdCancel_Click);
			// 
			// panel7
			// 
			this.panel7.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.panel7.Controls.AddRange(new System.Windows.Forms.Control[] {
																				 this.ChkParity,
																				 this.label7,
																				 this.ErrorBox,
																				 this.ChkDiscard,
																				 this.ChkEvent,
																				 this.label13,
																				 this.EventBox,
																				 this.label12,
																				 this.EofBox,
																				 this.label11,
																				 this.XoffBox,
																				 this.label10,
																				 this.XonBox});
			this.panel7.Location = new System.Drawing.Point(24, 200);
			this.panel7.Name = "panel7";
			this.panel7.Size = new System.Drawing.Size(200, 256);
			this.panel7.TabIndex = 14;
			// 
			// ChkParity
			// 
			this.ChkParity.Location = new System.Drawing.Point(16, 56);
			this.ChkParity.Name = "ChkParity";
			this.ChkParity.Size = new System.Drawing.Size(168, 24);
			this.ChkParity.TabIndex = 41;
			this.ChkParity.Text = "Replace On Parity Error";
			this.toolTip.SetToolTip(this.ChkParity, "Replace each char with an error with the Error Char (set below).");
			this.ChkParity.CheckedChanged += new System.EventHandler(this.ChangeHandler);
			// 
			// label7
			// 
			this.label7.Location = new System.Drawing.Point(8, 224);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(112, 24);
			this.label7.TabIndex = 40;
			this.label7.Text = "Error Character";
			this.label7.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// ErrorBox
			// 
			this.ErrorBox.Location = new System.Drawing.Point(128, 224);
			this.ErrorBox.Name = "ErrorBox";
			this.ErrorBox.Size = new System.Drawing.Size(64, 24);
			this.ErrorBox.TabIndex = 39;
			this.toolTip.SetToolTip(this.ErrorBox, "Parity error replacement char.");
			this.ErrorBox.SelectedIndexChanged += new System.EventHandler(this.ChangeHandler);
			// 
			// ChkDiscard
			// 
			this.ChkDiscard.Location = new System.Drawing.Point(16, 32);
			this.ChkDiscard.Name = "ChkDiscard";
			this.ChkDiscard.Size = new System.Drawing.Size(168, 24);
			this.ChkDiscard.TabIndex = 37;
			this.ChkDiscard.Text = "Discard Received NULs";
			this.toolTip.SetToolTip(this.ChkDiscard, "Throw away all NULL chars received.");
			this.ChkDiscard.CheckedChanged += new System.EventHandler(this.ChangeHandler);
			// 
			// ChkEvent
			// 
			this.ChkEvent.Location = new System.Drawing.Point(16, 8);
			this.ChkEvent.Name = "ChkEvent";
			this.ChkEvent.Size = new System.Drawing.Size(168, 24);
			this.ChkEvent.TabIndex = 36;
			this.ChkEvent.Text = "OnEvent Receive Mode";
			this.toolTip.SetToolTip(this.ChkEvent, "If true, each char received is handled immediately. If false, the receiver must b" +
				"e polled for available data.");
			this.ChkEvent.CheckedChanged += new System.EventHandler(this.ChangeHandler);
			// 
			// label13
			// 
			this.label13.Location = new System.Drawing.Point(8, 192);
			this.label13.Name = "label13";
			this.label13.Size = new System.Drawing.Size(112, 24);
			this.label13.TabIndex = 35;
			this.label13.Text = "Event Character";
			this.label13.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// EventBox
			// 
			this.EventBox.Location = new System.Drawing.Point(128, 192);
			this.EventBox.Name = "EventBox";
			this.EventBox.Size = new System.Drawing.Size(64, 24);
			this.EventBox.TabIndex = 34;
			this.toolTip.SetToolTip(this.EventBox, "Event signaling char.");
			this.EventBox.SelectedIndexChanged += new System.EventHandler(this.ChangeHandler);
			// 
			// label12
			// 
			this.label12.Location = new System.Drawing.Point(8, 160);
			this.label12.Name = "label12";
			this.label12.Size = new System.Drawing.Size(112, 24);
			this.label12.TabIndex = 33;
			this.label12.Text = "EOF Character";
			this.label12.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// EofBox
			// 
			this.EofBox.Location = new System.Drawing.Point(128, 160);
			this.EofBox.Name = "EofBox";
			this.EofBox.Size = new System.Drawing.Size(64, 24);
			this.EofBox.TabIndex = 32;
			this.toolTip.SetToolTip(this.EofBox, "End of file (data) char.");
			this.EofBox.SelectedIndexChanged += new System.EventHandler(this.ChangeHandler);
			// 
			// label11
			// 
			this.label11.Location = new System.Drawing.Point(8, 128);
			this.label11.Name = "label11";
			this.label11.Size = new System.Drawing.Size(112, 24);
			this.label11.TabIndex = 31;
			this.label11.Text = "XOFF Character";
			this.label11.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// XoffBox
			// 
			this.XoffBox.Location = new System.Drawing.Point(128, 128);
			this.XoffBox.Name = "XoffBox";
			this.XoffBox.Size = new System.Drawing.Size(64, 24);
			this.XoffBox.TabIndex = 30;
			this.toolTip.SetToolTip(this.XoffBox, "Software suspend flow char.");
			this.XoffBox.SelectedIndexChanged += new System.EventHandler(this.ChangeHandler);
			// 
			// label10
			// 
			this.label10.Location = new System.Drawing.Point(8, 96);
			this.label10.Name = "label10";
			this.label10.Size = new System.Drawing.Size(112, 24);
			this.label10.TabIndex = 29;
			this.label10.Text = "XON Character";
			this.label10.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// XonBox
			// 
			this.XonBox.Location = new System.Drawing.Point(128, 96);
			this.XonBox.Name = "XonBox";
			this.XonBox.Size = new System.Drawing.Size(64, 24);
			this.XonBox.TabIndex = 28;
			this.toolTip.SetToolTip(this.XonBox, "Software flow resume char.");
			this.XonBox.SelectedIndexChanged += new System.EventHandler(this.ChangeHandler);
			// 
			// label6
			// 
			this.label6.Location = new System.Drawing.Point(16, 168);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(80, 24);
			this.label6.TabIndex = 26;
			this.label6.Text = "Handshake";
			this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// HshakeBox
			// 
			this.HshakeBox.Location = new System.Drawing.Point(104, 168);
			this.HshakeBox.Name = "HshakeBox";
			this.HshakeBox.Size = new System.Drawing.Size(120, 24);
			this.HshakeBox.TabIndex = 25;
			this.toolTip.SetToolTip(this.HshakeBox, "Peer flow control mode.");
			this.HshakeBox.SelectedIndexChanged += new System.EventHandler(this.ChangeHandler);
			// 
			// label5
			// 
			this.label5.Location = new System.Drawing.Point(16, 136);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(80, 24);
			this.label5.TabIndex = 24;
			this.label5.Text = "Parity:";
			this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// ParityBox
			// 
			this.ParityBox.Location = new System.Drawing.Point(104, 136);
			this.ParityBox.Name = "ParityBox";
			this.ParityBox.Size = new System.Drawing.Size(120, 24);
			this.ParityBox.TabIndex = 23;
			this.toolTip.SetToolTip(this.ParityBox, "Receiver parity checking");
			this.ParityBox.SelectedIndexChanged += new System.EventHandler(this.ChangeHandler);
			// 
			// label4
			// 
			this.label4.Location = new System.Drawing.Point(16, 104);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(80, 24);
			this.label4.TabIndex = 22;
			this.label4.Text = "Stop Bits:";
			this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// StopBox
			// 
			this.StopBox.Location = new System.Drawing.Point(104, 104);
			this.StopBox.Name = "StopBox";
			this.StopBox.Size = new System.Drawing.Size(120, 24);
			this.StopBox.TabIndex = 21;
			this.toolTip.SetToolTip(this.StopBox, "Number of stop bits following each char.");
			this.StopBox.SelectedIndexChanged += new System.EventHandler(this.ChangeHandler);
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(16, 72);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(80, 24);
			this.label3.TabIndex = 20;
			this.label3.Text = "Data Bits:";
			this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// DataBox
			// 
			this.DataBox.Location = new System.Drawing.Point(104, 72);
			this.DataBox.Name = "DataBox";
			this.DataBox.Size = new System.Drawing.Size(120, 24);
			this.DataBox.TabIndex = 19;
			this.toolTip.SetToolTip(this.DataBox, "Number of bits/char.");
			this.DataBox.SelectedIndexChanged += new System.EventHandler(this.ChangeHandler);
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(16, 40);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(80, 24);
			this.label2.TabIndex = 18;
			this.label2.Text = "Baud Rate:";
			this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(16, 8);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(80, 24);
			this.label1.TabIndex = 17;
			this.label1.Text = "Serial Port:";
			this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// BaudBox
			// 
			this.BaudBox.Location = new System.Drawing.Point(104, 40);
			this.BaudBox.Name = "BaudBox";
			this.BaudBox.Size = new System.Drawing.Size(120, 24);
			this.BaudBox.TabIndex = 16;
			this.toolTip.SetToolTip(this.BaudBox, "Serial port line speed (bits/second).");
			this.BaudBox.SelectedIndexChanged += new System.EventHandler(this.ChangeHandler);
			// 
			// PortBox
			// 
			this.PortBox.Location = new System.Drawing.Point(104, 8);
			this.PortBox.Name = "PortBox";
			this.PortBox.Size = new System.Drawing.Size(120, 24);
			this.PortBox.TabIndex = 15;
			this.toolTip.SetToolTip(this.PortBox, "Serial port that settings apply to.");
			// 
			// Settings
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 15);
			this.ClientSize = new System.Drawing.Size(238, 506);
			this.ControlBox = false;
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.label6,
																		  this.HshakeBox,
																		  this.label5,
																		  this.ParityBox,
																		  this.label4,
																		  this.StopBox,
																		  this.label3,
																		  this.DataBox,
																		  this.label2,
																		  this.label1,
																		  this.BaudBox,
																		  this.PortBox,
																		  this.panel7,
																		  this.CmdCancel,
																		  this.CmdSave});
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "Settings";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "           Configure Serial Port";
			this.panel7.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// Handle the save command button
		/// </summary>
		private void CmdSave_Click(object sender, System.EventArgs e)
		{
			this.UpdateConfig();
			this.config.Save(this.fileName);
			this.changed = false;
			this.Close();
		}

		/// <summary>
		/// Handle the cancel command button
		/// </summary>
		private void CmdCancel_Click(object sender, System.EventArgs e)
		{
			if(this.changed)
			{
				Confirm dialog = new Confirm();
				if(dialog.Display("Confirm Close", "The Settings Were Changed." +
					"Are You Sure You Want To Discard All Changes?", 1) == false)
				{
					return;
				}
			}
			this.Close();
		}

		/// <summary>
		/// Set the change flag when and control is selected.
		/// </summary>
		private void ChangeHandler(object sender, System.EventArgs e)
		{
			this.changed = true;
		}
		
		/// <summary>
		/// Load the combo box with the printable ASCII and control codes.
		/// </summary>
		/// <param name="cb">Combo box reference.</param>
		/// <param name="obj">Initially focused member.</param>
		private void LoadChar(ComboBox cb, object obj)
		{
			this.LoadCtrl(cb, null);
			for(int c = 33; c < 127; c++)
			{
				cb.Items.Add((char) c);
			}
			cb.SelectedIndex = this.FindMatch(cb, obj);
		}

		/// <summary>
		/// Load the combo box with the ASCII control code members.
		/// </summary>
		/// <param name="cb">Combo box reference.</param>
		/// <param name="obj">Initially focused member.</param>
		private void LoadCtrl(ComboBox cb, object obj)
		{
			this.EnumLoad(typeof(CtrlChar), cb, obj);
			return;
		}

		/// <summary>
		/// Copies to current form settings to the configuration reference.
		/// </summary>
		private void UpdateConfig()
		{
			// Update baud rate.
			this.config.BaudRate = (LineSpeed) int.Parse(this.BaudBox.Text.Substring(5));

			// Update data bits.
			this.config.DataBits = (ByteSize) 
				this.EnumFind(typeof(ByteSize), this.DataBox.Text);

			// Update stop bits.
			this.config.StopBits = (StopBits) 
				this.EnumFind(typeof(StopBits), this.StopBox.Text);

			// Update parity.
			this.config.Parity = (Parity) 
				this.EnumFind(typeof(Parity), this.ParityBox.Text);

			// Update handshake.
			this.config.FlowCtrl = (Handshake) 
				this.EnumFind(typeof(Handshake), this.HshakeBox.Text);

			// Update receive mode, null discard and error replacement.
			this.config.ReceiveMode = this.ChkEvent.Checked;
			this.config.NullDiscard = this.ChkDiscard.Checked;
			this.config.ErrReplace  = this.ChkParity.Checked;

			// Update XON/XOFF characters.
			this.config.XonChar  = (byte) Enum.Parse(typeof(CtrlChar), this.XonBox.Text);
			this.config.XoffChar = (byte) Enum.Parse(typeof(CtrlChar), this.XoffBox.Text);

			// Update EOF, Event and Error characters.
			this.config.EofChar  = (this.EofBox.Text.Length > 1)
				? (byte) Enum.Parse(typeof(CtrlChar), this.EofBox.Text)
				: (byte) Char.Parse(this.EofBox.Text);

			this.config.EvtChar  = (this.EventBox.Text.Length > 1)
				? (byte) Enum.Parse(typeof(CtrlChar), this.EventBox.Text)
				: (byte) Char.Parse(this.EventBox.Text);

			this.config.ErrChar  = (this.ErrorBox.Text.Length > 1)
				? (byte) Enum.Parse(typeof(CtrlChar), this.ErrorBox.Text)
				: (byte) Char.Parse(this.ErrorBox.Text);

			this.config.SetFlowControl();
			return;
		}

		/// <summary>
		/// Return the enum which matches the member name.
		/// </summary>
		/// <param name="type">typeof(Enum)</param>
		/// <param name="member">String member name.</param>
		/// <returns>Enum member or null.</returns>
		private object EnumFind(System.Type type, string member)
		{
			try
			{
				return Enum.Parse(type, member);
			}
			catch
			{
				return null;
			}
		}

		/// <summary>
		/// Load a combo box list with the members of an enum.
		/// </summary>
		/// <param name="type">typeof(Enum)</param>
		/// <param name="cb">Combo box reference.</param>
		/// <param name="obj">Initially focused member.</param>
		private void EnumLoad(System.Type type, ComboBox cb, object obj)
		{
			try
			{
				string[] names = Enum.GetNames(type);
				for(int i = 0; i < names.Length; i++)
				{
					cb.Items.Add(Enum.Parse(type, names[i]));
				}
				cb.SelectedIndex = this.FindMatch(cb, obj);
			}
			catch
			{
				return;
			}
		}

		/// <summary>
		/// Find a matching object (integer) in the combo box.
		/// </summary>
		/// <param name="cb">Combo box reference.</param>
		/// <param name="obj">Object to find.</param>
		/// <returns>Index of object.</returns>
		private int FindMatch(ComboBox cb, object obj)
		{
			if(obj == null)
				return 0;
			
			int i = 0;
			foreach(object item in cb.Items)
			{
				if(Convert.ToInt32(item) == Convert.ToInt32(obj))
				{
					return i;
				}
				++i;
			}
			return 0;
		}

	}
}
