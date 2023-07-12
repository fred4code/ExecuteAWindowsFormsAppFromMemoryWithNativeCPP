/*
This .cs file is part of the Visual Studio Project WindowsFormsHelloWorldApp
WindowsFormsHelloWorldApp Visual Studio Project project is part of the Visual Studio Solution ExecuteAWindowsFormsAppFromMemoryWithNativeCPP
This .cs file, the project mentioned and the Visual studio solution mentioned are licensed under the MIT License. See http://opensource.org/licenses/MIT for more information.

Copyright(c) 2023 fred4code
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsHelloWorldApp
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
            Application.Run(new Form1());
        }

        public static void ShowForm()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Form1 form = new Form1();
            form.Show();
            Application.Run();
        }

    }
}
