using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace WebcamSecurity
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            // 底下這兩行目的是讓執行緒中也可以存取 MainForm 方法，主要是 CameraMonitor.cs 中呼叫了 log()
            mf = new MainForm();
            Control.CheckForIllegalCrossThreadCalls = false; 
            Application.Run(mf);
        }
        public static MainForm mf = null;
    }
}
