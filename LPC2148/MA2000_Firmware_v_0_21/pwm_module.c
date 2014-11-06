/*
 * pwm_module.c
 *
 *  Created on: 30/11/2012
 *      Author: Daniel T. G. Mariano
 *    Filename: pwm_module.c
 *     Version: 0.0
 * Description: PWM module developed to set the PWM Channels
 */

#include "pwm_module.h"
#include "LPC214x.h"
#include "cpu_init.h"

void setPWM1Function()
{
	//Setting PWM1 function: Sets P0.0 with value 10 for Bits 1 and 0 (PINSEL0)
	//P0.0 -> EXT1-1 -> Pin 25
	PINSEL0 &= ~(3<<0);
	PINSEL0 |=  (1<<1);
}

void setPWM3Function()
{
	//Setting PWM3 function: Sets P0.1 with value 10 for Bits 3 and 2 (PINSEL0)
	//P0.1 -> EXT1-2 -> Pin 26
	PINSEL0 &= ~(3<<2);
	PINSEL0 |=  (1<<3);
}

void setPWM2Function()
{
	//Setting PWM2 function: Sets P0.7 with value 10 for Bits 15 and 14 (PINSEL0)
	//P0.7 -> EXT1-8 -> Pin 20
	PINSEL0 &= ~(3<<14);
	PINSEL0 |=  (1<<15);
}

void setPWM4Function()
{
	//Setting PWM4 function: Sets P0.8 with value 10 for Bits 17 and 16 (PINSEL0)
	//P0.8 -> EXT1-9 -> Pin 17
	PINSEL0 &= ~(3<<16);
	PINSEL0 |=  (1<<17);
}

void setPWM6Function()
{
	//Setting PWM6 function: Sets P0.9 with value 10 for Bits 19 and 18 (PINSEL0)
	//P0.9 -> EXT1-10 -> Pin 18
	PINSEL0 &= ~(3<<18);
	PINSEL0 |=  (1<<19);
}

void setPWM5Function()
{
	//Setting PWM5 function: Sets P0.21 with value 01 for Bits 11 and 10 (PINSEL1)
	//P0.21 -> EXT1-22 -> Pin 6
	PINSEL1 &= ~(1<<11);
	PINSEL1 |=  (1<<10);
}

void setPWMModuleConfigs()
{
	//PWM Timer Control Register. The PWMTCR is used to control the Timer
	//Counter functions. The Timer Counter can be disabled or reset through
	//the PWMTCR.
	PWMTCR  |= (1<<1); // Reset PWM

	//PWM Match Register 0. PWMMR0 can be enabled through PWMMCR to
	//reset the PWMTC, stop both the PWMTC and PWMPC, and/or generate
	//an interrupt when it matches the PWMTC. In addition, a match between
	//PWMMR0 and the PWMTC sets all PWM outputs that are in single-edge
	//mode, and sets PWM1 if it is in double-edge mode.
	PWMMR0  = 48000; //set PWM period (400uS -> 2,5khz)

	//PWM Prescale Register. The PWMTC is incremented every PWMPR+1
	//cycles of PCLK.
	PWMPR   = 0;  // Prescale unable

	//PWM Control Register. Enables PWM outputs and selects PWM channel
	//types as either single edge or double edge controlled.
	PWMPCR &= ~((1<<2)|(1<<4)|(1<<5));
	PWMPCR |= ((1<<13)|(1<<12)|(1<<10)); // Sets PWM 2,4,5 as single edge output

	//PWM Timer Control Register. The PWMTCR is used to control the Timer
	//Counter functions. The Timer Counter can be disabled or reset through
	//the PWMTCR.
	PWMTCR  |= ((1<<0)|(1<<3)); // Counter and PWM enabled

	//PWM Match Register 2. PWMMR2 can be enabled through PWMMCR to
	//reset the PWMTC, stop both the PWMTC and PWMPC, and/or generate
	//an interrupt when it matches the PWMTC. In addition, a match between
	//PWMMR2 and the PWMTC clears PWM2 in either single-edge mode or
	//double-edge mode, and sets PWM3 if it is in double-edge mode.
	PWMMR2  = 12000; //Set PWM 2 Duty Cycle

	//PWM Match Register 4. PWMMR4 can be enabled through PWMMCR to
	//reset the PWMTC, stop both the PWMTC and PWMPC, and/or generate
	//an interrupt when it matches the PWMTC. In addition, a match between
	//PWMMR4 and the PWMTC clears PWM4 in either single-edge mode or
	//double-edge mode, and sets PWM5 if it is in double-edge mode.
	PWMMR4  = 24000; //Set PWM 4 Duty Cycle

	//PWM Match Register 5. PWMMR5 can be enabled through PWMMCR to
	//reset the PWMTC, stop both the PWMTC and PWMPC, and/or generate
	//an interrupt when it matches the PWMTC. In addition, a match between
	//PWMMR5 and the PWMTC clears PWM5 in either single-edge mode or
	//double-edge mode, and sets PWM6 if it is in double-edge mode.
	PWMMR5  = 36000; //Set PWM 5 Duty Cycle

	//The PWM Match Control Register is used to control what operations are performed when
	//one of the PWM Match Registers matches the PWM Timer Counter.
	PWMMCR  = 0x0000003; // Reset on Match 0
}

void initializePWM()
{
	setPWM2Function();
	setPWM4Function();
	setPWM5Function();
	setPWMModuleConfigs();
}
