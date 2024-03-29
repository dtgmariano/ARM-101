/*
  LPCUSB, an USB device driver for LPC microcontrollers
  Copyright (C) 2006 Bertrik Sikken (bertrik@sikken.nl)

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
  3. The name of the author may not be used to endorse or promote products
     derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
  This is a very simple custom device (not belonging to a specific USB
  class). It implements primitive read and write in the ARM memory space.

  Each transfer is initiated by a control transfer to inform the device
  about the address and size of the following data transfer.
  The data transfer takes place over a bulk endpoint (BULK_IN_EP for
  reads and BULK_OUT_EP for writes).

  This example can be used to measure USB transfer speed.
*/

#include <string.h>

#include "type.h"
#include "usbdebug.h"

//#include "console.h"
#include "usbapi.h"
#include "startup.h"
#include "uart.h"
#include "lpc214x.h"
#include "lcd.h"


#define BULK_IN_EP    0x82
#define BULK_OUT_EP   0x05

#define MAX_PACKET_SIZE 64

#define LE_WORD(x)    ((x)&0xFF),((x)>>8)

#define MAX_LENGTH 12800


static const U8 abDescriptors[] = {

/* Device descriptor */
  0x12,               // Size descriptor in bytes (18 bytes)
  DESC_DEVICE,        // Device descriptor
  LE_WORD(0x0200),    // bcdUSB - USB 2.0
  0x00,                 // bDeviceClass -  FIXME: 0x00 each interface specifies it's own class code if 0xFF The class code is vendor specified
  0x00,                 // bDeviceSubClass
  0x00,                 // bDeviceProtocol
  MAX_PACKET_SIZE0,     // bMaxPacketSize
  LE_WORD(0xFFFF),    // idVendor FIXME:
  LE_WORD(0x000E),    // idProduct - antes 0x0004
  LE_WORD(0x0100),    // bcdDevice
  0x01,                 // iManufacturer
  0x02,                 // iProduct
  0x03,                 // iSerialNumber
  0x01,                 // bNumConfigurations

// configuration
  0x09,
  DESC_CONFIGURATION,
  LE_WORD(0x20),      // wTotalLength
  0x01,           // bNumInterfaces
  0x01,           // bConfigurationValue
  0x00,           // iConfiguration
  0x80,           // bmAttributes
  0x32,           // bMaxPower

// interface
  0x09,
  DESC_INTERFACE,
  0x00,           // bInterfaceNumber
  0x00,           // bAlternateSetting
  0x02,           // bNumEndPoints  FIXME: Antes 0x02
  0xFF,           // bInterfaceClass - antes 0xFF
  0x00,           // bInterfaceSubClass
  0x00,           // bInterfaceProtocol
  0x00,           // iInterface

// bulk in
  0x07,
  DESC_ENDPOINT,
  BULK_IN_EP,       // bEndpointAddress
  0x02,           // bmAttributes = BULK
  LE_WORD(MAX_PACKET_SIZE),// wMaxPacketSize
  0,            // bInterval

// bulk out
  0x07,
  DESC_ENDPOINT,
  BULK_OUT_EP,      // bEndpointAddress
  0x02,           // bmAttributes = BULK
  LE_WORD(MAX_PACKET_SIZE),// wMaxPacketSize
  0,            // bInterval

// string descriptors
  0x04,
  DESC_STRING,
  LE_WORD(0x0409),

  // manufacturer string
  0x0E,
  DESC_STRING,
  'L', 0, 'P', 0, 'C', 0, 'U', 0, 'S', 0, 'B', 0,

  // product string
  0x1A,
  DESC_STRING,
  'G', 0, 'F', 0, 'T', 0, ' ', 0, 'D', 0, 'e', 0, 'v', 0, 'i', 0, 'c', 0, 'e', 0, 'v', 0, '1', 0,

  // serial number string
  0x12,
  DESC_STRING,
  'D', 0, 'E', 0, 'A', 0, 'D', 0, 'C', 0, '0', 0, 'D', 0, 'E', 0,

  // terminator
  0
};


typedef struct {
  U32   dwAddress;
  U32   dwLength;
} TMemoryCmd;


static TMemoryCmd MemoryCmd;
static U8     abVendorReqData[sizeof(TMemoryCmd)];
U8 str[] = "Felipe";
U8 buff[50] = {0};

static void _HandleBulkIn(U8 bEP, U8 bEPStatus)
{

  USBHwEPWrite(bEP, str, sizeof(str));

}


static void _HandleBulkOut(U8 bEP, U8 bEPStatus)
{
  int x = 0;

  x = USBHwEPRead(bEP,buff,sizeof(buff));


}


