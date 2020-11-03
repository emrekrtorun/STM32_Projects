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

namespace Final_Project_App
{
    public partial class Form1 : Form
    {
        private string data;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            foreach (string portlar in SerialPort.GetPortNames())
            {
                comboBox1.Items.Add(portlar);
            }

           
            serialPort1.DataReceived += new SerialDataReceivedEventHandler(SerialPort1_DataReceived); //DataReceived eventini oluşturma
            pictureBox1.Enabled = false;
        }
        private void SerialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            data = serialPort1.ReadLine();                      //Veriyi al
            this.Invoke(new EventHandler(displayData_event));


        }
        private void displayData_event(object sender, EventArgs e)
        {
           
            textBox1.Text += DateTime.Now.ToString() + "           " + data + "\n"; //Gelen veriyi textBox içine güncel zaman ile ekle
            if (data.ToString() == "1")
            {
                pictureBox1.Enabled = true;
            }
            else
            {
                pictureBox1.Enabled = false;

            }
        }

        
        private void Connect_But_Click(object sender, EventArgs e)
        {
            try
            {
                serialPort1.PortName = comboBox1.Text;
                serialPort1.BaudRate = Convert.ToInt32(comboBox2.Text);
                serialPort1.Open();

                if (serialPort1.IsOpen)
                {
                    
                    MessageBox.Show("Connection Succesfull");
                }
            }
            catch
            {
                MessageBox.Show("Connection Unsuccesfull");
            }

        }
        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (serialPort1.IsOpen) serialPort1.Close();    //Seri port açıksa kapat
        }
    }
}
