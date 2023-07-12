/*
This .cs file is part of the Visual Studio Project WindowsFormsHelloWorldApp
WindowsFormsHelloWorldApp Visual Studio Project project is part of the Visual Studio Solution ExecuteAWindowsFormsAppFromMemoryWithNativeCPP
This .cs file, the project mentioned and the Visual studio solution mentioned are licensed under the MIT License. See http://opensource.org/licenses/MIT for more information.

Copyright(c) 2023 fred4code
*/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.IO.MemoryMappedFiles;

namespace WindowsFormsHelloWorldApp
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void _button_pressMe_Click(object sender, EventArgs e)
        {
            this._button_pressMe.Text = "Hello World! :D \n I am loaded from a string with Native C++!";
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            using (MemoryMappedFile mmf = MemoryMappedFile.CreateOrOpen("MySharedMemory", 1))
            {
                using (MemoryMappedViewAccessor accessor = mmf.CreateViewAccessor())
                {
                    accessor.Write(0, (byte)0);
                }
            }
        }
    }
}
