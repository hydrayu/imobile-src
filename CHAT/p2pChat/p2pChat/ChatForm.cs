using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

// add for CHAT
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Net.NetworkInformation;

namespace p2pChat
{
    public partial class ChatForm : Form
    {
        delegate void AddMessage(string message);
        string userName;
        const int port = 54545;
        const string broadcastAddress = "255.255.255.255";
        // List<IPAddress> targetIPs = new List<IPAddress>();

        UdpClient receivingClient;
        UdpClient sendingClient;

        Thread receivingThread;

        private void log(string msg)
        {
            rtbChat.Text += msg;
            rtbChat.Text += "\r\n";
        }

        private void log(string format_msg, params string[] args)
        {
            log(string.Format(format_msg, args));
        }

        //private void findNICs()
        //{
        //    NetworkInterface[] adapters  = NetworkInterface.GetAllNetworkInterfaces();
        //    foreach (NetworkInterface adapter in adapters)
        //    {
        //        if (adapter.OperationalStatus != OperationalStatus.Up ||
        //            adapter.NetworkInterfaceType == NetworkInterfaceType.Loopback)
        //        {
        //            continue;
        //        }
        //        UnicastIPAddressInformationCollection uniCast = adapter.GetIPProperties().UnicastAddresses;
        //        if (uniCast.Count > 0)
        //        {
        //            foreach (UnicastIPAddressInformation uni in uniCast)
        //            {
        //                if (uni.Address.AddressFamily == AddressFamily.InterNetworkV6)
        //                    continue;
        //                targetIPs.Add(uni.Address);
        //            }
        //        }
        //    }
        //}

        public ChatForm()
        {
            InitializeComponent();
        }

        private void ChatForm_Load(object sender, EventArgs e)
        {
            this.Hide();
            this.ControlBox = false;
            this.Text = "";

            using (LoginForm loginForm = new LoginForm())
            {
                loginForm.ControlBox = false;
                loginForm.Text = "";

                loginForm.ShowDialog();
                if (loginForm.UserName == "") this.Close();
                else
                {
                    userName = loginForm.UserName;
                    this.WindowState = FormWindowState.Maximized;
                    this.Show();
                }
            }

            tbSend.Focus();
            InitializeSender();
            InitializeReceiver();
            sendCmd("/login", userName, "*");
            // findNICs();
        }

        private void InitializeSender()
        {
            sendingClient = new UdpClient(broadcastAddress, port);
            sendingClient.EnableBroadcast = true;
        }

        private void InitializeReceiver()
        {
            receivingClient = new UdpClient(port);

            ThreadStart start = new ThreadStart(Receiver);
            receivingThread = new Thread(start);
            receivingThread.IsBackground = true;
            receivingThread.Start();
        }

        private void Receiver()
        {
            IPEndPoint endPoint = new IPEndPoint(IPAddress.Any, port);
            AddMessage messageDelegate = MessageReceived;

            while (true)
            {
                byte[] data = receivingClient.Receive(ref endPoint);
                string message = Encoding.Unicode.GetString(data);
                Invoke(messageDelegate, message);
            }
        }

        private void MessageReceived(string message)
        {
            char lead = message[0];
            if (lead == '/')
            {
                parseReceiveCmd(message);
            }
            else if (lead == '#')
                log(message.Substring(1));
        }

        private void parseReceiveCmd(string cmd)
        {
            string[] elms = cmd.Split(' ');
            switch (elms[0])
            {
                case "/login":
                    log(string.Format("[sys]{0}@{1:yyyyMMdd hh:mm:ss} login", elms[1], DateTime.Now));
                    break;
                default:
                    log(cmd);
                    break;
            }
        }

        private void tbSend_KeyPress(object sender, KeyPressEventArgs e)
        {
            int kc = (int)e.KeyChar;
            if (kc == 13) // Enter
            {
                btnSend_Click(sender, e);
            }
        }

        private void say(string msg)
        {
            string toSend = string.Format("#{0}@{1:yyyyMMdd hh:mm:ss} Said: {2}", userName, DateTime.Now, msg);
            byte[] data = Encoding.Unicode.GetBytes(toSend);

            try
            {
                sendingClient.Send(data, data.Length);
            }
            catch (SocketException m)
            {
                log("Error toSend({0})...{1}\r\n", toSend, m.Message);
            }
        }

        private void sendCmd(string cmd, string from, string to, params string[] args)
        {
            byte[] data;
            if (args.Length == 0) 
                data = Encoding.Unicode.GetBytes(string.Format("{0} {1} {2}", cmd, from, to));
            else {
                data = Encoding.Unicode.GetBytes(string.Format("{0} {1} {2} {3}", cmd, from, to, string.Join(" ", args)));
            }

            try
            {
                sendingClient.Send(data, data.Length);
            }
            catch (SocketException m)
            {
                log("Error sendCmd({0})...{1}\r\n", cmd, m.Message);
            }
        }

        private void parseCmd(string cmd)
        {
            string[] elms = cmd.Split(' ');
            switch (elms[0])
            {
                case "/smile":
                    say("哈哈哈");
                    break;
                default:
                    sendCmd(cmd, userName, "*");
                    break;
            }
        }

        private void btnSend_Click(object sender, EventArgs e)
        {
            string msg = tbSend.Text.Trim();
            tbSend.Text = "";

            if (!string.IsNullOrEmpty(msg))
            {
                if (msg[0] == '/')
                {
                    parseCmd(msg);
                }
                else
                {
                    say(msg);
                }
            }

            tbSend.Focus();
        }

        private void btnQUIT_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void ChatForm_DoubleClick(object sender, EventArgs e)
        {
            this.WindowState = FormWindowState.Normal;
        }

        private void rtbChat_Enter(object sender, EventArgs e)
        {
            tbSend.Focus();
        }

        private void ChatForm_Enter(object sender, EventArgs e)
        {
            tbSend.Focus();
        }
    }
}
