/*------------------------------------------------------------------------------------*/
/*                                       HEADER                                       */
/*------------------------------------------------------------------------------------*/

/***************************************************************************************
 *   File Name: main.c
 *  Created on: 19/03/2013
 *      Author: Daniel Teodoro G. Mariano
 * Description: Firmware for LPC2148, to control the MA2000 robot, a mechanical arm
 * 				with 6 degrees of freedom.
 *
 * 				The first three DOF's (1-Waist, 2-Shoulder, 3-Elbow) are controlled each
 * 				one by a DC Motor with the following configurations: A PWM at 2500 Hz with
 * 				variable Duty Cycle is assigned to the variable DOF_PWM_DC, according with
 * 				the proportional control and two GPIO for controlling the inputs of the
 * 				LPC6203 Full Bridge Driver.
 *
 * 				Each motor has a potentiometer connected to the motor shaft by a toothed
 * 				belt, and this potentiometer provides a voltage signal proportional to the
 * 				angular position of the motor. This signal is capture by an ADC channel
 * 				and is assigned to the variable DOF_ADC_POT
 *
 * 				The proportional control is effected by the relationship of the variables
 * 				DOF_ADC_POT and the setpoint position established by the user assigned to
 * 				the variable DOF_SETPOINT.
 *
 * 				The others three DOF's (4-Pitch, 5-Yaw, 6-Roll) are controlled each one
 * 				by a Servomotor with the following configurations: A PWM at 50 Hz with
 * 				variable Duty Cycle (5%-10%) to control the servo.
***************************************************************************************/


/*------------------------------------------------------------------------------------*/
/*                                PINS CONFIGURATION                                  */
/*------------------------------------------------------------------------------------*/

/***************************************************************************************
* Function	  	  LPC2148 		Connector
* PWM 2 (31)      EXT1-8		EXT1 20
* PWM 4 (31)      				EXT1 17
* PWM 5 (31)           			EXT1 9
* P0.18 (53)      EXT1-19		EXT1 7
* P0.19 (54)      EXT1-20		EXT1 8
* AD0.1 (13)      EXT2-1		EXT2 25
* AD0.2 (14)      EXT2-2		EXT2 26
*
* 				  +3.3V   		EXT2-4
* 			      GND     		EXT2-1 e EXT1-2
***************************************************************************************/

/*------------------------------------------------------------------------------------*/
/*                                 	   INCLUDES                                       */
/*------------------------------------------------------------------------------------*/
#include "LPC214x.h"
#include "cpu_init.h"
#include "Modules/adc.h"
#include "Modules/pwm.h"
#include "Modules/gpio.h"
#include "Modules/uart.h"
#include "Modules/iprintf.h"
#include "Modules/timer.h"

/*------------------------------------------------------------------------------------*/
/*                                 	   DEFINES                                        */
/*------------------------------------------------------------------------------------*/
#define PWMSERVO_COUNT 4000

/*------------------------------------------------------------------------------------*/
/*                         	   VARIABLES DECLARATION                                  */
/*------------------------------------------------------------------------------------*/
typedef enum {IDLE, RECEIVING_DATA, DEF_SETPOINT} STATE;

unsigned int DOF1_SETPOINT=0;
//unsigned int DOF2_SETPOINT=0;
//unsigned int DOF3_SETPOINT=0;
//unsigned int DOF4_SETPOINT=0;
//unsigned int DOF5_SETPOINT=0;
//unsigned int DOF6_SETPOINT=0;

unsigned int DOF1_PWM_DC=0;
//unsigned int DOF2_PWM_DC=0;
//unsigned int DOF3_PWM_DC=0;
unsigned int DOF4_PWM_DC=0;
unsigned int DOF5_PWM_DC=0;
//unsigned int DOF6_PWM_DC=0;

unsigned int DOF1_ADC_POT=0;
//unsigned int DOF2_ADC_POT=0;
//unsigned int DOF3_ADC_POT=0;

