using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

 // the next imports are the most important libraries for this class

using AForge.Vision.Motion; // Motion detection
using AForge.Video;              //
using AForge.Video.DirectShow;   // Video Recording
using AForge.Video.FFMPEG;       // 

using System.Threading;


namespace WebcamSecurity
{
    class CameraMonitor
    {
        PictureBox display;    // a refrence to the PictureBox on the MainForm
        public VideoCaptureDevice cam; // refrence to the actual VidioCaptureDevice (webcam)
        public String cameraName; // string for display purposes
        public bool IsRunning = false;

        MotionDetector md;
        public CameraMonitor(PictureBox display,string monikerString,String  cameraName)
        {
            this.cameraName = cameraName;
            this.display = display;
            this.display.Paint += new PaintEventHandler(DrawMessage);

            md = new MotionDetector(new TwoFramesDifferenceDetector(), new MotionAreaHighlighting()); // creates the motion detector

            cam = new VideoCaptureDevice(monikerString);
            cam.NewFrame += new NewFrameEventHandler(cam_NewFrame); // defines which method to call when a new frame arrives

            // cam.Start();
            IsRunning = false;
        }

        public void StartCapture()
        {
            if (!this.cam.IsRunning)
            {
                this.cam.Start();
            }
            this.IsRunning = this.cam.IsRunning;
        }

        public void StopCapture()
        {
            if (this.cam.IsRunning)
            {
                // we must stop the VideoCaptureDevice when done to free it so it can be used by other applications
                this.cam.Stop();
            }
            IsRunning = this.cam.IsRunning;
        }

        /*
         * the following method draws information on the PictureBox
         * (date / time / motion if detected / recording state ...)
         */
        private void DrawMessage(object sender, PaintEventArgs e)
        {
            using (Font myFont = new Font("Tahoma", 10,FontStyle.Bold))
            {

                e.Graphics.DrawString(DateTime.Now.ToString() + ((this.motionDetected) ? " + Motion !" : ""), myFont, ((this.motionDetected) ? Brushes.Red : Brushes.Green), new Point(2, 2));
                if (this.IsRecording)
                {
                    if (this.showRecordMarkerCount > 10)
                    {
                        e.Graphics.DrawString("[RECORDING]", myFont, Brushes.Red, new Point(2, 14));

                        if (this.showRecordMarkerCount == 20)
                        {
                            this.showRecordMarkerCount = 0;
                        }
                    }
                    this.showRecordMarkerCount++;
                }
            }
        }
        
        bool motionDetected = false; // was there any motion detected previously
        int calibrateAndResume = 0; // counter used delay/skip frames from being processed by the MotionDetector

        void cam_NewFrame(object sender, NewFrameEventArgs eventArgs)
        {
            Bitmap bit = (Bitmap)eventArgs.Frame.Clone(); // get a copy of the BitMap from the VideoCaptureDevice
            try
            {
                this.display.Image = (Bitmap)bit.Clone(); // displays the current frame on the main form

                if (this.MotionDetection && !this.motionDetected)
                {
                    // if motion detection is enabled and there werent any previous motion detected
                    Bitmap bit2 = (Bitmap)bit.Clone(); // clone the bits from the current frame

                    if (md.ProcessFrame(bit2) > 0.001) // feed the bits to the MD 
                    {
                        if (this.calibrateAndResume > 3)
                        {
                            // if motion was detected in 3 subsequent frames
                            Thread th = new Thread(MotionReaction);
                            th.Start(); // start the motion reaction thread
                        }
                        else this.calibrateAndResume++;
                    }
                    bit2.Dispose();
                }
            }
            finally{}
            if (IsRecording)
            {
                // if recording is enabled we enqueue the current frame to be encoded to a video file
                Graphics gr = Graphics.FromImage(bit);
                Pen p = new Pen(Color.Red);
                p.Width = 5.0f;
                using (Font myFont = new Font("Tahoma", 10, FontStyle.Bold))
                {
                    gr.DrawString(DateTime.Now.ToString(), myFont, Brushes.Red, new Point(2, 2));
                }
                frames.Enqueue((Bitmap)bit.Clone());
                gr.Dispose();
            }
            bit.Dispose();
            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();
        }

        // different option toggles
        public bool RecordOnMotion = false;
        public bool BeepOnMotion = false;
        public bool MotionDetection = false;
        public bool forceRecord = false;
        
        private void MotionReaction()
        {
            this.motionDetected = true;
            if (this.RecordOnMotion)
            {
                this.StartRecording(); // record if Autorecord is toggled
            }
            if (this.BeepOnMotion)
            {
                // beep if BeepOnMotion is toggeled
                System.Console.Beep(400, 500);
                System.Console.Beep(800, 500);
            }
            
            Thread.Sleep(3000); // the user is notified for 10 seconds
            calibrateAndResume = 0;
            this.motionDetected = false;
            Thread.Sleep(1000);
            // the thread waits 3 seconds if there is no motion detected we stop the AutoRecord
            if (!this.forceRecord && this.motionDetected == false)
            {
                this.StopRecording();
            }
        }

        public bool IsRecording = false; // recording flag

        Queue<Bitmap> frames = new Queue<Bitmap>(); // Queue that stors frames to be written by the recorder thread
        public string RecordingPath = "recording"; // default recording path

        private void DoRecord()
        {
            //// we set our VideoFileWriter as well as the file name, resolution and fps
            VideoFileWriter writer = new VideoFileWriter();
            string filename = String.Format("{0}\\{1}_{2:dd-MM-yyyy_hh-mm-ss}.avi",
                RecordingPath, cameraName, DateTime.Now);
            string logStr = "";
            int afr =0;

            if (cam.VideoResolution.FrameSize.Width * cam.VideoResolution.FrameSize.Height >= 1024 * 768)
            {
                afr = 10; // minimum framerate is 10?
            }
            else afr = cam.VideoResolution.AverageFrameRate;
            writer.Open(filename, cam.VideoResolution.FrameSize.Width,
                cam.VideoResolution.FrameSize.Height,
                afr,
                VideoCodec.MPEG4);  // (int)(cam.VideoResolution.AverageFrameRate / 3)
            logStr = String.Format("DoRecord({0}) with ({1},{2}) x {3}",
                filename, cam.VideoResolution.FrameSize.Width,
                cam.VideoResolution.FrameSize.Height,
                afr);
            Program.mf.log(logStr);

            // as long as we're recording
            // we dequeue the BitMaps waiting in the Queue and write them to the file
            while (IsRecording)
            {
                if (frames.Count > 0)
                {
                    Bitmap bmp = frames.Dequeue();
                    writer.WriteVideoFrame(bmp);
                    bmp.Dispose();
                }
            }
            writer.Close();
        }

        int showRecordMarkerCount = 0; // used to display message on the main form
        public void StartRecording()
        {
            if (!IsRecording)
            {
                // if were not already recording we start the recording thread
                this.IsRecording = true;
                Thread th = new Thread(new ThreadStart(DoRecord));
                th.Start();
            }
        }

        // stops recording
        public void StopRecording()
        {
            this.IsRecording = false; // DoRecord thread auto stop
        }
    }
}
