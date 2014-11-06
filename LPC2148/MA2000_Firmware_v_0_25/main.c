/*------------------------------------------------------------------------------------*/
/*                                       HEADER                                       */
/*------------------------------------------------------------------------------------*/
/***************************************************************************************
 *Project Name: MA2000 Firmware
 *	   Version: 0.24
 *   File Name: main.c
 *  Created on: 19/03/2013
 *      Author: Daniel Teodoro G. Mariano
 *       Email: dtgm@uol.com.br
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
* Pins		Function	  	LPC2148 	Connector
* P0.7		PWM 2 (31)      EXT1-8		EXT1 20
* P0.8		PWM 4 (31)      			EXT1 17
* P0.21		PWM 5 (31)           		EXT1 9
* P0.18		P0.18 (53)      EXT1-19		EXT1 7
* P0.19		P0.19 (54)      EXT1-20		EXT1 8
* P0.28		AD0.1 (13)      EXT2-1		EXT2 25
* P0.29		AD0.2 (14)      EXT2-2		EXT2 26
* P0.30		AD0.3 ()
* P0.25		AD0.4
*
* 				  			+3.3V   	EXT2-4
* 			      			GND     	EXT2-1 e EXT1-2
***************************************************************************************/

/*------------------------------------------------------------------------------------*/
/*                                 	   INCLUDES                                       */
/*------------------------------------------------------------------------------------*/
#include <string.h>

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
#define PWMSERVO_COUNT 4290
// PWMSERVO_COUNT = (MATCHREGISTER0 * 500)


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
int iCountUart=0;
int iCountTest=0;

uint8_t Test[5]={'0','1','b','3','e'};

uint8_t tBuffer_In;
uint8_t tBuffer_Out;

//unsigned int SVMOTOR_VEC[180] = {};
//unsigned int DCMOTOR_VEC[270] = {};

/*------------------------------------------------------------------------------------*/
/*                                     PROTOTYPES                                     */
/*------------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------*/
/*                            INTERRUPTIONS  IMPLEMENTATION                           */
/*------------------------------------------------------------------------------------*/

/***************************************************************************************
 * Function Name: PWM_InterruptControl(void)
 * 		 	Type: Void
 *	  Parameters:
 * 	 	 Returns: Void
 * 	 Description: Function called by a timer interruption to control PWM 4, 5 and 6
***************************************************************************************/
void PWM_InterruptControl(void)
{
	if(iCountTimer0 >= DOF4_PWM_DC)
		IOCLR0 = (1<<18); //Sets low for GPIO 0.18

	if(iCountTimer0 >= DOF5_PWM_DC)
		IOCLR0 = (1<<19); //Sets low for GPIO 0.19

	if(iCountTimer0 >= PWMSERVO_COUNT)
	{
		IOSET0 = (1<<18); //Sets high for GPIO 0.18
 		IOSET0 = (1<<19); //Sets high for GPIO 0.19
		iCountTimer0 = 0;
	}
	iCountTimer0++;
}

/*------------------------------------------------------------------------------------*/
/*                               FUNCTIONS  IMPLEMENTATION                            */
/*------------------------------------------------------------------------------------*/

/***************************************************************************************
 * Function Name: Modules_Init(void)
 * 		 	Type: Void
 *	  Parameters:
 * 		 Returns: Void
 * 	 Description: Initiates CPU configurations,
 * 				  Configure Timer, PWM, ADC, UART Modules,
 * 				  Sets GPIO Pins.
***************************************************************************************/
void Modules_Init(void)
{
	cpu_init();

	PWMInit();
	ADCInit(ADC_CLK);
	Timer_Init();
	Timer_RegisterCallback(PWM_InterruptControl);
	Timer_Enable();
	UART_Init(0,9600);
	GPIOInit();
}

/***************************************************************************************
 * Function Name: IdleState(void)
 * 		 	Type: STATE
 *	  Parameters:
 * 		 Returns:
 * 	 Description:
***************************************************************************************/
STATE IdleState(void)
{
	if(iCountUart>=20000)
	{
		iCountUart=0;
		iprintf("DOF1_ADC_POT: %d In: %d\r\n",DOF1_ADC_POT, tBuffer_In);


		UART_SendBuffer('0',Test,(sizeof Test));

		UART_ReceiveBuffer('0',&tBuffer_In,4);

//		UART_ReceiveBuffer('0',&tBuffer,1);
//		UART_SendBuffer('0',&tBuffer,1);

//		uint8_t data[4];
//		uint8_t data[8];

//		float x;
//		int y;
//		int idx = 0;
//
//		while(UART_ReceiveBuffer(0, data, 4) == 0);
//		UART_ReceiveBuffer(0, data, 8);

//		memcpy(&x, &data[0], sizeof(float));
//		idx += sizeof(float);
//		memcpy(&y, &data[idx], sizeof(int)); idx += sizeof(int);

//		iprintf("x: %f\r\n",x);
		//iprintf("x: %f y: %d\r\n",x, y);
	}
	iCountUart++;

//	if(iCountTest>=1023)
//	{
//		iCountTest=0;
//	}
//	iCountTest++;

	return RECEIVING_DATA;
}

/***************************************************************************************
 * Function Name: ReceivingDataState(void)
 * 		 	Type: STATE
  *	  Parameters:
 * 		 Returns:
 * 	 Description:
***************************************************************************************/
STATE ReceivingDataState(void)
{
	//UART_ReceiveBuffer(0,teste,1);
	DOF1_SETPOINT = 1000;
	DOF1_ADC_POT = ADC0Read(1);
	return DEF_SETPOINT;
}

/***************************************************************************************
 * Function Name: DefSetPointState(void)
 * 		 	Type: STATE
 *	  Parameters:
 * 		 Returns:
 * 	 Description:
***************************************************************************************/
STATE DefSetPointState(void)
{
	DOF4_PWM_DC = tBuffer_In;
	DOF5_PWM_DC = 450;
	setDutyCycle(2,(unsigned int)150);
//	if((DOF1_SETPOINT>DOF1_ADC_POT) || DOF1_ADC_POT < 50)
//	{
//		IOCLR0 = (1<<18);
//		IOSET0 = (1<<19);
//		DOF1_ERROR = DOF1_SETPOINT - DOF1_ADC_POT;
//	}
//	if((DOF1_SETPOINT<DOF1_ADC_POT) || DOF1_ADC_POT > 970)
//	{
//		IOCLR0 = (1<<19);
//		IOSET0 = (1<<18);
//		DOF1_ERROR = DOF1_ADC_POT - DOF1_SETPOINT;
//	}
//	setDutyCycle(2,DOF1_ERROR);
	return IDLE;
}

/***************************************************************************************
 * Function Name: main(void)
 * 		 	Type: Integer
 *	  Parameters:
 * 		 Returns: 0
 * 	 Description: Main function for firmware
***************************************************************************************/
int main (void)
{
	STATE nextState = IDLE; //Creates a variable of STATE type, and attributes its initial value
	Modules_Init(); //Calls function Modules_Init()

	while (1)
	{
		switch(nextState)
		{
			case IDLE:
				nextState = IdleState();
			case RECEIVING_DATA:
				nextState = ReceivingDataState();
			case DEF_SETPOINT:
				nextState = DefSetPointState();
		}
	}
	return(0);
}
