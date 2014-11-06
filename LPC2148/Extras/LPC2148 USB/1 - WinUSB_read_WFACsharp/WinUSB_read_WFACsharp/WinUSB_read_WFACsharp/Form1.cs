using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;


namespace WinUSB_read_WFACsharp
{
    public partial class Form1 : Form
    {
        WinUsbAbs UsbDeviceAbs;
        internal Form1 myForm1;
        private const String WINUSB_GUID_STRING = "{DE2E6670-494F-11E1-BF7E-BD4E4824019B}";

        private WinUsbDevice myWinUsbDevice = new WinUsbDevice();

        String receivedtext = "";
        
        //private delegate void ReadFromDeviceDelegate
        //    (Byte pipeID,
        //    UInt32 bufferLength,
        //    ref Byte[] buffer,
        //    ref UInt32 lengthTransfered,
        //    ref Boolean sucess);

        
        public Form1()
        {
            InitializeComponent();
            myForm1 = this;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            bool b_result = false;

            b_result = UsbDeviceAbs.FindAndEnumerateDevice();
            if (b_result == true)
            {
                MessageBox.Show("Device's connected!");
                
            }
            else
            {
                MessageBox.Show("Connection failed!");
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            UsbDeviceAbs = new WinUsbAbs(WINUSB_GUID_STRING, myForm1);
        }

        //Botão usado para pegar o dado na USB
        private void button2_Click(object sender, EventArgs e)
        {
            // Se o dispositivo não foi detectado, foi removido, ou uma tentativa de acessálo anteriormente expirou
            // procurar o dispositivo novamente
            if (UsbDeviceAbs.DeviceIsConnected() == false)
            {
                UsbDeviceAbs.FindAndEnumerateDevice();

                UsbDeviceAbs.ReadDataViaBulkTransfer(this.GetReceivedBulkData);
            }
            else
                UsbDeviceAbs.ReadDataViaBulkTransfer(this.GetReceivedBulkData);

            
        }

        private void writeTextbox(object sender, EventArgs e)
        {
            //textBox1.Text = receivedtext;
            textBox1.AppendText(receivedtext);
        }

        //Esta função será chamada quando a tentativa de ler um dado no endpoint bulk for válida
        public void GetReceivedBulkData(IAsyncResult ar)
        {
            UInt32 bytesRead = 0;
            Byte[] receivedDataBuffer;
            
            Boolean success = false;

            System.Text.ASCIIEncoding myEnc = new System.Text.ASCIIEncoding();

            receivedDataBuffer = null;

            //Recebe o delegate usado para chamar ReadViaBulkTransfer
            //Para obter os dados que foram lidos da usb
            UsbDeviceAbs.deleg = ((WinUsbAbs.ReadFromDeviceDelegate)(ar.AsyncState));

            //Recebe os dados que foram lidos no endpoint bulk pelo método ReadViaBulkTransfer
            UsbDeviceAbs.deleg.EndInvoke(ref receivedDataBuffer, ref bytesRead, ref success, ar);

            //Converte byte para string em ascii
            receivedtext = myEnc.GetString(receivedDataBuffer);

            //Invoca o metodo para plotar o dado no textbox
            //Aqui poderia ser chamado a função que plota dado no zedgraph
            this.Invoke(new EventHandler(writeTextbox));

            
        }

        //Pegando dado na usb usando o TIMER
        private void timer1_Tick(object sender, EventArgs e)
        {
            // Se o dispositivo não foi detectado, foi removido, ou uma tentativa de acessálo anteriormente expirou
            // procurar o dispositivo novamente
            if (UsbDeviceAbs.DeviceIsConnected() == false)
            {
                UsbDeviceAbs.FindAndEnumerateDevice();

                UsbDeviceAbs.ReadDataViaBulkTransfer(this.GetReceivedBulkData);
            }
            else
                UsbDeviceAbs.ReadDataViaBulkTransfer(this.GetReceivedBulkData);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (timer1.Enabled == true)
                timer1.Enabled = false;
            else
                timer1.Enabled = true;
        }


    }
}
