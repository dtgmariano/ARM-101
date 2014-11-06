using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
//using WinUSBDevice;
//using WinUSBDeviceAPI;
using System.Windows.Forms;

namespace WinUSB_read_WFACsharp
{
    class WinUsbAbs
    {
        private IntPtr deviceNotificationHandle = new IntPtr();
        private WinUsbDevice winusbevice = new WinUsbDevice();
        private DeviceManagement deviceManagement = new DeviceManagement();
        private Boolean DeviceDetected = false;
        private String DevicePathName;
        private Form form;
        private String GUID_STRING;
        System.Guid winUsbGUID;

        public delegate void ReadFromDeviceDelegate
            (Byte pipeID,
            UInt32 bufferLength,
            ref Byte[] buffer,
            ref UInt32 lengthTransfered,
            ref Boolean sucess);

        public delegate void mydelegate(IAsyncResult arr);

        public ReadFromDeviceDelegate deleg;

        public WinUsbAbs(string guid, Form frm)
        {
            form = frm;
            GUID_STRING = guid;

            winusbevice = new WinUsbDevice();

            winUsbGUID = new System.Guid(GUID_STRING);

            //deviceManagement.RegisterForDeviceNotifications(DevicePathName,
            //                                                  form.Handle,
            //                                                  winUsbGUID,
            //                                                  ref deviceNotificationHandle);
        }

        public Boolean FindAndEnumerateDevice()
        {
            Boolean deviceFound;
            String devicePathName = "";
            //Boolean lastDevice;
            Boolean success;

            try
            {
                if (DeviceDetected == false)
                {
                    //Cria um objeto GUID apartir do GUID presente no .inf 
                    System.Guid winUsbGUID = new System.Guid(GUID_STRING);

                    //Obtem o pathname dos dispositivos conectados nesta interface a partir do guid
                    deviceFound = deviceManagement.FindDeviceFromGuid(winUsbGUID, ref devicePathName);

                    if (deviceFound == true)
                    {
                        //Pega um file handle para manipular o dispositivo conectado na interface
                        success = winusbevice.GetDeviceHandle(devicePathName);

                        if (success == true)
                        {
                            //variavel local para identificar se o dispositivo está conectado ou não
                            DeviceDetected = true;

                            //Salva o devicepath name do dispositivo
                            DevicePathName = devicePathName;

                        }
                        else
                        {
                            DeviceDetected = false;
                            winusbevice.CloseDeviceHandle();

                            //MessageBox.Show("erro");
                        }
                    }
                    else
                    {
                        //MessageBox.Show("erro");
                    }

                    if (DeviceDetected == true)
                    {
                        success = deviceManagement.RegisterForDeviceNotifications(DevicePathName,
                                                                                    form.Handle,
                                                                                    winUsbGUID,
                                                                                    ref deviceNotificationHandle);
                        if (success)
                        {
                            winusbevice.InitializeDevice();

                        }


                    }
                }
                else
                {
                    //MessageBox.Show("erro");
                }
                return DeviceDetected;
            }
            catch //(Exception ex)
            {
                //MessageBox.Show("Erro: " + ex.Message);
                throw;
            }


        }

        public void SendByte(Byte buff)
        {

            byte[] buff_tmp = new byte[1];

            buff_tmp[0] = (byte)buff;

            winusbevice.SendViaBulkTransfer(ref buff_tmp, 1);
        }

        public void SendBuff(ref byte[] buff, UInt32 size)
        {
            if (DeviceIsConnected() == true)
            {
                winusbevice.SendViaBulkTransfer(ref buff, size);
            }
        }

        public bool DeviceIsConnected()
        {
            //DeviceDetected = FindAndEnumerateDevice();
            Boolean deviceFound;
            String devicePathName = "";

            System.Guid winUsbGUID = new System.Guid(GUID_STRING);
            deviceFound = deviceManagement.FindDeviceFromGuid(winUsbGUID, ref devicePathName);
            if (!deviceFound)
            {
                DeviceDetected = false;
            }
            return DeviceDetected;
        }

        public void ReadDataViaBulkTransfer(mydelegate func)
        {
            IAsyncResult ar = null;
            Byte[] buffer = new Byte[64];
            UInt32 bytesRead = 0;
            UInt32 bytesToRead = System.Convert.ToUInt32(64);
            Boolean success = false;

            ReadFromDeviceDelegate MyReadFromDeviceDelegate = new ReadFromDeviceDelegate(winusbevice.ReadViaBulkTransfer);

            try
            {
                ar = MyReadFromDeviceDelegate.BeginInvoke(
                     System.Convert.ToByte(winusbevice.myDevInfo.bulkInPipe),
                     bytesToRead,
                     ref buffer,
                     ref bytesRead,
                     ref success,
                     new AsyncCallback(func),
                     MyReadFromDeviceDelegate);
            }
            catch
            {
            }
            
        }
    }
}
