using System;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;

namespace REGDI.net5
{
    public partial class Form1 : Form
    {
        Timer t;

        static int f = 0;
        static Random rnd = new Random();
        static int size = 6;
        static int columnGrid = 99;
        public Form1()
        {
            InitializeComponent();
        }

        private void T_Tick(object sender, EventArgs e)
        {
            this.Invoke(new Action(() =>
            {
                this.Text = "Creating controls....";
                Stopwatch sp = Stopwatch.StartNew();
                Controls.Clear();
                for (int i = 0; i < columnGrid; i++)
                {
                    for (int j = 0; j < columnGrid + 1; j++) // 100 controls instead of 99
                    {
                        Control b = new Control();
                        b.Height = size;
                        b.Width = size;
                        b.Padding = Padding.Empty;
                        b.Margin = Padding.Empty;
                        //Random may slow down a bit, but it's for visual
                        //To test controls generation speed properly, please set a fixed color.
                        b.BackColor = Color.FromArgb(rnd.Next(0, 256), rnd.Next(0, 256), rnd.Next(0, 256));
                        b.Location = new Point(i * size, j * size);
                        Controls.Add(b);
                    }
                }

                label1.Visible = false;
                sp.Stop();
                this.Text = "Controls created in: " + sp.Elapsed.ToString("mm\\:ss\\.fff");
            }));
            t.Stop();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            t = new Timer();
            t.Interval = 2000;
            t.Tick += T_Tick;
            t.Start();
        }

    }
}
