/*
 * gpio.c
 *
 *  Created on: 22 de Fev de 2013
 *      Author: ufu
 */
#include "LPC214x.h"
#include "gpio.h"

void GPIOInit()
{
	//Setting GPIO Port 0.18 function: Sets P0.18 with value 00 for Bits 5 and 4 (PINSEL1)
	//P0.18       EXT1-19		EXT1 7
	PINSEL1 &= ~(1<<4);
	PINSEL1 &= ~(1<<5);
	IODIR0 |= (1<<18);	//Setting P0.18 as output

	//Setting GPIO Port 0.19 function: Sets P0.19 with value 00 for Bits 7 and 6 (PINSEL1)
	//P0.19      EXT1-20		EXT1 8
	PINSEL1 &= ~(1<<6);
	PINSEL1 &= ~(1<<7);
	IODIR0 |= (1<<19);	//Setting P0.19 as output
}

