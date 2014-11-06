/*
 * robot.c
 *
 *  Created on: 22 de Fev de 2013
 *      Author: Daniel Teodoro G Mariano
 */

#include "LPC214x.h"
#include "Modules/pwm.h"
#include "Modules/adc.h"
#include "Modules/gpio.h"
#include "Modules/uart.h"
#include "Modules/iprintf.h"

unsigned int position_dof1;

//Function to configure ADC Channels, PWM Channels and GPIO pins for DOF1 Control
void robot_setconfig()
{
	//UART_Init(0,9600);
	ADCInit(ADC_CLK);
	PWMInit();
	GPIOInit();
}

//Function to control DOF1
void control_dof1(unsigned int setpoint_dof1)
{
	position_dof1 = ADC0Read(1);
	if(setpoint_dof1 > position_dof1)
	{
		IOCLR0 = (1<<19);
		IOSET0 = (1<<18);
		//setDutyCycle(2,(setpoint_dof1 - position_dof1)*100/1024);
	}
	if(position_dof1 > setpoint_dof1)
	{
		IOCLR0 = (1<<18);
		IOSET0 = (1<<19);
		//setDutyCycle(2,(position_dof1 - setpoint_dof1)*100/1024);
	}
	setDutyCycle(2,85);
	//iprintf("Position: %d     Setpoint: %d\r\n",position_dof1, setpoint_dof1);
}
