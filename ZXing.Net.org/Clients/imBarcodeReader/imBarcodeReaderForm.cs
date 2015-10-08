/*
 * Copyright 2012 ZXing.Net authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Threading;
using System.Windows.Forms;

using AForge.Video;
using AForge.Imaging;
using AForge.Imaging.Filters;
using ZXing;

namespace imBarcodeReader
{
    /// <summary>
    /// imBarcodeReaderForm class
    /// </summary>
    public partial class imBarcodeReaderForm : Form
    {
        private struct Device
        {
            public int Index;
            public string Name;
            public override string ToString()
            {
                return Name;
            }
        }

        private readonly CameraDevices camDevices;
        private Bitmap currentBitmapForDecoding;
        private readonly Thread decodingThread;
        private Result currentResult;
        private readonly Pen resultRectPen;
        private int sleep = 100;
        // Filter...Please apply carefully...
        private ContrastStretch csFilter = new ContrastStretch();
        private HistogramEqualization heFilter = new HistogramEqualization();
        private Threshold tsFilter = new Threshold(100);
        private Grayscale gsFilter = new Grayscale(0.2125, 0.7154, 0.0721);

        /// <summary>
        /// constructor imBarcodeReaderForm()
        /// </summary>
        public imBarcodeReaderForm()
        {
            InitializeComponent();

            camDevices = new CameraDevices();

            decodingThread = new Thread(DecodeBarcode);
            decodingThread.Start();
            btnThreshold.BackColor = Color.Red;
            pictureBox1.Paint += pictureBox1_Paint;
            resultRectPen = new Pen(Color.Green, 10);
        }

        void pictureBox1_Paint(object sender, PaintEventArgs e)
        {
        }
        /// <summary>
        /// OnLoad()
        /// </summary>
        /// <param name="e"></param>
        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);
            LoadDevicesToCombobox();
        }
        /// <summary>
        /// OnClosing()
        /// </summary>
        /// <param name="e"></param>
        protected override void OnClosing(System.ComponentModel.CancelEventArgs e)
        {
            base.OnClosing(e);
            if (!e.Cancel)
            {
                decodingThread.Abort();
                if (camDevices.Current != null)
                {
                    camDevices.Current.NewFrame -= Current_NewFrame;
                    if (camDevices.Current.IsRunning)
                    {
                        camDevices.Current.SignalToStop();
                    }
                }
            }
        }

        private void LoadDevicesToCombobox()
        {
            cmbDevice.Items.Clear();
            for (var index = 0; index < camDevices.Devices.Count; index++)
            {
                cmbDevice.Items.Add(new Device { Index = index, Name = camDevices.Devices[index].Name });
            }
            cmbDevice.SelectedIndex = camDevices.Devices.Count - 1;
        }

        private void cmbDevice_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (camDevices.Current != null)
            {
                camDevices.Current.NewFrame -= Current_NewFrame;
                if (camDevices.Current.IsRunning)
                {
                    camDevices.Current.SignalToStop();
                }
            }

            camDevices.SelectCamera(((Device)(cmbDevice.SelectedItem)).Index);
            txtContent.Text = txtBarcodeFormat.Text = "";
            Size s;
            int ci = 0;
            for (var i = 1; i < camDevices.Current.VideoCapabilities.Length; i++)
            {
                s = camDevices.Current.VideoCapabilities[i].FrameSize;
                txtContent.Text += s.Width + " x " + s.Height + "\r\n";
                if (s.Width < 1920 && s.Width > camDevices.Current.VideoCapabilities[ci].FrameSize.Width) ci = i;
            }
            camDevices.Current.VideoResolution = camDevices.Current.VideoCapabilities[ci];
            txtContent.Text += "======================================\r\n";
            s = camDevices.Current.VideoResolution.FrameSize;
            txtBarcodeFormat.Text += s.Width + " x " + s.Height + " x " +
                camDevices.Current.VideoResolution.AverageFrameRate;

            if (camDevices.Current.VideoResolution.AverageFrameRate > 0)
                sleep = 1000 / camDevices.Current.VideoResolution.AverageFrameRate;
            else sleep = 100;
            camDevices.Current.NewFrame += Current_NewFrame;
            camDevices.Current.Start();
        }

        private void Current_NewFrame(object sender, NewFrameEventArgs eventArgs)
        {
            if (IsDisposed)
            {
                return;
            }

            try
            {
                if (currentBitmapForDecoding == null)
                {
                    currentBitmapForDecoding = (Bitmap)eventArgs.Frame.Clone();
                }
            }
            catch (ObjectDisposedException)
            {
                // not sure, why....
            }
        }

        private void ShowFrame(Bitmap frame)
        {
            System.Drawing.Image newImage = new Bitmap(640, 480);
            using (Graphics graphicsHandle = Graphics.FromImage(newImage))
            {
                graphicsHandle.InterpolationMode = InterpolationMode.Low;
                graphicsHandle.DrawImage(frame, 0, 0, 640, 480);
            }
            pictureBox1.Image = newImage;
        }

        private void DecodeBarcode()
        {
            var reader = new BarcodeReader();
            while (true)
            {
                if (currentBitmapForDecoding != null)
                {
                    currentBitmapForDecoding = gsFilter.Apply(currentBitmapForDecoding);
                    if (cbContrast.Checked) csFilter.ApplyInPlace(currentBitmapForDecoding);
                    if (cbHistogram.Checked)
                        heFilter.ApplyInPlace(currentBitmapForDecoding);
                    if (btnThreshold.BackColor == Color.Green)
                    {
                        tsFilter.ApplyInPlace(currentBitmapForDecoding);
                    }
                    Invoke(new Action<Bitmap>(ShowFrame), currentBitmapForDecoding.Clone());
                    var result = reader.Decode(currentBitmapForDecoding);
                    if (result != null)
                    {
                        Invoke(new Action<Result>(ShowResult), result);
                    }
                    currentBitmapForDecoding.Dispose();
                    currentBitmapForDecoding = null;
                }
                Thread.Sleep(sleep);
            }
        }

        private void ShowResult(Result result)
        {
            currentResult = result;
            txtBarcodeFormat.Text = result.BarcodeFormat.ToString();
            txtContent.Text = result.Text;
        }

        private void btnThreshold_Click(object sender, EventArgs e)
        {
            if (btnThreshold.BackColor == Color.Red)
            {
                btnThreshold.BackColor = Color.Green;
            }
            else
            {
                btnThreshold.BackColor = Color.Red;
            }
        }

        private void tbThreshold_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                try
                {
                    int ts = Convert.ToInt32(tbThreshold.Text);
                    if (ts > 0 && ts < 255)
                        tsFilter.ThresholdValue = ts;
                    else
                    {
                        tbThreshold.Text = "" + tsFilter.ThresholdValue;
                    }
                }
                catch { }
            }
        }
       private void btnCamera_Click(object sender, EventArgs e)
        {
            camDevices.Current.DisplayPropertyPage(IntPtr.Zero);
        }

        private void cbClear_Click(object sender, EventArgs e)
        {
            txtContent.Text = "";
            txtBarcodeFormat.Text = "";
        }
    }
}
