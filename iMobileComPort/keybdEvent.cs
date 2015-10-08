using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace iMobileComPort
{
    internal static class keybdEvent
    {
        [DllImport("user32.dll", SetLastError = true)]
        static extern void keybd_event(byte bVk, byte bScan, int dwFlags, int dwExtraInfo);

        internal const int KEYEVENTF_EXTENDEDKEY = 0x0001; //Key down flag
        internal const int KEYEVENTF_KEYUP = 0x0002; //Key up flag
        internal const int VK_RETURN = 0x0D;
        internal const int VK_SHIFT = 0x10;
        internal const int VK_CONTROL = 0x11;
        internal const int VK_MENU = 0x12;
        internal static void GenerateKey(byte vk, byte scan)
        {
            // May be rewrite to use
            // http://msdn.microsoft.com/en-us/library/system.windows.forms.sendkeys.aspx
            // http://msdn.microsoft.com/en-us/library/windows/desktop/ms646304(v=vs.85).aspx
            // http://www.codeproject.com/Articles/7305/Keyboard-Events-Simulation-using-keybd_event-funct
            if (vk >= (byte)'A' && vk <= (byte)'Z')
            {
                keybd_event(VK_SHIFT, 0, 0, 0);
                keybd_event(vk, scan, 0, 0);
                keybd_event(vk, scan, KEYEVENTF_KEYUP, 0);
                keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
            }
            else
            {
                keybd_event(vk, scan, 0, 0);
                keybd_event(vk, scan, KEYEVENTF_KEYUP, 0);
            }
        }

        internal static void sendToKeyboard(string msg)
        {
            for (int i=0; i<msg.Length && i<256; i++) {
                    GenerateKey((byte)msg[i], 0);
            }
            GenerateKey(VK_RETURN, 0);
        }
    }
}
