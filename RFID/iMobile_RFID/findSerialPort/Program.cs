using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Management;

namespace findSerialPort
{
    class Program
    {
        static void Main(string[] args)
        {
            String path = @"\\" + Environment.MachineName + @"\root\cimv2";
            String queryArea = "Win32_PnPEntity";
            ManagementClass managementClass = new ManagementClass(path + ":" + queryArea);
            ManagementObjectCollection instances = managementClass.GetInstances();
            if (instances == null)
            {
                return;
            }
            string fCaption = "", fDeviceID = "";
            int verbose = 0;
            if (args.Count() > 0)
            {
                for (int i = 0; i < args.Count(); i++)
                {
                    string arg = args[i];
                    switch (arg)
                    {
                        case "-h":
                            Console.WriteLine("arguments:\n\t" +
                                "-h : this usage\n\t" +
                                "-v : print out WMI message\n\t" +
                                "-c : filter with Device Caption substring(ie. Description)\n\t" +
                                "-d : filter with DeviceID string");
                            break;
                        case "-v":
                            verbose = 1;
                            break;
                        case "-c":
                            if (i < args.Count() - 1)
                                fCaption = args[i + 1]; // "Prolific"; // "CP210x";
                            break;
                        case "-d":
                            // DeviceID == DeviceManager 上的 Device Instance Path
                            // USB\VID_10C4&PID_EA60\0001
                            if (i < args.Count() - 1)
                            {
                                fDeviceID = args[i + 1]; // "Prolific"; // "CP210x";
                            }
                            break;
                    }
                }
            }
            else
            {
                fDeviceID = @"USB\VID_10C4&PID_EA60\0001";
            }
            foreach (ManagementObject m in instances)
            {
                if (m["Caption"].ToString().Contains("(COM") && m["Caption"].ToString().Contains(fCaption))
                {
                    if (m["DeviceID"].ToString().Contains(fDeviceID))
                    {
                        if (verbose == 1)
                        {
                            foreach (PropertyData pd in m.Properties)
                            {
                                if (null != pd.Value)
                                {
                                    if (pd.Value.GetType().ToString() == "System.String[]")
                                    {
                                        Console.WriteLine("{0} =", pd.Name);
                                        foreach (string s in (string[])pd.Value)
                                            Console.WriteLine("\t{0}", s);
                                    }
                                    else
                                        Console.WriteLine("{0} = {1}", pd.Name, pd.Value);
                                }
                            }
                        }
                        string description = m["Caption"].ToString();
                        int p, q;
                        string mPort;
                        p = description.IndexOf("(COM");
                        q = description.IndexOf(")", p);
                        mPort = description.Substring(p + 1, q - p - 1);
                        Environment.SetEnvironmentVariable("SERIAL_RFID", mPort, EnvironmentVariableTarget.User);
                        Console.WriteLine("{0}", mPort);
                    }
                }
            }
        }
    }
}
