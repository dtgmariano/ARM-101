/*
 * uart.c
 *
 *  Created on: 12/02/2011
 *      Author: FELIPE
 */

#include "uart.h"
#include "LPC214x.h"
#include <stdint.h>

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

void vUART_ISR_Wrapper(void)
{
  VICVectAddr = 0;
}

void UART_Init(uint8_t channel, uint16_t baudrate)
{
	uint16_t Fdiv;

	//Setting TXD(UART0) function: Sets P0.0 with value 01 for Bits 1 and 0 (PINSEL0)
  	PINSEL0 &= ~(1<<1);
  	PINSEL0 |=  (1<<0);

  	//Setting RxD(UART0) function: Sets P0.1 with value 01 for Bits 3 and 2 (PINSEL0)
  	PINSEL0 &= ~(1<<3);
  	PINSEL0 |=  (1<<2);

  	U0LCR = 0x83; /* 8 bits, sem paridade, 1 Stop bit e o bit 7 é setado para configurar o divisor */
  	Fdiv = (60000000 / 16) / baudrate;
  	U0DLM = Fdiv / 256;  /* MSByte  */
  	U0DLL = Fdiv % 256;  /* LSByte */
  	U0LCR = 0x03; /* DLAB = 0 */
  	U0FCR = 0x07; /* Habilita e reseta TX e RX FIFO. */
  	U0TER = 0x80; /* Habilita Transmissão */

//  VICVectAddr1 = (unsigned) vUART_ISR_Wrapper;   //Set the timer ISR vector address
//  VICVectCntl1 = (1 << 5)|0x06;
//  VICIntSelect &= ~(1<<6);      //seleciona a se a interrupção sera FIQ ou IRQ para o timmer0
//  VICIntEnable |= (1 << 6);    //habilita a interrupção da UART
//
//  U0IER = 0x01|0x04;
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

void UART_putc(char c)
{
  while(!(U0LSR & LSR_TEMT)){} // Aguarda o registro ficar disponível
  U0THR = c;
}

uint16_t UART_ReceiveBuffer(uint8_t channel, uint8_t *Buffer, uint16_t size)
{
  uint16_t available = 0;

  if (U0LSR & LSR_RDR) //Verifica a existência de um dado valido
  {
    while (size > 0)
    {
      *Buffer++ = U0RBR;
      size--;
      available++;
    }
  }
  return available;
}