unsigned int DOF1_ERROR=0;
//unsigned int DOF2_ERROR=0;
//unsigned int DOF3_ERROR=0;

int iCountTimer0=0;

/*------------------------------------------------------------------------------------*/
/*                                     PROTOTYPES                                     */
/*------------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------*/
/*                               FUNCTIONS  IMPLEMENTATION                            */
/*------------------------------------------------------------------------------------*/

/***************************************************************************************
 * Function Name: PWM_InterruptControl(void)
 * 		 	Type: Void
 * 	 Return Type: Void
 * 	 Description: Function called by a timer interruption to control PWM 4, 5 and 6
***************************************************************************************/
void PWM_InterruptControl(void)
{
	if(iCountTimer0 >= DOF4_PWM_DC)
	{
		IOCLR0 = (1<<18);
	}

	if(iCountTimer0 >= DOF5_PWM_DC)
	{
		IOCLR0 = (1<<19);
	}

	//if(iCountTimer0 == 10000)
	if(iCountTimer0 >= PWMSERVO_COUNT)
	{
		IOSET0 = (1<<18);
		IOSET0 = (1<<19);
		iCountTimer0 = 0;
	}
	iCountTimer0++;
}

/***************************************************************************************
 * Function Name: Modules_Init(void)
 * 		 	Type: Void
 * 		  Return: Void
 * 	 Description: Initiates CPU configurations,
 * 				  Configure Timer, PWM, ADC, UART Modules,
 * 				  Sets GPIO Pins.
***************************************************************************************/
void Modules_Init(void)
{
	cpu_init();
	Timer_Init();
	Timer_RegisterCallback(PWM_InterruptControl);
	Timer_Enable();
	PWMInit();
	setDutyCycle(2,100);
	ADCInit(ADC_CLK);
	//UART_Init(0,9600);
	GPIOInit();
}

/***************************************************************************************
 * Function Name: IdleState(void)
 * 		 	Type: STATE
 * 		  Return:
 * 	 Description: Function called by atimer interruption
***************************************************************************************/
STATE IdleState(void)
{
	return RECEIVING_DATA;
}

/***************************************************************************************
 * Function Name: ReceivingDataState(void)
 * 		 	Type: STATE
 * 		  Return:
 * 	 Description: Function called by a timer interruption
***************************************************************************************/
STATE ReceivingDataState(void)
{
	DOF1_SETPOINT = 1000;
	DOF1_ADC_POT = ADC0Read(1);
	return DEF_SETPOINT;
}

/***************************************************************************************
 * Function Name: DefSetPointState(void)
 * 		 	Type: STATE
 * 		  Return:
 * 	 Description: Function called by a timer interruption
***************************************************************************************/
STATE DefSetPointState(void)
{
	if(DOF1_SETPOINT > DOF1_ADC_POT)
	{
		return DEF_SETPOINT;
	}
	else if(DOF1_SETPOINT < DOF1_ADC_POT)
	{
		return DEF_SETPOINT;
	}
	else
	{
		return IDLE;
	}
}

/***************************************************************************************
 * Function Name: main(void)
 * 		 	Type: Integer
 * 		  Return: 0
 * 	 Description: Main function for firmware
***************************************************************************************/

int main (void)
{
	//Creates a variable of STATE type, and attributes its initial value
//	STATE nextState = IDLE;

	//Calls function Modules_Init()
	Modules_Init();

	while (1)
	{
		DOF4_PWM_DC = 200;
		DOF5_PWM_DC = 400;
//		switch(nextState)
//		{
//			case IDLE:
//			{
//				nextState = IdleState();
//			}
//			case RECEIVING_DATA:
//			{
//				nextState = ReceivingDataState();
//			}
//			case DEF_SETPOINT:
//			{
//				nextState = DefSetPointState();
//			}
//		}
	}
	return(0);
}
