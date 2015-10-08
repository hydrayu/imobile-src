using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Security.Principal;
using System.Security.AccessControl;
using System.Runtime.InteropServices;

// Libraries needed to work with VideoInputDevices
using AForge.Video;
using AForge.Video.DirectShow;

namespace WebcamSecurity
{
    public partial class MainForm : Form
    {
        // Refrence to cameraMonitors of all 2 cameras
        CameraMonitor[] CamMonitor = new CameraMonitor[2];

        // Indexed arrays containing referces to the user interface components
        // so they can be easily accessed later on

        PictureBox[] DisplayReference = new PictureBox[2];
        int activeCamera = 0;
        System.Drawing.Imaging.ImageFormat[] ifPictureFormat = new System.Drawing.Imaging.ImageFormat[3];
        string[] sPictureExt = new string[3];
        int iPictureFormatSelected = 0;
        bool bResolutionEqual = false;
        bool bNoLog = false;
        bool bLED = false;

        [DllImport("ACamDll.dll")]
        public static extern int ALC_Initialization();
        [DllImport("ACamDll.dll")]
        public static extern bool ALC_UnInitialization();
        [DllImport("ACamDll.dll")]
        public static extern bool ALC_ProcWriteToISP(Byte wAddr, Byte length, Byte[] pValue);
        [DllImport("ACamDll.dll")]
        public static extern bool ALC_ProcReadFromISP(Byte wAddr, Byte length, Byte[] pValue);
        [DllImport("ACamDll.dll")]
        public static extern bool ALC_ProcWriteToI2C(Byte wAddr, Byte length, Byte[] buff);
        [DllImport("ACamDll.dll")]
        public static extern bool ALC_ProcReadFromI2C(Byte wAddr, Byte length, Byte[] buff);
        [DllImport("ACamDll.dll")]
        public static extern bool ALC_GetFirmwareVersion(char[] pRomVersion);
        [DllImport("ACamDll.dll")]
        public static extern bool ALC_GetSensorSettingVersion(Byte[] SettingVer);
        [DllImport("ACamDll.dll")]
        public static extern bool ALC_GetPIDVID(Byte[] VID, Byte[] PID);

        public void log(string msg)
        {
            if (!bNoLog)
            {
                textBoxLog.Text += msg + "\r\n";
                textBoxLog.SelectionStart = textBoxLog.Text.Length;
                textBoxLog.ScrollToCaret();
            }
        }

        private void log(Byte[] baMsg, string prev, int mode)
        {
            string sLog = "";
            foreach (Byte letter in baMsg)
            {
                if (letter == 0x00) break;
                if (mode == 0)
                    sLog += Char.ConvertFromUtf32(letter); // String.Format("{0:X} ", value);
                else
                    sLog += String.Format("{0:00X} ", letter);
            }
            log(prev + " '" + sLog + "'");
        }

        private void log(char[] caMsg, string prev, int mode)
        {
            string sLog = "";
            foreach (char letter in caMsg)
            {
                if (letter == 0x00) break;
                if (mode == 0)
                    sLog += Char.ConvertFromUtf32(letter);
                else
                    sLog += String.Format("{0:X} ", letter);
            }
            log(prev + " '" + sLog + "'");
        }

        public MainForm()
        {
            InitializeComponent();
            // linking the user interface componets to the arrays
            this.DisplayReference[0] = this.Display_Cam1;
            this.DisplayReference[1] = this.Display_Cam2;

            // these two lines to remove title bar entirely
            this.ControlBox = false; // remove control box
            this.Text = ""; // remove title bar
            ifPictureFormat[0] =  System.Drawing.Imaging.ImageFormat.Jpeg;
            ifPictureFormat[1] =  System.Drawing.Imaging.ImageFormat.Png;
            ifPictureFormat[2] = System.Drawing.Imaging.ImageFormat.Bmp;
            sPictureExt[0] = "jpg";
            sPictureExt[1] = "png";
            sPictureExt[2] = "bmp";
        }

