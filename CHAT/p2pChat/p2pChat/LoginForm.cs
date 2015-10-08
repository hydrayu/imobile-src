using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace p2pChat
{
    public partial class LoginForm : Form
    {
        string userName = "";

        public string UserName
        {
            get { return userName; }
        }

        public LoginForm()
        {
            InitializeComponent();
            tbUserName.Focus();
        }

        private void LoginForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            userName = "";
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            userName = tbUserName.Text.Trim();
            if (string.IsNullOrEmpty(userName))
            {
                MessageBox.Show("Please enter a user name(up to 32 char max)");
                tbUserName.Focus();
                return;
            }
            this.FormClosing -= LoginForm_FormClosing;
            this.Close();
        }

        private void btnQUIT_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void tbUserName_KeyPress(object sender, KeyPressEventArgs e)
        {
            int kc = (int)e.KeyChar;
            if (kc == 13) // Enter
            {
                btnOK_Click(sender, e);
            }
        }
    }
}
