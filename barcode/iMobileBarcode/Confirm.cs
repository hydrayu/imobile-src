using System;
using System.Drawing;
using System.Windows.Forms;

namespace iMobileBarcode
{
	/// <summary>
	/// Summary description for Dialog.
	/// </summary>
	public class Confirm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button CmdConfirm;
		private System.Windows.Forms.Button CmdCancel;
		private System.Windows.Forms.Label Message;

		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		private bool status = false;

		/// <summary>
		/// Default constructor. Initialize dialog form.
		/// </summary>
		public Confirm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
		}

		/// <summary>
		/// Display the dialog and return the confirm status.
		/// </summary>
		/// <param name="str"></param>
		/// <returns></returns>
		public bool Display(string str)
		{
			this.Message.Text = str;
			this.Select();		
			this.ShowDialog();
			return this.status;
		}

		/// <summary>
		/// Display the dialog and return the confirm status.
		/// </summary>
		/// <param name="str"></param>
		/// <returns></returns>
		public bool Display(string str, int yesno)
		{
			this.CmdCancel.Text = "No";
			this.CmdConfirm.Text = "Yes";
			this.Message.Text = str;
			this.Select();		
			this.ShowDialog();
			return this.status;
		}

		/// <summary>
		/// Display the dialog and return the confirm status.
		/// </summary>
		/// <param name="str"></param>
		/// <returns></returns>
		public bool Display(string hdr, string str)
		{
			this.Text = hdr;
			this.Message.Text = str;
			this.Select();		
			this.ShowDialog();
			return this.status;
		}

		/// <summary>
		/// Display the dialog and return the confirm status.
		/// </summary>
		/// <param name="str"></param>
		/// <returns></returns>
		public bool Display(string hdr, string str, int yesno)
		{
			this.Text = hdr;
			this.CmdCancel.Text = "&No";
			this.CmdConfirm.Text = "&Yes";
			this.Message.Text = str;
			this.Select();		
			this.ShowDialog();
			return this.status;
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
			this.Message = new System.Windows.Forms.Label();
			this.CmdConfirm = new System.Windows.Forms.Button();
			this.CmdCancel = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// Message
			// 
			this.Message.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.Message.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.Message.ForeColor = System.Drawing.SystemColors.ControlText;
			this.Message.Name = "Message";
			this.Message.Size = new System.Drawing.Size(280, 80);
			this.Message.TabIndex = 0;
			this.Message.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// CmdConfirm
			// 
			this.CmdConfirm.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.CmdConfirm.Location = new System.Drawing.Point(32, 96);
			this.CmdConfirm.Name = "CmdConfirm";
			this.CmdConfirm.Size = new System.Drawing.Size(88, 40);
			this.CmdConfirm.TabIndex = 1;
			this.CmdConfirm.Text = "&Confirm";
			this.CmdConfirm.Click += new System.EventHandler(this.CmdConfirm_Click);
			this.CmdConfirm.KeyUp += new System.Windows.Forms.KeyEventHandler(this.KeyPressHandler);
			// 
			// CmdCancel
			// 
			this.CmdCancel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.CmdCancel.Location = new System.Drawing.Point(152, 96);
			this.CmdCancel.Name = "CmdCancel";
			this.CmdCancel.Size = new System.Drawing.Size(88, 40);
			this.CmdCancel.TabIndex = 2;
			this.CmdCancel.Text = "C&ancel";
			this.CmdCancel.Click += new System.EventHandler(this.CmdCancel_Click);
			this.CmdCancel.KeyUp += new System.Windows.Forms.KeyEventHandler(this.KeyPressHandler);
			// 
			// Confirm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 15);
			this.ClientSize = new System.Drawing.Size(280, 140);
			this.ControlBox = false;
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.CmdCancel,
																		  this.CmdConfirm,
																		  this.Message});
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "Confirm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "             Confirmation Requested";
			this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.KeyPressHandler);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// Sets the confirm value to false and closes the dialog.
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void CmdCancel_Click(object sender, System.EventArgs e)
		{
			this.status = false;
			this.Close();
		}

		/// <summary>
		/// Sets the confirm status to true and closes the dialog.
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void CmdConfirm_Click(object sender, System.EventArgs e)
		{
			this.status = true;
			this.Close();
		}

		private void KeyPressHandler(object sender, System.Windows.Forms.KeyEventArgs e)
		{
			int key = e.KeyValue;

			if((key == 'C') || (key == 'Y'))
				this.status = true;
			if((key == 'a') || (key == 'N'))
				this.status = false;

			this.Close();
			return;
		}
	}
}