        // the FilterInfoCollection is where we get information about VideoCaptureDevices
        private FilterInfoCollection webcams;

        // When the form loads
        private void Form1_Load(object sender, EventArgs e)
        {
            this.textBoxOutputDirectory.Text = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);

            // an instance of FilterInfoCollection is created to fetch available VideoCaptureDevices
            webcams = new FilterInfoCollection(FilterCategory.VideoInputDevice);
            if (webcams.Count == 0)
            {
                groupBoxMainMenu.Enabled = false;
                groupBoxMotionDetectOptions.Enabled = false;
                log("There is no any camera!");
                return;
            }
            // we create our CameraMonitors
            for (int i = 0; i < webcams.Count && i<2; i++)
            {
                this.CamMonitor[i] = new CameraMonitor(this.DisplayReference[i], webcams[i].MonikerString,"Camera"+(i+1));
                this.CamMonitor[i].RecordingPath = textBoxOutputDirectory.Text;
            }
            if (webcams.Count == 1)
            {
                buttonFocusCam2.BackColor = DefaultBackColor;
                buttonFocusCam2.Enabled = false;
            }
            this.buttonFocusCam1.Focus();
            TakingPicture = 0;
            this.SetFocus(0);
        }

        // this method will stop recording and running cameras 
        // also save the options to an xml file
        private void StopCameras(object sender, FormClosingEventArgs e)
        {
            for (int i = 0; i < webcams.Count && i < 2; i++)
            {
                try
                {
                    this.CamMonitor[i].StopRecording();
                    this.CamMonitor[i].StopCapture();
                }
                catch (Exception ex) {
                    log("Error while stop Camera " + webcams[i].Name);
                }
            }
        }

        private void toggleOption(int camIndex, int optionIndex, bool value)
        {
            switch (optionIndex)
            {
                case 0:
                    this.CamMonitor[camIndex].MotionDetection = value;
                    log("Motion Detect for " + webcams[activeCamera].Name + " is " + (value?"on":"off"));
                    break;
                case 1:
                    this.CamMonitor[camIndex].RecordOnMotion = value;
                    log("Record on motion for " + webcams[activeCamera].Name + " is " + (value?"on":"off"));
                    break;
                case 2:
                    this.CamMonitor[camIndex].BeepOnMotion = value;
                    log("Beep on motion for " + webcams[activeCamera].Name + " is " + (value?"on":"off"));
                    break;
            }
        }

        private void MotionDetection1_CheckedChanged(object sender, EventArgs e)
        {
            if (((CheckBox)sender).Checked)
            {
                this.toggleOption(0, 0, true);
            }
            else {
                this.toggleOption(0, 0, false);
                this.AutoRecordWhenMotion.Checked = false;
            }
        }

        private void AutoRecord1_CheckedChanged(object sender, EventArgs e)
        {
            if (((CheckBox)sender).Checked)
            {
                this.toggleOption(0, 1, true);
                this.toggleOption(0, 0, true);
                this.MotionDetection1.Checked = true;
            }
            else
            {
                this.toggleOption(0, 1, false);
                this.CamMonitor[activeCamera].IsRecording = false;
            }
        }
        private void BeepOnMotionCheck1_CheckedChanged(object sender, EventArgs e)
        {
            if (((CheckBox)sender).Checked)
            {
                this.toggleOption(0, 2, true);
            }
            else
            {
                this.toggleOption(0, 2, false);
            }
        }

        private void startCamera()
        {
            if (webcams.Count > 1) {
                if (CamMonitor[1-activeCamera].IsRecording) CamMonitor[1-activeCamera].StopRecording();
                if (CamMonitor[1-activeCamera].IsRunning) CamMonitor[1-activeCamera].StopCapture();
            }
            if (!CamMonitor[activeCamera].IsRunning) CamMonitor[activeCamera].StartCapture();
        }

