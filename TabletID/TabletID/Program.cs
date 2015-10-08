using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using WMIHelper;

namespace TabletID
{
    class Program
    {
        String MachineName = Environment.MachineName;

        static void Main(string[] args)
        {
            int ordered = 0;
            int brief = 0;
            int netOnly = 0;
            int hdOnly = 0;

            if (args.Length == 0)
            {
                Console.WriteLine("arguments:\n\t"+
                    "-o #: get assigned instance\n\t"+
                    "-b: brief\n\t-n: network adapter only\n\t"+
                    "-d: disk only\n\t-a: all\n\t" +
                    "-h: this usage\n");
            }
            else
            {
                foreach (String arg in args)
                {
                    switch (arg)
                    {
                        case "-o": ordered = 1; break;
                        case "-b": brief = 1; break;
                        case "-n":
                            netOnly = 1;
                            hdOnly = 0;
                            break;
                        case "-d":
                            hdOnly = 1;
                            netOnly = 0;
                            break;
                        case "-a":
                            hdOnly = 0;
                            netOnly = 0;
                            ordered = 0;
                            break;
                        case "-h":
                            Console.WriteLine("arguments:\n\t" +
                                "-o #: get assigned instance\n\t" +
                                "-b: brief\n\t-n: network adapter only\n\t" +
                                "-d: disk only\n\t-a: all\n\t"+
                                "-h: this usage\n");
                            Environment.Exit(0);
                            break;
                        default:
                            if (ordered > 0 && !Int32.TryParse(arg, out ordered)) 
                                ordered = 0;
                            break;
                    }
                }
            }

            //BIOS biosOB;
            //OS osOB;
            //Serial[] serial;
            NetworkAdapter[] networkAdapterOB;
            LogicalDisk[] hdOB;

            //biosOB = WMIHelper.WMIHelper.FillBiosInformation(Environment.MachineName, "", "", "Win32_BIOS");
            //osOB = WMIHelper.WMIHelper.FillOSInformation(Environment.MachineName, "", "", "Win32_OperatingSystem");
            networkAdapterOB = WMIHelper.WMIHelper.FillNetworkAdapter(Environment.MachineName, "", "", "Win32_NetworkAdapterConfiguration");
            hdOB = WMIHelper.WMIHelper.FillLogicalDisks(Environment.MachineName, "", "", "Win32_LogicalDIsk");
            //serial = WMIHelper.WMIHelper.FillSerial(Environment.MachineName, "", "", "Win32_PnPEntity", "CP210x");
            //for (int i = 0; i < serial.Count(); ++i)
            //    Console.WriteLine("{0}: {1}", serial[i].port, serial[i].Description);
            //Console.WriteLine("BIOS # {0}", biosOB.SerialNumber);
            //Console.WriteLine("OS # {0}", osOB.SerialNumber);
            if (hdOnly == 0)
            {
                for (int i = 0; i < networkAdapterOB.Count(); ++i)
                {
                    NetworkAdapter na = networkAdapterOB[i];
                    if (na.MACAddress.Length > 0)
                    {
                        if (ordered > 1) ordered--;
                        else {
                            if (brief == 1)
                                Console.WriteLine("{0}", na.MACAddress);
                            else
                                Console.WriteLine("{0} # Network Interface({1})", na.MACAddress, na.Description);
                            if (ordered == 1)
                            {
                                netOnly = 1;
                                break;
                            }
                        }
                    }
                }
            }
            if (netOnly == 0)
            {
                for (int i = 0; i < hdOB.Count(); ++i)
                {
                    LogicalDisk ld = hdOB[i];
                    if (ld.VolumeSerialNumber.Length > 0)
                    {
                        if (ordered > 1) ordered--;
                        else
                        {
                            if (brief == 1)
                                Console.WriteLine("{0}", ld.VolumeSerialNumber);
                            else
                                Console.WriteLine("{0} # Harddisk({1})", ld.VolumeSerialNumber, ld.Name);
                            if (ordered == 1)
                            {
                                netOnly = 1;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}
