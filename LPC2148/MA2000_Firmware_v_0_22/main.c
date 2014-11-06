/*
 * main.c
 *
 *  Created on : 22/02/2013
 *      Author: Daniel Teodoro G. Mariano
 */
//Function	  	  LPC2148 		Connector
//PWM 2 (31)      EXT1-8		EXT1 20
//PWM 4 (31)      				EXT1 17
//PWM 5 (31)           			EXT1 9
//P0.18 (53)      EXT1-19		EXT1 7
//P0.19 (54)      EXT1-20		EXT1 8
//AD0.1 (13)      EXT2-1		EXT2 25
//AD0.2 (14)      EXT2-2		EXT2 26
//
//				  +3.3V   		EXT2-4
//			      GND     		EXT2-1 e EXT1-2

#include "LPC214x.h"
#include "cpu_init.h"
//#include "MA2000/robot.h"
#include "Modules/adc.h"
#include "Modules/pwm.h"
#include "Modules/gpio.h"
#include "Modules/uart.h"
#include "Modules/iprintf.h"


int main (void)
{
	cpu_init();
	//robot_setconfig();
	UART_Init(0,9600);
	GPIOInit();
	PWMInit();
	ADCInit(ADC_CLK);

	int count = 0;

	unsigned int setpoint_dof1=0;
	unsigned int reostato_dof1=0;
	unsigned int k = 0;
	setDutyCycle(2,1022);
	setDutyCycle(4,25);
	setDutyCycle(5,50);

	while (1)
	{
		setpoint_dof1 = ADC0Read(1);
		asm volatile("NOP");
		reostato_dof1 = 950;

		for(int i=0; i<50000;i++)
		{
			asm volatile("NOP");
		}

		if((setpoint_dof1 > reostato_dof1) || (reostato_dof1 < 50))
		{
			IOCLR0 = (1<<19);
			IOSET0 = (1<<18);
			k = setpoint_dof1 - reostato_dof1;
			setDutyCycle(2,(setpoint_dof1 - reostato_dof1));
		}
		if((reostato_dof1 > setpoint_dof1) || (reostato_dof1 > 970))
		{
			IOCLR0 = (1<<18);
			IOSET0 = (1<<19);
			k = reostato_dof1 - setpoint_dof1;
			setDutyCycle(2,(reostato_dof1 - setpoint_dof1));
		}
		//setDutyCycle(2,23999);

		count++;
		if(count==1000)
		{

		}
	}
	return(0);
}