        private void SetFocus(int camIndex)
        {
            activeCamera = camIndex;

            DisplayReference[1 - activeCamera].Hide();
            DisplayReference[activeCamera].Show();
            
            this.textBoxOutputDirectory.Text = CamMonitor[activeCamera].RecordingPath;
            VideoCapabilities[] vcs = CamMonitor[activeCamera].cam.VideoCapabilities;
            cbVideoResolutions.Items.Clear();

            foreach (VideoCapabilities vc in vcs)
            {
                cbVideoResolutions.Items.Add(string.Format("{0}x{1}x{2}",
                    vc.FrameSize.Width, vc.FrameSize.Height,vc.AverageFrameRate));
            }
            cbVideoResolutions.SelectedIndex = 0;
            CamMonitor[activeCamera].cam.VideoResolution = vcs[0];
            startCamera();
        }

        private void buttonFocusCam1_Click(object sender, EventArgs e)
        {
            this.SetFocus(0);
        }

        private void buttonFocusCam2_Click(object sender, EventArgs e)
        {
            this.SetFocus(1);
        }

        private void buttonRecord_Click(object sender, EventArgs e)
        {
            if (this.CamMonitor[activeCamera].IsRecording)
            {
                this.CamMonitor[activeCamera].StopRecording();
                this.CamMonitor[activeCamera].forceRecord = false;
                
                ((Button)sender).Text = "Record";
            }
            else
            {
                this.CamMonitor[activeCamera].StartRecording();
                this.CamMonitor[activeCamera].forceRecord = true;
                ((Button)sender).Text = "Stop";
            }
        }

        private void buttonPicture_Click(object sender, EventArgs e)
        {
            buttonPicture.Enabled = false;
            System.Media.SoundPlayer myPlayer = new System.Media.SoundPlayer();
            myPlayer.Play();
            try
            {
                string filename = this.CamMonitor[activeCamera].RecordingPath + "\\" +
                    this.CamMonitor[activeCamera].cameraName +
                    String.Format("{0:_dd-M-yyyy_hh-mm-ss}", DateTime.Now) + "." + sPictureExt[iPictureFormatSelected];
                VideoCapabilities[] vcs = CamMonitor[activeCamera].cam.VideoCapabilities;

                if (DisplayReference[activeCamera].Image.Width * DisplayReference[activeCamera].Image.Height <
                    vcs[cbVideoResolutions.SelectedIndex].FrameSize.Width *
                    vcs[cbVideoResolutions.SelectedIndex].FrameSize.Height)
                {
                    Bitmap result = 
                        new Bitmap(vcs[cbVideoResolutions.SelectedIndex].FrameSize.Width,
                            vcs[cbVideoResolutions.SelectedIndex].FrameSize.Height);
                    using (Graphics graphics = Graphics.FromImage(result))
                    {
                        graphics.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighQuality;
                        graphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
                        graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
                        graphics.DrawImage(DisplayReference[activeCamera].Image, 0, 0, result.Width, result.Height);
                        result.Save(filename, ifPictureFormat[iPictureFormatSelected]);
                        result.Dispose();
                    }
                }
                else
                {
                    DisplayReference[activeCamera].Image.Save(filename, ifPictureFormat[iPictureFormatSelected]);
                }
                
                Program.mf.log("Take a picture into " + filename);
            }
            finally {
                buttonPicture.Enabled = true;
                GC.Collect();
                GC.WaitForPendingFinalizers();
                GC.Collect();
            }
        }

