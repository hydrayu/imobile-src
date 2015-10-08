using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Win32;

namespace ComputerName
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                RegistryKey key = Registry.LocalMachine;

                string activeComputerName = "SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ActiveComputerName";
                RegistryKey activeCmpName = key.CreateSubKey(activeComputerName);
                string computerName = "SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ComputerName";
                RegistryKey cmpName = key.CreateSubKey(computerName);
                string _hostName = "SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters\\";
                RegistryKey hostName = key.CreateSubKey(_hostName);
                string myComputerName = activeCmpName.GetValue("ComputerName").ToString();

                string newName = string.Concat(args);
                if (newName.Length > 0)
                {
                    Console.WriteLine(String.Format("The Origin Computer Name : {0}\r\n", myComputerName));
                    Console.WriteLine("You want to set computer as " + newName);

                    activeCmpName.SetValue("ComputerName", newName);
                    cmpName.SetValue("ComputerName", newName);
                    hostName.SetValue("Hostname", newName);
                    hostName.SetValue("NV Hostname", newName);
                }
                else
                    Console.WriteLine(String.Format("Computer Name {0}\r\n", myComputerName));

                activeCmpName.Close();
                cmpName.Close();
                hostName.Close();
            }
            catch (Exception e)
            {
                Console.WriteLine("You have no PERMISSION to set the computer name\r\n " + e.ToString());
            }
        }
    }
}
