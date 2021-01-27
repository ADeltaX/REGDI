using System;
using System.Windows.Forms;

namespace REGDI.net5
{
    static class Program
    {
        /// <summary>
        ///  The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            var form = new Form1();
            form.Width = 609;
            form.Height = 639;

            Application.Run(form);
        }
    }
}
