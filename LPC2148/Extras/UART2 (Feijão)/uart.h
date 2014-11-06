/*
 * uart.h
 *
 *  Created on: 12/02/2011
 *      Author: FELIPE
 */


#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "uart.h"
#include "task.h"
/*
#include "FreeRTOS.h"
#include "task.h"
*/

void vUART_ISR_Wrapper( void ) __attribute__((interrupt("IRQ"),naked));
void vSerialISREntry(void);
int ReadChar_USART0_NonStop(void);
void UART_Init(uint8_t channel, uint16_t baud_rate);
void UART_SendBuffer(uint8_t channel, const uint8_t *Buffer, uint16_t size);
uint16_t UART_ReceiveBuffer(uint8_t channel, uint8_t *Buffer, uint16_t size);
uint16_t UART_ReceiveNBytes(uint8_t channel, uint8_t *Buffer, uint16_t u16_nbytes, portTickType timeout);
void UART_ResetFIFO(uint8_t channel, uint8_t u8_tx, uint8_t u8_rx);
void UART_SetInterCharTimeout(uint8_t channel, uint16_t nchars);

#endif /* UART_H_ */