        private static bool checkDirectoryPermission(string path)
        {
            try
            {
                FileSystemSecurity security;
                security = File.GetAccessControl(path);
                var rules = security.GetAccessRules(true, true, typeof(NTAccount));
                var currentuser = new WindowsPrincipal(WindowsIdentity.GetCurrent());
                bool result = false;
                foreach (FileSystemAccessRule rule in rules)
                {
                    if (0 == (rule.FileSystemRights & (FileSystemRights.WriteData | FileSystemRights.Write))) continue;
                    if (rule.IdentityReference.Value.StartsWith("S-1-"))
                    {
                        var sid = new SecurityIdentifier(rule.IdentityReference.Value);
                        if (!currentuser.IsInRole(sid)) continue;
                    }
                    else
                    {
                        if (!currentuser.IsInRole(rule.IdentityReference.Value)) continue;
                    }
                    if (rule.AccessControlType == AccessControlType.Deny) return false;
                    if (rule.AccessControlType == AccessControlType.Allow) return true;
                }
                return result;
            }
            catch {
                return false;
            }
        }

        private void buttonBrowseDirectory_Click(object sender, EventArgs e)
        {
            // prompt the user with a FolderBrowserDialog
            FolderBrowserDialog folder = new FolderBrowserDialog();
            int done = 0;

            while (done == 0) {
                folder.ShowDialog();

                if (folder.SelectedPath != "" && checkDirectoryPermission(folder.SelectedPath))
                {
                    done = 1;
                    this.buttonBrowseDirectory.BackColor = DefaultBackColor;
                    this.textBoxOutputDirectory.Text = folder.SelectedPath;
                    CamMonitor[activeCamera].RecordingPath = folder.SelectedPath;
                }
                else
                {
                    DialogResult r = MessageBox.Show("You have no write permission for "+folder.SelectedPath,
                    "Select again", MessageBoxButtons.OKCancel);
                    if (r == DialogResult.Cancel) done = 1;
                }
            }
        }

        private void buttonQuit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private int ticker = 0;
        private void timer1_Tick(object sender, EventArgs e)
        {
            ticker++;
            if (ticker % 10 == 0) textBoxLogClick = 0;
            if (this.CamMonitor[activeCamera].IsRecording)
            {
                this.buttonPicture.Enabled = false;
                this.buttonRecord.Text = "Stop";
            }
            else
            {
                this.buttonPicture.Enabled = true;
                this.buttonRecord.Text = "Record";
            }
        }

