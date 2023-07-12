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

        /*
         * This 'ShowForm' method is specifically designed to be used by a C++ application
         * that wishes to instantiate and interact with a C# form.
         * It includes the same initialization routines found in the Main method.
         */
        public static void ShowForm()
        {
            // Enables visual styles for the application.
            Application.EnableVisualStyles();

            // Sets whether the application is compatible with text rendering based on GDI+.
            Application.SetCompatibleTextRenderingDefault(false);

            // Create a new instance of Form1.
            Form1 form = new Form1();

            // Show the form without blocking the current thread.
            form.Show();

            // Begins running a standard application message loop on the current thread, without a form.
            Application.Run();
        }

    }
}
