namespace imBarcodeReader
{
    partial class imBarcodeReaderForm
    {
        /// <summary>
        /// Erforderliche Designervariable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Verwendete Ressourcen bereinigen.
        /// </summary>
        /// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Vom Windows Form-Designer generierter Code

        /// <summary>
        /// Erforderliche Methode für die Designerunterstützung.
        /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.cmbDevice = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.txtBarcodeFormat = new System.Windows.Forms.TextBox();
            this.txtContent = new System.Windows.Forms.TextBox();
            this.tbThreshold = new System.Windows.Forms.TextBox();
            this.btnThreshold = new System.Windows.Forms.Button();
            this.cbContrast = new System.Windows.Forms.CheckBox();
            this.cbHistogram = new System.Windows.Forms.CheckBox();
            this.btnCamera = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.cbClear = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(37, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "Device";
            // 
            // cmbDevice
            // 
            this.cmbDevice.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cmbDevice.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbDevice.FormattingEnabled = true;
            this.cmbDevice.Location = new System.Drawing.Point(56, 9);
            this.cmbDevice.Name = "cmbDevice";
            this.cmbDevice.Size = new System.Drawing.Size(596, 20);
            this.cmbDevice.TabIndex = 0;
            this.cmbDevice.SelectedIndexChanged += new System.EventHandler(this.cmbDevice_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 37);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(38, 12);
            this.label2.TabIndex = 2;
            this.label2.Text = "Format";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(13, 61);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(42, 12);
            this.label3.TabIndex = 4;
            this.label3.Text = "Content";
            // 
            // txtBarcodeFormat
            // 
            this.txtBarcodeFormat.Location = new System.Drawing.Point(56, 33);
            this.txtBarcodeFormat.Name = "txtBarcodeFormat";
            this.txtBarcodeFormat.ReadOnly = true;
            this.txtBarcodeFormat.Size = new System.Drawing.Size(214, 22);
            this.txtBarcodeFormat.TabIndex = 3;
            // 
            // txtContent
            // 
            this.txtContent.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtContent.Location = new System.Drawing.Point(56, 58);
            this.txtContent.Multiline = true;
            this.txtContent.Name = "txtContent";
            this.txtContent.ReadOnly = true;
            this.txtContent.Size = new System.Drawing.Size(596, 72);
            this.txtContent.TabIndex = 5;
            // 
            // tbThreshold
            // 
            this.tbThreshold.Location = new System.Drawing.Point(410, 33);
            this.tbThreshold.Name = "tbThreshold";
            this.tbThreshold.Size = new System.Drawing.Size(100, 22);
            this.tbThreshold.TabIndex = 8;
            this.tbThreshold.Text = "80";
            this.tbThreshold.KeyDown += new System.Windows.Forms.KeyEventHandler(this.tbThreshold_KeyDown);
            // 
            // btnThreshold
            // 
            this.btnThreshold.BackColor = System.Drawing.Color.Red;
            this.btnThreshold.ForeColor = System.Drawing.SystemColors.ControlLightLight;
            this.btnThreshold.Location = new System.Drawing.Point(347, 32);
            this.btnThreshold.Name = "btnThreshold";
            this.btnThreshold.Size = new System.Drawing.Size(60, 23);
            this.btnThreshold.TabIndex = 9;
            this.btnThreshold.Text = "Threshold";
            this.btnThreshold.UseVisualStyleBackColor = false;
            this.btnThreshold.Click += new System.EventHandler(this.btnThreshold_Click);
            // 
            // cbContrast
            // 
            this.cbContrast.AutoSize = true;
            this.cbContrast.Location = new System.Drawing.Point(586, 36);
            this.cbContrast.Name = "cbContrast";
            this.cbContrast.Size = new System.Drawing.Size(63, 16);
            this.cbContrast.TabIndex = 10;
            this.cbContrast.Text = "Contrast";
            this.cbContrast.UseVisualStyleBackColor = true;
            // 
            // cbHistogram
            // 
            this.cbHistogram.AutoSize = true;
            this.cbHistogram.Location = new System.Drawing.Point(517, 36);
            this.cbHistogram.Name = "cbHistogram";
            this.cbHistogram.Size = new System.Drawing.Size(72, 16);
            this.cbHistogram.TabIndex = 11;
            this.cbHistogram.Text = "Histogram";
            this.cbHistogram.UseVisualStyleBackColor = true;
            // 
            // btnCamera
            // 
            this.btnCamera.Image = global::imBarcodeReader.Properties.Resources.camera48;
            this.btnCamera.Location = new System.Drawing.Point(-1, 84);
            this.btnCamera.Name = "btnCamera";
            this.btnCamera.Size = new System.Drawing.Size(56, 46);
            this.btnCamera.TabIndex = 12;
            this.btnCamera.UseVisualStyleBackColor = true;
            this.btnCamera.Click += new System.EventHandler(this.btnCamera_Click);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(12, 136);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(640, 480);
            this.pictureBox1.TabIndex = 2;
            this.pictureBox1.TabStop = false;
            // 
            // cbClear
            // 
            this.cbClear.Location = new System.Drawing.Point(276, 31);
            this.cbClear.Name = "cbClear";
            this.cbClear.Size = new System.Drawing.Size(43, 23);
            this.cbClear.TabIndex = 13;
            this.cbClear.Text = "Clear";
            this.cbClear.UseVisualStyleBackColor = true;
            this.cbClear.Click += new System.EventHandler(this.cbClear_Click);
            // 
            // imBarcodeReaderForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(660, 623);
            this.Controls.Add(this.cbClear);
            this.Controls.Add(this.cbContrast);
            this.Controls.Add(this.btnCamera);
            this.Controls.Add(this.cbHistogram);
            this.Controls.Add(this.btnThreshold);
            this.Controls.Add(this.tbThreshold);
            this.Controls.Add(this.cmbDevice);
            this.Controls.Add(this.txtContent);
            this.Controls.Add(this.txtBarcodeFormat);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.label1);
            this.Name = "imBarcodeReaderForm";
            this.Text = "imBarcodeReader";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cmbDevice;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox txtBarcodeFormat;
        private System.Windows.Forms.TextBox txtContent;
        private System.Windows.Forms.TextBox tbThreshold;
        private System.Windows.Forms.Button btnThreshold;
        private System.Windows.Forms.CheckBox cbContrast;
        private System.Windows.Forms.CheckBox cbHistogram;
        private System.Windows.Forms.Button btnCamera;
        private System.Windows.Forms.Button cbClear;
    }
}

