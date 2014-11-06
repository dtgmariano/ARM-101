/*
 * uart.c
 *
 *  Created on: 12/02/2011
 *      Author: FELIPE
 */

#include "uart.h"
#include "LPC214x.h"
#include <stdint.h>
#include "FreeRTOS.h"

#define LSR_RDR          0x01
#define LSR_OE           0x02
#define LSR_PE           0x04
#define LSR_FE           0x08
#define LSR_BI           0x10
#define LSR_RXFE         0x80
#define LSR_TEMT         0x40
#define FCR_resetRXFIFO  0x01
#define FCR_resetTXFIFO  0x02
#define N_UART_CHANNELS     2

static volatile int usart_rxprod = 0;
static volatile int usart_rxcons = 0;
#define USART_RX_SIZE 200
static volatile unsigned char usart_rxbuf[USART_RX_SIZE];

typedef struct
{
  uint16_t u8_interCharTimeoutInChars;
  portTickType interCharTimeout;
  uint32_t baud;

} ST_ChannelConfig;

static ST_ChannelConfig mast_channelConfig[N_UART_CHANNELS];

static void UpdateInterCharTimeoutInTicks(uint8_t channel);

void vUART_ISR_Wrapper(void)
{
  /* Save the context of the interrupted task. */
//  portSAVE_CONTEXT();

  /* Call the handler.  This must be a separate function from the wrapper
   to ensure the correct stack frame is set up. */
//  vSerialISREntry();

  VICVectAddr = 0;
  /* Restore the context of whichever task is going to run next. */
//  portRESTORE_CONTEXT();
}

void vSerialISREntry(void)
{
  // interrupt code goes here
  if (U0LSR & LSR_RDR) //Verifica a existência de um dado valido
  {
    usart_rxbuf[usart_rxprod++] = U0RBR; //Incrementa o índice do buffer circular e armazeda o dado que está presente no buffer da serial
    if (usart_rxprod >= USART_RX_SIZE)  //Verifica se o índice do produtor chegou no final do buffer
    {
      usart_rxprod = 0;
    }
    if (usart_rxprod == usart_rxcons)  //verifica se o buffer está cheio pois o buffer está mais que o consumo
    {
      usart_rxcons++;                   //Incrementa o consumidor pois na proxima interrupção caso o buffer continue cheio o dado mais antigo será perdido e novamente prod == consum
      if (usart_rxcons >= USART_RX_SIZE) //Verifica necessidade de rotacinar o índice o consumidor
      {
        usart_rxcons = 0;
      }
    }
  }
}

int ReadChar_USART0_NonStop(void)
{
  int c = -1;
  if (usart_rxprod != usart_rxcons)
  {
    c = usart_rxbuf[usart_rxcons++];
    if (usart_rxcons >= USART_RX_SIZE)
    {
      usart_rxcons = 0;
    }
  }
  return c;
}

void UART_SetInterCharTimeout(uint8_t channel, uint16_t nchars)
{
  mast_channelConfig[channel].u8_interCharTimeoutInChars = nchars;
  UpdateInterCharTimeoutInTicks(channel);
}

static void UpdateInterCharTimeoutInTicks(uint8_t channel)
{
  //For example if baud is 9600bps and u8_interCharTimeoutInChars = 3 chars then
  //9600b ------ 1s
  //3*8b  ------ Xs
  //this implies that Xs = ((3*8)/(9600))
  //and X in ms is Xms = (((3*8)/(9600))*1000)
  // X in tiks is Xms/1000
  mast_channelConfig[channel].interCharTimeout = (portTickType) (((((uint32_t)mast_channelConfig[channel].u8_interCharTimeoutInChars)*8000)/mast_channelConfig[channel].baud)/portTICK_RATE_MS);
  if (!mast_channelConfig[channel].interCharTimeout)
  {
    mast_channelConfig[channel].interCharTimeout = 1; // minimum wait time
  }
//  TRACE_DEBUG("Interchar timeout:%d", (int)mast_channelConfig[channel].interCharTimeout);
}

void UART_Init(uint8_t channel, uint16_t baudrate)
{
  uint16_t Fdiv;

  PINSEL0 = 0x00050005; /* Habilita RxD1, TxD1, RxD0 e TxD0 */

  U0LCR = 0x83; /* 8 bits, sem paridade, 1 Stop bit e o bit 7 é setado para configurar o divisor */
  Fdiv = (60000000 / 16) / baudrate;
  U0DLM = Fdiv / 256;  /* MSByte  */
  U0DLL = Fdiv % 256;  /* LSByte */
  U0LCR = 0x03; /* DLAB = 0 */
  U0FCR = 0x07; /* Habilita e reseta TX e RX FIFO. */
  U0TER = 0x80; /* Habilita Transmissão */

  mast_channelConfig[channel].baud = baudrate;

  VICVectAddr1 = (unsigned) vUART_ISR_Wrapper;   //Set the timer ISR vector address
  VICVectCntl1 = (1 << 5)|0x06;
  VICIntSelect &= ~(1<<6);      //seleciona a se a interrupção sera FIQ ou IRQ para o timmer0
  VICIntEnable |= (1 << 6);    //habilita a interrupção da UART

  U0IER = 0x01|0x04;
//  enableIRQ();
}

void UART_SendBuffer(uint8_t channel,const uint8_t *Buffer, uint16_t size)
{
  if(size > 0)
  {
    do
    {
      while(!(U0LSR & LSR_TEMT)){} // Aguarda o registro ficar disponível
      U0THR = *Buffer;
      Buffer++;
      size--;
    }while(size > 0);
  }
}

uint16_t UART_ReceiveBuffer(uint8_t channel, uint8_t *Buffer, uint16_t size)
{
  uint16_t available = 0;
  int c;

  while ((size > 0) && ((c=ReadChar_USART0_NonStop())!=-1))
  {
    *Buffer++ = c;
    size--;
    available++;
  }
  return available;
}

void UART_ResetFIFO(uint8_t channel, uint8_t u8_tx, uint8_t u8_rx)
{
  if(u8_tx)
    U0FCR = FCR_resetTXFIFO;
  if(u8_rx)
    U0FCR = FCR_resetRXFIFO;

  usart_rxprod = usart_rxcons;
}

uint16_t UART_ReceiveNBytes(uint8_t channel, uint8_t *Buffer, uint16_t u16_nbytes, portTickType timeout)
{
  portTickType start = xTaskGetTickCount();
  portTickType currTick  = start;
  portTickType checkInterval = mast_channelConfig[channel].interCharTimeout/2;

  uint16_t rx;
  uint8_t *p = Buffer;

  if (checkInterval == 0)
  {
    checkInterval = 1;
  }

  while(u16_nbytes && ((currTick - start) < timeout))
  {
    rx = UART_ReceiveBuffer(0,p,u16_nbytes);
    currTick = xTaskGetTickCount();

    if(rx)
    {
      u16_nbytes -= rx;
      p += rx;
      start = currTick;
      timeout = mast_channelConfig[channel].interCharTimeout;
    }
    else if((timeout > (currTick - start)) && (checkInterval < (timeout - (currTick-start))))
    {
      vTaskDelay(checkInterval);
    }
  }
  return (p-Buffer);
}