/*************************************************************************
  HandleVendorRequest
  ===================
    Handles vendor specific requests

  Control transfer fields:
  * request:  0x01 = prepare memory read
        0x02 = prepare memory write
  * index:  ignored
  * value:  ignored
  * data:   U32 dwAddress
        U32 dwLength

**************************************************************************/
static BOOL HandleVendorRequest(TSetupPacket *pSetup, int *piLen, U8 **ppbData)
{
  TMemoryCmd  *pCmd;

  pCmd = (TMemoryCmd *)*ppbData;

  switch (pSetup->bRequest) {

  // prepare read
  case 0x01:
    MemoryCmd = *pCmd;
    TRACE_DEBUG("READ: addr=%X, len=%d\n", MemoryCmd.dwAddress, MemoryCmd.dwLength);
    // send initial packet
    _HandleBulkIn(BULK_IN_EP, 0);
    *piLen = 0;
    break;

  // prepare write
  case 0x02:
    MemoryCmd = *pCmd;
    TRACE_DEBUG("WRITE: addr=%X, len=%d\n", MemoryCmd.dwAddress, MemoryCmd.dwLength);
    *piLen = 0;
    break;

  default:
    TRACE_DEBUG("Unhandled class %X\n", pSetup->bRequest);
    return FALSE;
  }
  return TRUE;
}




#define BAUD_RATE 9600


/*************************************************************************
  main
  ====
**************************************************************************/

int main(void)
{
  // PLL and MAM
//  int res = 0;
//  uint32_t i = 0;
//  U8 buff[] = "Felipe" ;


  Initialize();
  IODIR1 |= (1 << 17);
//  IOCLR1 = 1 << 17;
  IOSET1 = (1 << 17);

  // init DBG
//  ConsoleInit(60000000 / (16 * BAUD_RATE));
  UART_Init(0,BAUD_RATE);
//  Inicializar_LCD();

  TRACE_DEBUG("Initialising USB stack\n");
//  lcd_string("FELIPE");
  // initialise stack
  USBInit();

  // register device descriptors
  USBRegisterDescriptors(abDescriptors);

  // override standard request handler
  USBRegisterRequestHandler(REQTYPE_TYPE_VENDOR, HandleVendorRequest, abVendorReqData);

  // register endpoints
  USBHwRegisterEPIntHandler(BULK_IN_EP, _HandleBulkIn);
  USBHwRegisterEPIntHandler(BULK_OUT_EP, _HandleBulkOut);

  TRACE_DEBUG("Starting USB communication\n");

  // connect to bus
  USBHwConnect(TRUE);


//  res = USBHwEPWrite(BULK_OUT_EP,buff,sizeof(buff));
//  res = USBHwEPWrite(BULK_OUT_EP,buff,sizeof(buff));
//  res = USBHwEPWrite(BULK_OUT_EP,buff,sizeof(buff));
//  res = USBHwEPWrite(BULK_OUT_EP,buff,sizeof(buff));
//
//  if(res != TRUE)
//    TRACE_DEBUG("No write EP")
//  else
//    TRACE_DEBUG("Write bulk")
  // call USB interrupt handler continuously

  T0PR = 30;          //incrementa o contador, quando o contador prescale atinge o valor de PR.
  T0TCR = 0x00000002; //desabilita o contador e reseta o
  T0MCR = 0x00000003; //On match reset the counter and generate an interrupt
  T0MR0 = 0xFFFFFFFF;      // T0MR0 = Match Register 0,
                // Tempo = (CLOCK dos periféricos (PCLK) / Prescaler (T0PR)) *  Match Register (T0MR0)
                // Tempo = (PCLK/T0PR) * T0MR0, Ex: Tempo = (30 MHz/30) * 1000000 = 1us

  T0MR1   = 0x00000000; // Set duty cycle to zero
  T0TCR     = 0x00000001; //enable timer

//  VICVectAddr4 = (unsigned)T0isr;   //Set the timer ISR vector address
//  VICVectCntl4 = 0x00000024;      //Set channel
//  VICIntSelect &= ~(1<<4);      //seleciona a se a interrupção sera FIQ ou IRQ para o timmer0
//  VICIntEnable |= 0x00000010;     //habilita a interrupção de timer 0
//  enableIRQ(); // habilita que havera a utilização de interrupção


  while (1) {

    USBHwISR();
//    if(T0TC >= 10000000)
//    {
//      T0TC = 0;
////      USBHwISR();
//    }









//    if(i >= 30000)
//    {
//      res = USBHwEPWrite(BULK_OUT_EP,buff,sizeof(buff));
//     if(res != TRUE)
//       TRACE_DEBUG("No write EP")
//     else
//       TRACE_DEBUG("Write bulk")
//
//       i = 0;
//    }
//
//       i++;
  }

  return 0;
}

