using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace DC_Motor_Control_App
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            int trackBar1Deger = trackBar1.Value;
            label3.Text = Convert.ToString(trackBar1.Value);


            if (trackBar1Deger == 1)
            {
                serialPort1.Write("1");
            }

            else if (trackBar1Deger == 2)
            {
                serialPort1.Write("2");
            }

            else if (trackBar1Deger == 3)
            {
                serialPort1.Write("3");
            }

            else if (trackBar1Deger == 4)
            {
                serialPort1.Write("4");
            }
            else
            {
                serialPort1.Write("0");
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            foreach (string portlar in SerialPort.GetPortNames())
            {
                comboBox1.Items.Add(portlar);
            }

            groupBox2.Enabled = false;
            groupBox3.Enabled = false;
        
            

        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                serialPort1.PortName = comboBox1.Text;
                serialPort1.BaudRate = Convert.ToInt32(comboBox2.Text);
                serialPort1.Open();

                if (serialPort1.IsOpen)
                {
                    groupBox2.Enabled = true;
                    groupBox3.Enabled = true;
                    MessageBox.Show("Connection Succesfull");
                }
            }
            catch 
            {
                MessageBox.Show("Connection Unsuccesfull");
            }


        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            trackBar1.Value = 0;
            serialPort1.Write("a");
        }

        private void trackBar2_Scroll(object sender, EventArgs e)
        {
            label4.Text = Convert.ToString(trackBar2.Value+5);
            int trackBar2Deger = trackBar2.Value;

            if(trackBar2Deger+5 == 5)
            {
                serialPort1.Write("5");
            }
            else if (trackBar2Deger+5 == 6)
            {
                serialPort1.Write("6");
            }
            else if (trackBar2Deger + 5 == 7)
            {
                serialPort1.Write("7");
            }
            else if (trackBar2Deger + 5 == 8)
            {
                serialPort1.Write("8");
            }
            else if (trackBar2Deger + 5 == 9)
            {
                serialPort1.Write("9");
            }


        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            trackBar1.Value = 0;
            serialPort1.Write("b");
        }

        private void radioButton4_CheckedChanged(object sender, EventArgs e)
        {
            trackBar2.Value = 0;
            serialPort1.Write("d");

        }

        private void radioButton3_CheckedChanged(object sender, EventArgs e)
        {
            trackBar2.Value = 0;
            serialPort1.Write("c");
        }

        private void groupBox4_Enter(object sender, EventArgs e)
        {

        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }
    }
}
   