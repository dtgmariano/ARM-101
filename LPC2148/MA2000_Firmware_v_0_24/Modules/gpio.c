/*------------------------------------------------------------------------------------*/
/*                                       HEADER                                       */
/*------------------------------------------------------------------------------------*/

/***************************************************************************************
 *Project Name: MA2000 Firmware
 *	   Version: 0.24
 *   File Name: gpio.c
 *  Created on: 19/03/2013
 *      Author: Daniel Teodoro G. Mariano
 *       Email: dtgm@uol.com.br
 * Description: GPIO Module with the purpose of setting the output pins
***************************************************************************************/

/*------------------------------------------------------------------------------------*/
/*                                 	   INCLUDES                                       */
/*------------------------------------------------------------------------------------*/
#include "LPC214x.h"
#include "gpio.h"

/*------------------------------------------------------------------------------------*/
/*                               FUNCTIONS  IMPLEMENTATION                            */
/*------------------------------------------------------------------------------------*/

/***************************************************************************************
 * Function Name: GPIOInit()
 * 		 	Type: Void
 *	  Parameters: None
 * 	 	 Returns: Void
 * 	 Description: Sets P0.18, P0.19 as GPIO
***************************************************************************************/

void GPIOInit()
{
	//Setting GPIO Port 0.18 function: Sets P0.18 with value 00 for Bits 5 and 4 (PINSEL1)
	//P0.18       EXT1-19		EXT1 7
	PINSEL1 &= ~((1<<4)|(1<<5));
	IODIR0 |= (1<<18);	//Setting P0.18 as output

	//Setting GPIO Port 0.19 function: Sets P0.19 with value 00 for Bits 7 and 6 (PINSEL1)
	//P0.19      EXT1-20		EXT1 8
	PINSEL1 &= ~((1<<6)|(1<<7));
	IODIR0 |= (1<<19);	//Setting P0.19 as output
}

