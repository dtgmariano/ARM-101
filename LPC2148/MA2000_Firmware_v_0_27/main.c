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
 * 				one by a DC Motor with the following configurations: A PWM at 20000 Hz with
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


/*------------------------------------------------------------------------------------*/
/*                         	   VARIABLES DECLARATION                                  */
/*------------------------------------------------------------------------------------*/
typedef enum {IDLE, RECEIVING_DATA, SENDING_DATA, CONTROLLER} STATE;
int size = 0;
int data[15] = {0};
uint8_t buffer[100] = {0};
uint32_t adc[3] = {0}, pos[3] = {0}, kp[3] = {0}, ki[3] = {0}, kd[3] = {0}, ort[3] = {0}, e[3];



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
//	if(iCountTimer0 >= ort[0])
//		IOCLR1 = (1<<x);
//	if(iCountTimer0 >= ort[1])
//		IOCLR1 = (1<<x);
//	if(iCountTimer0 >= ort[2])
//		IOCLR1 = (1<<x);
//	if(iCountTimer0 >= PWMSERVO_COUNT)
//	{
//		IOSET1 = (1<<x);
// 		IOSET1 = (1<<x);
//		IOSET1 = (1<<x);
//		iCountTimer0 = 0;
//	}
//	iCountTimer0++;
}


/*------------------------------------------------------------------------------------*/
/*                               FUNCTIONS  IMPLEMENTATION                            */
/*------------------------------------------------------------------------------------*/

#define PLOCK 0x400

static void feed(void)
{
  PLLFEED = 0xAA;
  PLLFEED = 0x55;
}

/***************************************************************************************
 * Function Name: Modules_Init(void)
 * 		 	Type: Void
 *	  Parameters:
 * 		 Returns: Void
 * 	 Description: Initiates CPU configurations,
 * 				  Configure Timer, PWM, ADC, UART Modules,
 * 				  Sets GPIO Pins.
***************************************************************************************/
void delay()
{
  for(int i = 0; i < 1000000; i++)
  {
    asm volatile ("NOP");
    asm volatile ("NOP");
    asm volatile ("NOP");
    asm volatile ("NOP");
    asm volatile ("NOP");
    asm volatile ("NOP");
    asm volatile ("NOP");
  }
}
void Initialize(void)
{
	PLLCFG = 0x24; //Configura o multiplicador PLL para 4 e o divisor para 2
             	   //Assim com o cristal de 12MHz temos um CCLK = 48MHz e um FCCO = 192MHz
    feed();

    PLLCON = 0x01; //Habilita o bloco PLL
    feed();      //Para que as alterações anteriores tenham efeito

    while(!(PLLSTAT & PLOCK)) ; //Garante que a alteração anterior teve efeito uma vez que quando
                    //o bit PLOCK do reg. PLLSTAT vale zero ouve um bloqueio no PLL que
                    //ocorre logo após a modificação dos registradores

    PLLCON = 0x03; //Depois de habilitado resta conectá-lo a fonte de clock para que comece o trabalho
    feed();        //Necessário para que as alterações anteriores tenham efeito

    MAMCR = 0x02;  //Habilita todas as funções do Módulo acelerador de memória
    MAMTIM = 0x04;  //O ciclo de busca do MAM tem duração de 4 ciclos
    VPBDIV = 0x01;  //Clock dos periféricos igual ao clock do Sistema
}

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
	Initialize();
	ADC_Init(ADC_CLK);
	UART_Init(0,9600);
	PWM_Init();
	GPIO_Init();
//	Timer_Init();
//	Timer_RegisterCallback(PWM_InterruptControl);
//	Timer_Enable();
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
	size = UART_GetNumberAvailableElements();
	if(size >= 60)
	{
		UART_ReceiveBuffer(0,buffer,60);
	  	memcpy(data,buffer,sizeof(int)*15);
	  	pos[0] = data[0];
	  	pos[1] = data[1];
	  	pos[2] = data[2];
	  	ort[0] = data[3];
	  	ort[1] = data[4];
	  	ort[2] = data[5];
	  	kp[0] = data[6];
	  	ki[0] = data[7];
	  	kd[0] = data[8];
	  	kp[1] = data[9];
	  	ki[1] = data[10];
	  	kd[1] = data[11];
	  	kp[2] = data[12];
	  	ki[2] = data[13];
	  	kd[2] = data[14];
	}
	return SENDING_DATA;
}

/***************************************************************************************
 * Function Name: SendingData(void)
 * 		 	Type: STATE
  *	  Parameters:
 * 		 Returns:
 * 	 Description:
***************************************************************************************/
STATE SendingDataState(void)
{
//	static int count_sending_data = 0;
//
//	if(count_sending_data == 250000)
//	{
//		adc[0] = ADC0Read(3);
//		adc[1] = ADC0Read(1);
//		adc[2] = ADC0Read(2);
//		//UART_SendBuffer(0,adc,(sizeof(unsigned long))*3);
//		//iprintf("GDL2: %d %d %d \r\n",pos[1], adc[1], e[1]);
//		iprintf("GDL3: POS %d ADC %d ERRO %d \r\n",pos[2], adc[2], e[2]);
//		//iprintf("ADC: %d %d %d \r\n",adc[0], adc[1], adc[2]);
//		count_sending_data = 0;
//	}
//	count_sending_data++;
	return CONTROLLER;
}
/***************************************************************************************
 * Function Name: Controller(void)
 * 		 	Type: STATE
 *	  Parameters:
 * 		 Returns:
 * 	 Description:
***************************************************************************************/
STATE Controller(void)
{
//	if((pos[0]>adc[0]) || adc[0] < 50)
//	{
//		IOCLR0 = (1<<22);
//		IOSET0 = (1<<23);
//		e[0] = pos[0] - adc[0];
//	}
//	if((pos[0]<adc[0]) || adc[0] > 970)
//	{
//		IOCLR0 = (1<<23);
//		IOSET0 = (1<<22);
//		e[0] = adc[0] - pos[0];
//	}
//	setDutyCycle(5,(unsigned int)1020);  //DOF 1

//	if((pos[1]>adc[1]) || adc[1] < 50)
//	{
//		IOCLR0 = (1<<17);
//		IOSET0 = (1<<19);
//		e[1] = pos[1] - adc[1];
//	}
//	if((pos[1]<adc[1]) || adc[1] > 970)
//	{
//		IOCLR0 = (1<<19);
//		IOSET0 = (1<<17);
//		e[1] = adc[1] - pos[1];
//	}
//	setDutyCycle(4,e[1]);  //DOF 2

	if((pos[2]>adc[2]) || adc[2] < 50)
	{
		IOCLR0 = (1<<6);
		IOSET0 = (1<<9);
		e[2] = pos[2] - adc[2];
	}
	if((pos[2]<adc[2]) || adc[2] > 970)
	{
		IOCLR0 = (1<<9);
		IOSET0 = (1<<6);
		e[2] = adc[2] - pos[2];
	}

	setDutyCycle(2,e[2]);  //DOF 3

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
			case SENDING_DATA:
				nextState = SendingDataState();
			case CONTROLLER:
				nextState = Controller();
		}
	}
	return(0);
}
