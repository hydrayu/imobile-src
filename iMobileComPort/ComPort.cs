using System;
using System.Collections.Generic;
using System.Text;

using System.IO;
using System.IO.Ports;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Threading;
using System.Runtime.InteropServices;
using System.Management;
using System.Text.RegularExpressions;

namespace iMobileComPort
{
    public class ComPort
    {
        public SerialPort Port;
        private Terminal term;
        public string dev = "UNKNOWN";

        public ComPort(WithEvents ev, string PortName, Terminal term) 
        {
            this.Port = new SerialPort(ev, PortName);
            this.term = term;
        }

        /// <summary>
        /// Controls opening/closing the port.
        /// returns 0 for close, 1 for open, -1 for error
        /// </summary>
        public int PortControl(int active=0) // 1 try to open
        { 
            if (active == 1 || !this.Port.IsOpen)
            {
                int idx = Convert.ToInt32(this.Port.Cnfg.PortName.Substring(3, this.Port.Cnfg.PortName.Length - 4));
                if (idx <= 0 || this.Port.Open(idx) == false)
                {
                    return -1;
                }
            }
            else
            {
                if (this.Port.IsOpen)
                {
                    this.Port.Close();
                }
                this.Port.Signals();
                return 0;
            }
            return 1;
        }
    }
}
