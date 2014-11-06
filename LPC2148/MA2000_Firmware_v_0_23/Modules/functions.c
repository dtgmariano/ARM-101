/*
 * functions.c
 *
 *  Created on: 05/12/2012
 *      Author: Daniel
 */
#include "LPC214x.h"
#include "cpu_init.h"

/*void setADC0Function(int adc0_channel){

	if(adc0_channel == 1){
		//Setting AD0.1 function: Sets P0.28 with value 01 for Bits 25 and 24 (PINSEL1)
		//P0.28 -> EXT2.1 -> Pin 25
		PINSEL1 &= ~(1<<25);
		PINSEL1 |=  (1<<24);
	}
	else if(adc0_channel == 2){
		//Setting AD0.2 function: Sets P0.29 with value 01 for Bits 27 and 26 (PINSEL1)
		//P0.29 -> EXT2.2 -> Pin 26
		PINSEL1 &= ~(1<<27);
		PINSEL1 |=  (1<<26);
	}
	else if(adc0_channel == 3){
		//Setting AD0.3 function: Sets P0.30 with value 01 for Bits 29 and 28 (PINSEL1)
		//P0.30 -> EXT2.3 -> Pin 24
		PINSEL1 &= ~(1<<29);
		PINSEL1 |=  (1<<28);
	}
	else if(adc0_channel == 4){
		//Setting AD0.4 function: Sets P0.25 with value 01 for Bits 19 and 18 (PINSEL1)
		//P0.25 -> EXT1.25 -> Pin 1
		PINSEL1 &= ~(1<<19);
		PINSEL1 |=  (1<<18);
	}
}

void setPWMFunction(int pwm_channel) {
	if(pwm_channel == 1){
		//Setting PWM1 function: Sets P0.0 with value 10 for Bits 1 and 0 (PINSEL0)
		//P0.0 -> EXT1-1 -> Pin 25
		PINSEL0 &= ~(3 << 0);
		PINSEL0 |= (1 << 1);
	}
	else if(pwm_channel == 2){
		//Setting PWM2 function: Sets P0.7 with value 10 for Bits 15 and 14 (PINSEL0)
		//P0.7 -> EXT1-8 -> Pin 20
		PINSEL0 &= ~(3 << 14);
		PINSEL0 |= (1 << 15);
	}
	else if(pwm_channel == 3){
		//Setting PWM3 function: Sets P0.1 with value 10 for Bits 3 and 2 (PINSEL0)
		//P0.1 -> EXT1-2 -> Pin 26
		PINSEL0 &= ~(3 << 2);
		PINSEL0 |= (1 << 3);
	}
	else if(pwm_channel == 4){
		//Setting PWM4 function: Sets P0.8 with value 10 for Bits 17 and 16 (PINSEL0)
		//P0.8 -> EXT1-9 -> Pin 17
		PINSEL0 &= ~(3 << 16);
		PINSEL0 |= (1 << 17);
	}
	else if(pwm_channel == 5){
		//Setting PWM5 function: Sets P0.21 with value 01 for Bits 11 and 10 (PINSEL1)
		//P0.21 -> EXT1-22 -> Pin 6
		PINSEL1 &= ~(3 << 11);
		PINSEL1 |= (1 << 10);
	}
	else if(pwm_channel == 6){
		//Setting PWM6 function: Sets P0.9 with value 10 for Bits 19 and 18 (PINSEL0)
		//P0.9 -> EXT1-10 -> Pin 18
		PINSEL0 &= ~(3 << 18);
		PINSEL0 |= (1 << 19);
	}
}*/
