using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Win32;
namespace WindowsAutoLogin
{
    class RegistryWriter
    {
        /// <summary>
        /// enable automatic login to windows , for the provided user name
        /// </summary>
        /// <param name="usr">string the user name</param>
        /// <param name="pwd">string the password</param>

        public void WriteDefaultLogin(string usr, string pwd)
        {
            RegistryKey rekey = Registry.LocalMachine.CreateSubKey("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon");
            if (rekey == null)
                System.Windows.Forms.MessageBox.Show("There has been an erro while trying to write to windows registry");
            else
            {
                rekey.SetValue("AutoAdminLogon", "1");
                rekey.SetValue("DefaultUserName", usr);
                rekey.SetValue("DefaultPassword", pwd);
            }
            rekey.Close();
        }

        public void RemoveDefaultLogin()
        {
            RegistryKey rekey = Registry.LocalMachine.CreateSubKey("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon");
            if (rekey == null)
                System.Windows.Forms.MessageBox.Show("There has been an erro while trying to write to windows registry");
            else
            {
                rekey.DeleteValue("DefaultUserName", false);
                rekey.DeleteValue("DefaultPassword", false);
                rekey.DeleteValue("AutoAdminLogon", false);
            }
            rekey.Close();
        }

        /// <summary>
        /// verifies if theres a default login configuration present in the system
        /// </summary>
        public bool VerifyDefaultLoginEntries()
        {
            RegistryKey rekey = Registry.LocalMachine.CreateSubKey("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon");
            if (rekey.GetValue("DefaultUserName").ToString().Equals(null))
                return false;
            else if (rekey.GetValue("DefaultPassword").ToString().Equals(null))
                return false;
            else
                return Convert.ToBoolean(rekey.GetValue("AutoAdminLogin"));
        }
    }
}

