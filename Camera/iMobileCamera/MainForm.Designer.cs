namespace WebcamSecurity
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.AutoRecordWhenMotion = new System.Windows.Forms.CheckBox();
            this.groupBoxMotionDetectOptions = new System.Windows.Forms.GroupBox();
            this.MotionDetection1 = new System.Windows.Forms.CheckBox();
            this.BeepOnMotionCheck1 = new System.Windows.Forms.CheckBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.textBoxLog = new System.Windows.Forms.TextBox();
            this.groupBoxOutputDirectory = new System.Windows.Forms.GroupBox();
            this.textBoxOutputDirectory = new System.Windows.Forms.TextBox();
            this.buttonBrowseDirectory = new System.Windows.Forms.Button();
            this.groupBoxMainMenu = new System.Windows.Forms.GroupBox();
            this.buttonLED = new System.Windows.Forms.Button();
            this.cbVideoResolutions = new System.Windows.Forms.ComboBox();
            this.buttonQuit = new System.Windows.Forms.Button();
            this.buttonPicture = new System.Windows.Forms.Button();
            this.buttonRecord = new System.Windows.Forms.Button();
            this.buttonFocusCam2 = new System.Windows.Forms.Button();
            this.buttonFocusCam1 = new System.Windows.Forms.Button();
            this.Display_Cam1 = new System.Windows.Forms.PictureBox();
            this.Display_Cam2 = new System.Windows.Forms.PictureBox();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.groupBoxMotionDetectOptions.SuspendLayout();
            this.panel1.SuspendLayout();
            this.groupBoxOutputDirectory.SuspendLayout();
            this.groupBoxMainMenu.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Display_Cam1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Display_Cam2)).BeginInit();
            this.SuspendLayout();
            // 
            // AutoRecordWhenMotion
            // 
            this.AutoRecordWhenMotion.AutoSize = true;
            this.AutoRecordWhenMotion.Location = new System.Drawing.Point(5, 58);
            this.AutoRecordWhenMotion.Name = "AutoRecordWhenMotion";
            this.AutoRecordWhenMotion.Size = new System.Drawing.Size(180, 23);
            this.AutoRecordWhenMotion.TabIndex = 2;
            this.AutoRecordWhenMotion.Text = "Record when motion";
            this.AutoRecordWhenMotion.UseVisualStyleBackColor = true;
            this.AutoRecordWhenMotion.CheckedChanged += new System.EventHandler(this.AutoRecord1_CheckedChanged);
            // 
            // groupBoxMotionDetectOptions
            // 
            this.groupBoxMotionDetectOptions.BackColor = System.Drawing.Color.Plum;
            this.groupBoxMotionDetectOptions.Controls.Add(this.MotionDetection1);
            this.groupBoxMotionDetectOptions.Controls.Add(this.BeepOnMotionCheck1);
            this.groupBoxMotionDetectOptions.Controls.Add(this.AutoRecordWhenMotion);
            this.groupBoxMotionDetectOptions.Font = new System.Drawing.Font("新細明體", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.groupBoxMotionDetectOptions.Location = new System.Drawing.Point(2, 185);
            this.groupBoxMotionDetectOptions.Name = "groupBoxMotionDetectOptions";
            this.groupBoxMotionDetectOptions.Size = new System.Drawing.Size(208, 115);
            this.groupBoxMotionDetectOptions.TabIndex = 1;
            this.groupBoxMotionDetectOptions.TabStop = false;
            this.groupBoxMotionDetectOptions.Text = "Motion Detect Options";
            // 
            // MotionDetection1
            // 
            this.MotionDetection1.AutoSize = true;
            this.MotionDetection1.Location = new System.Drawing.Point(6, 29);
            this.MotionDetection1.Name = "MotionDetection1";
            this.MotionDetection1.Size = new System.Drawing.Size(56, 23);
            this.MotionDetection1.TabIndex = 4;
            this.MotionDetection1.Text = "Yes";
            this.MotionDetection1.UseVisualStyleBackColor = true;
            this.MotionDetection1.CheckedChanged += new System.EventHandler(this.MotionDetection1_CheckedChanged);
            // 
            // BeepOnMotionCheck1
            // 
            this.BeepOnMotionCheck1.AutoSize = true;
            this.BeepOnMotionCheck1.Location = new System.Drawing.Point(6, 87);
            this.BeepOnMotionCheck1.Name = "BeepOnMotionCheck1";
            this.BeepOnMotionCheck1.Size = new System.Drawing.Size(65, 23);
            this.BeepOnMotionCheck1.TabIndex = 3;
            this.BeepOnMotionCheck1.Text = "Beep";
            this.BeepOnMotionCheck1.UseVisualStyleBackColor = true;
            this.BeepOnMotionCheck1.CheckedChanged += new System.EventHandler(this.BeepOnMotionCheck1_CheckedChanged);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.textBoxLog);
            this.panel1.Controls.Add(this.groupBoxOutputDirectory);
            this.panel1.Controls.Add(this.groupBoxMainMenu);
            this.panel1.Controls.Add(this.groupBoxMotionDetectOptions);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Right;
            this.panel1.Location = new System.Drawing.Point(651, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(213, 577);
            this.panel1.TabIndex = 4;
            // 
            // textBoxLog
            // 
            this.textBoxLog.BackColor = System.Drawing.SystemColors.ButtonShadow;
            this.textBoxLog.HideSelection = false;
            this.textBoxLog.Location = new System.Drawing.Point(10, 432);
            this.textBoxLog.Multiline = true;
            this.textBoxLog.Name = "textBoxLog";
            this.textBoxLog.ReadOnly = true;
            this.textBoxLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxLog.Size = new System.Drawing.Size(191, 109);
            this.textBoxLog.TabIndex = 5;
            this.textBoxLog.TabStop = false;
            this.textBoxLog.Click += new System.EventHandler(this.textBoxLog_Click);
            this.textBoxLog.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxLog_KeyPress);
            // 
            // groupBoxOutputDirectory
            // 
            this.groupBoxOutputDirectory.Controls.Add(this.textBoxOutputDirectory);
            this.groupBoxOutputDirectory.Controls.Add(this.buttonBrowseDirectory);
            this.groupBoxOutputDirectory.Font = new System.Drawing.Font("新細明體", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.groupBoxOutputDirectory.Location = new System.Drawing.Point(3, 306);
            this.groupBoxOutputDirectory.Name = "groupBoxOutputDirectory";
            this.groupBoxOutputDirectory.Size = new System.Drawing.Size(207, 139);
            this.groupBoxOutputDirectory.TabIndex = 4;
            this.groupBoxOutputDirectory.TabStop = false;
            this.groupBoxOutputDirectory.Text = "Output Directory";
            // 
            // textBoxOutputDirectory
            // 
            this.textBoxOutputDirectory.Font = new System.Drawing.Font("新細明體", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.textBoxOutputDirectory.HideSelection = false;
            this.textBoxOutputDirectory.Location = new System.Drawing.Point(7, 72);
            this.textBoxOutputDirectory.Multiline = true;
            this.textBoxOutputDirectory.Name = "textBoxOutputDirectory";
            this.textBoxOutputDirectory.ReadOnly = true;
            this.textBoxOutputDirectory.Size = new System.Drawing.Size(191, 48);
            this.textBoxOutputDirectory.TabIndex = 1;
            this.textBoxOutputDirectory.TabStop = false;
            this.textBoxOutputDirectory.EnabledChanged += new System.EventHandler(this.Form1_Load);
            // 
            // buttonBrowseDirectory
            // 
            this.buttonBrowseDirectory.BackColor = System.Drawing.Color.Red;
            this.buttonBrowseDirectory.Location = new System.Drawing.Point(7, 22);
            this.buttonBrowseDirectory.Name = "buttonBrowseDirectory";
            this.buttonBrowseDirectory.Size = new System.Drawing.Size(191, 40);
            this.buttonBrowseDirectory.TabIndex = 0;
            this.buttonBrowseDirectory.Text = "Browse Directory";
            this.buttonBrowseDirectory.UseVisualStyleBackColor = false;
            this.buttonBrowseDirectory.Click += new System.EventHandler(this.buttonBrowseDirectory_Click);
            // 
            // groupBoxMainMenu
            // 
            this.groupBoxMainMenu.Controls.Add(this.buttonLED);
            this.groupBoxMainMenu.Controls.Add(this.cbVideoResolutions);
            this.groupBoxMainMenu.Controls.Add(this.buttonQuit);
            this.groupBoxMainMenu.Controls.Add(this.buttonPicture);
            this.groupBoxMainMenu.Controls.Add(this.buttonRecord);
            this.groupBoxMainMenu.Controls.Add(this.buttonFocusCam2);
            this.groupBoxMainMenu.Controls.Add(this.buttonFocusCam1);
            this.groupBoxMainMenu.Font = new System.Drawing.Font("新細明體", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.groupBoxMainMenu.Location = new System.Drawing.Point(2, 3);
            this.groupBoxMainMenu.Name = "groupBoxMainMenu";
            this.groupBoxMainMenu.Size = new System.Drawing.Size(208, 176);
            this.groupBoxMainMenu.TabIndex = 3;
            this.groupBoxMainMenu.TabStop = false;
            this.groupBoxMainMenu.Text = "Main Menu";
            // 
            // buttonLED
            // 
            this.buttonLED.BackColor = System.Drawing.Color.Brown;
            this.buttonLED.Font = new System.Drawing.Font("新細明體", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.buttonLED.ForeColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.buttonLED.Location = new System.Drawing.Point(154, 60);
            this.buttonLED.Name = "buttonLED";
            this.buttonLED.Size = new System.Drawing.Size(46, 30);
            this.buttonLED.TabIndex = 3;
            this.buttonLED.Text = "LED";
            this.buttonLED.UseVisualStyleBackColor = false;
            this.buttonLED.Click += new System.EventHandler(this.buttonLED_Click);
            // 
            // cbVideoResolutions
            // 
            this.cbVideoResolutions.FormattingEnabled = true;
            this.cbVideoResolutions.Location = new System.Drawing.Point(8, 62);
            this.cbVideoResolutions.Name = "cbVideoResolutions";
            this.cbVideoResolutions.Size = new System.Drawing.Size(144, 27);
            this.cbVideoResolutions.TabIndex = 2;
            this.cbVideoResolutions.SelectedIndexChanged += new System.EventHandler(this.cbVideoResolutions_SelectedIndexChanged);
            // 
            // buttonQuit
            // 
            this.buttonQuit.BackColor = System.Drawing.Color.GreenYellow;
            this.buttonQuit.Location = new System.Drawing.Point(107, 93);
            this.buttonQuit.Name = "buttonQuit";
            this.buttonQuit.Size = new System.Drawing.Size(92, 80);
            this.buttonQuit.TabIndex = 1;
            this.buttonQuit.Text = "Quit";
            this.buttonQuit.UseVisualStyleBackColor = false;
            this.buttonQuit.Click += new System.EventHandler(this.buttonQuit_Click);
            // 
            // buttonPicture
            // 
            this.buttonPicture.BackColor = System.Drawing.Color.Aqua;
            this.buttonPicture.Location = new System.Drawing.Point(6, 93);
            this.buttonPicture.Name = "buttonPicture";
            this.buttonPicture.Size = new System.Drawing.Size(94, 40);
            this.buttonPicture.TabIndex = 0;
            this.buttonPicture.Text = "Picture";
            this.buttonPicture.UseVisualStyleBackColor = false;
            this.buttonPicture.Click += new System.EventHandler(this.buttonPicture_Click);
            // 
            // buttonRecord
            // 
            this.buttonRecord.BackColor = System.Drawing.Color.Aqua;
            this.buttonRecord.Location = new System.Drawing.Point(6, 133);
            this.buttonRecord.Name = "buttonRecord";
            this.buttonRecord.Size = new System.Drawing.Size(94, 40);
            this.buttonRecord.TabIndex = 0;
            this.buttonRecord.Text = "Record";
            this.buttonRecord.UseVisualStyleBackColor = false;
            this.buttonRecord.Click += new System.EventHandler(this.buttonRecord_Click);
            // 
            // buttonFocusCam2
            // 
            this.buttonFocusCam2.BackColor = System.Drawing.Color.Yellow;
            this.buttonFocusCam2.Location = new System.Drawing.Point(107, 18);
            this.buttonFocusCam2.Name = "buttonFocusCam2";
            this.buttonFocusCam2.Size = new System.Drawing.Size(94, 40);
            this.buttonFocusCam2.TabIndex = 0;
            this.buttonFocusCam2.Text = "Camera 2";
            this.buttonFocusCam2.UseVisualStyleBackColor = false;
            this.buttonFocusCam2.Click += new System.EventHandler(this.buttonFocusCam2_Click);
            // 
            // buttonFocusCam1
            // 
            this.buttonFocusCam1.BackColor = System.Drawing.Color.Yellow;
            this.buttonFocusCam1.Location = new System.Drawing.Point(6, 18);
            this.buttonFocusCam1.Name = "buttonFocusCam1";
            this.buttonFocusCam1.Size = new System.Drawing.Size(94, 40);
            this.buttonFocusCam1.TabIndex = 0;
            this.buttonFocusCam1.Text = "Camera 1";
            this.buttonFocusCam1.UseVisualStyleBackColor = false;
            this.buttonFocusCam1.Click += new System.EventHandler(this.buttonFocusCam1_Click);
            // 
            // Display_Cam1
            // 
            this.Display_Cam1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.Display_Cam1.BackColor = System.Drawing.Color.Black;
            this.Display_Cam1.Location = new System.Drawing.Point(2, 3);
            this.Display_Cam1.Name = "Display_Cam1";
            this.Display_Cam1.Size = new System.Drawing.Size(640, 538);
            this.Display_Cam1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.Display_Cam1.TabIndex = 0;
            this.Display_Cam1.TabStop = false;
            // 
            // Display_Cam2
            // 
            this.Display_Cam2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.Display_Cam2.BackColor = System.Drawing.Color.Black;
            this.Display_Cam2.Location = new System.Drawing.Point(2, 3);
            this.Display_Cam2.Name = "Display_Cam2";
            this.Display_Cam2.Size = new System.Drawing.Size(640, 538);
            this.Display_Cam2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.Display_Cam2.TabIndex = 0;
            this.Display_Cam2.TabStop = false;
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 200;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(864, 577);
            this.Controls.Add(this.Display_Cam2);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.Display_Cam1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "iMobile Camera";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.StopCameras);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBoxMotionDetectOptions.ResumeLayout(false);
            this.groupBoxMotionDetectOptions.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.groupBoxOutputDirectory.ResumeLayout(false);
            this.groupBoxOutputDirectory.PerformLayout();
            this.groupBoxMainMenu.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.Display_Cam1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Display_Cam2)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.CheckBox AutoRecordWhenMotion;

        private System.Windows.Forms.GroupBox groupBoxMotionDetectOptions;
        private System.Windows.Forms.CheckBox BeepOnMotionCheck1;
        private System.Windows.Forms.CheckBox MotionDetection1;

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.GroupBox groupBoxMainMenu;
        public System.Windows.Forms.Button buttonRecord;
        private System.Windows.Forms.Button buttonFocusCam2;
        private System.Windows.Forms.Button buttonFocusCam1;
        private System.Windows.Forms.PictureBox Display_Cam1;
        private System.Windows.Forms.PictureBox Display_Cam2;
        private System.Windows.Forms.Button buttonPicture;
        private System.Windows.Forms.GroupBox groupBoxOutputDirectory;
        private System.Windows.Forms.TextBox textBoxOutputDirectory;
        private System.Windows.Forms.Button buttonBrowseDirectory;
        private System.Windows.Forms.TextBox textBoxLog;
        private System.Windows.Forms.Button buttonQuit;
        private System.Windows.Forms.Timer timer1;
        public int TakingPicture;
        private System.Windows.Forms.ComboBox cbVideoResolutions;
        private System.Windows.Forms.Button buttonLED;
    }
}