        private void cbVideoResolutions_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (CamMonitor[activeCamera].IsRecording) CamMonitor[activeCamera].StopRecording();
            if (CamMonitor[activeCamera].IsRunning) CamMonitor[activeCamera].StopCapture();
            VideoCapabilities[] vcs = CamMonitor[activeCamera].cam.VideoCapabilities;
            VideoCapabilities vc = vcs[cbVideoResolutions.SelectedIndex];
            if (!bResolutionEqual && (vc.FrameSize.Width > 1024 || vc.FrameSize.Height > 768))
            {
                int i=1, selected=0;
                for (; i < vcs.Length; ++i)
                {
                    if (vcs[i].FrameSize.Width <= 1024 && vcs[i].FrameSize.Height <= 768)
                    {
                        if (vcs[i].FrameSize.Width * vcs[i].FrameSize.Height >
                            vcs[selected].FrameSize.Width * vcs[selected].FrameSize.Height)
                        {
                            selected = i;
                        }
                    }
                }
                vc = vcs[selected];
            }
            CamMonitor[activeCamera].cam.VideoResolution = vc;
            startCamera();
        }
        private int textBoxLogClick = 0;
        private void textBoxLog_Click(object sender, EventArgs e)
        {
            textBoxLogClick++;
            if (textBoxLogClick >= 3)
            {
                textBoxLogClick = 0;
                textBoxLog.Text = "";
                textBoxLog.ReadOnly = !textBoxLog.ReadOnly;
                textBoxLog.BackColor = textBoxLog.ReadOnly?SystemColors.ButtonShadow:DefaultBackColor;
            }
        }

        private void textBoxLog_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (textBoxLog.ReadOnly) return;
            int keyCode = (int)e.KeyChar;
            if (keyCode == 13) // Enter
            {
                string sPictureType = textBoxLog.Text.Trim().Substring(0, 3).ToUpper();
                
                if (sPictureType == "BMP")
                {
                    iPictureFormatSelected = 2;
                    textBoxLog.Text = "Take Picture format will be BMP";
                }
                else if (sPictureType == "PNG")
                {
                    iPictureFormatSelected = 1;
                    textBoxLog.Text = "Take Picture format will be PNG";
                }
                else if (sPictureType == "JPG")
                {
                    iPictureFormatSelected = 0;
                    textBoxLog.Text = "Take Picture format will be JPG";
                }
                else if (sPictureType == "===")
                {
                    bResolutionEqual = true;
                    textBoxLog.Text = "Camera max resolution will follow selected after change the resolution.";
                }
                else if (sPictureType == "!!!")
                {
                    bResolutionEqual = false;
                    textBoxLog.Text = "Camera max resolution will be 1024x768 after change the resolution.";
                }
                else if (sPictureType == "NOL")
                {
                    bNoLog = true;
                    textBoxLog.ReadOnly = true;
                    textBoxLog.BackColor = SystemColors.ButtonShadow;
                    textBoxLog.Text = "Turn log off\r\n";
                }
                else if (sPictureType == "LOG")
                {
                    bNoLog = false;
                    textBoxLog.ReadOnly = true;
                    textBoxLog.BackColor = SystemColors.ButtonShadow;
                    textBoxLog.Text = "Turn log on\r\n";
                }
                else textBoxLog.Text = "";
                textBoxLog.SelectionStart = textBoxLog.Text.Length;
                textBoxLog.ScrollToCaret();
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //// ALC_Initialization() first!

            int ret = ALC_Initialization();
            if (ret != 0) {
                log("ALC_Initialization() return " + ret);
                return;
            }

            //// ALC_GetSensorSettingVersion()

            Byte[] baALC = new Byte[10];

            if (ALC_GetSensorSettingVersion(baALC))
            {
                log(baALC, "ALC_GetSensorSettingVersion() return ", 0);
            }
            else
                log("ALC_GetSensorSettingVersion() return failed!");

            //// ALC_GetFirmwareVersion()

            char[] caALC = new char[10];

            if (ALC_GetFirmwareVersion(caALC))
            {
                log(caALC, "ALC_GetFirmwareVersion() return", 1);
            }
            else
                log("ALC_GetFirmwareVersion() return failed!");

            //// ALC_GetPIDVID()

            Byte[] baVID, baPID;

            baVID = new Byte[10];
            baPID = new Byte[10];

            if (ALC_GetPIDVID(baVID, baPID))
            {
                log(baVID, "ALC_GetPIDVID(), VID: ", 1);
                log(baPID, "ALC_GetPIDVID(), PID: ", 1);
            }
            else
                log("ALC_GetPIDVID() return failed");

            //// Control LED

            Byte[] data = new Byte[1];
            // Byte LedOn = 0x40; // gpio 6
            Byte LedOn = 0x02; // gpio 1
            Byte LedDirc = 0x22;
            Byte LedLoc = 0x20;
            Byte xLen = 1;

            if (bLED)
            {
                // assign GPIO
                ALC_ProcReadFromISP(LedDirc, xLen, data);
                data[0] |= LedOn;
                ALC_ProcWriteToISP(LedDirc, xLen, data);

                // Read and Set LED on
                ALC_ProcReadFromISP(LedLoc, xLen, data);
                data[0] |= LedOn;
                ALC_ProcWriteToISP(LedLoc, xLen, data);
            }
            else
            {
                // assign GPIO
                ALC_ProcReadFromISP(LedDirc, xLen, data);
                data[0] |= (Byte)LedOn;
                ALC_ProcWriteToISP(LedDirc, xLen, data);

                // Read and set LED off
                ALC_ProcReadFromISP(LedLoc, xLen, data);
                data[0] &= (Byte)(~LedOn);
                ALC_ProcWriteToISP(LedLoc, xLen, data);
            }
        }
    }
}
