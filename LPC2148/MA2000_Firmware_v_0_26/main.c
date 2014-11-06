#include <string.h>
#include "LPC214x.h"
#include "cpu_init.h"
#include "Modules/adc.h"
#include "Modules/pwm.h"
#include "Modules/gpio.h"
#include "Modules/uart.h"
#include "Modules/iprintf.h"
#include "Modules/timer.h"

unsigned int DOF1_SETPOINT, DOF2_SETPOINT, DOF3_SETPOINT;
unsigned int DOF1_ADC, DOF2_ADC, DOF3_ADC;
unsigned int DOF1_ERROR, DOF2_ERROR, DOF3_ERROR;

int main (void)
{
	cpu_init();
	PWM_Init();
	ADC_Init(ADC_CLK);
	GPIOInit();

	DOF1_SETPOINT = 800;
	DOF2_SETPOINT = 800;
	DOF3_SETPOINT = 200;

	while(1)
	{
		DOF2_ADC = ADC0Read(1);  //dof 2
		DOF3_ADC = ADC0Read(2);  //dof 3
		DOF1_ADC = ADC0Read(3);  //dof 1
		DOF1_SETPOINT = DOF2_ADC;
		DOF3_SETPOINT = DOF2_ADC;
		setDutyCycle(2,1000);  //dof 3
		setDutyCycle(4,1000);  //dof 2
		setDutyCycle(5,1000); //dof 1

		IOCLR0 = (1<<22);
		IOSET0 = (1<<23);
		IOCLR0 = (1<<17);
		IOSET0 = (1<<19);

//		if((DOF1_SETPOINT>DOF1_ADC) || DOF1_ADC < 50)
//		{
//			IOCLR0 = (1<<22);
//			IOSET0 = (1<<23);
//			DOF1_ERROR = DOF1_SETPOINT - DOF1_ADC;
//		}
//		if((DOF1_SETPOINT<DOF1_ADC) || DOF1_ADC > 970)
//		{
//			IOCLR0 = (1<<23);
//			IOSET0 = (1<<22);
//			DOF1_ERROR = DOF1_ADC - DOF1_SETPOINT;
//		}
//
//		if((DOF2_SETPOINT>DOF2_ADC) || DOF2_ADC < 50)
//		{
//			IOCLR0 = (1<<17);
//			IOSET0 = (1<<19);
//			DOF2_ERROR = DOF2_SETPOINT - DOF2_ADC;
//		}
//		if((DOF2_SETPOINT<DOF2_ADC) || DOF2_ADC > 970)
//		{
//			IOCLR0 = (1<<19);
//			IOSET0 = (1<<17);
//			DOF2_ERROR = DOF2_ADC - DOF2_SETPOINT;
//		}
//		if((DOF3_SETPOINT>DOF3_ADC) || DOF3_ADC < 50)
//		{
//			IOCLR0 = (1<<6);
//			IOSET0 = (1<<9);
//			DOF3_ERROR = DOF3_SETPOINT - DOF3_ADC;
//		}
//		if((DOF3_SETPOINT<DOF3_ADC) || DOF3_ADC > 970)
//		{
//			IOCLR0 = (1<<9);
//			IOSET0 = (1<<6);
//			DOF3_ERROR = DOF3_ADC - DOF3_SETPOINT;
//		}

//		IOSET0 |= (1<<6);  //Input 1A (DOF1)
//		IOSET0 |= (1<<9);  //Input 1B (DOF1)
//		IOSET0 |= (1<<17); //Input 2A (DOF2)
//		IOSET0 |= (1<<19); //Input 2B (DOF2)
//		IOSET0 |= (1<<22); //Input 3A (DOF3)
//		IOSET0 |= (1<<23); //Input 3B (DOF3)
//		for(int i=0; i<5000; i++)
//		{
//			asm volatile("NOP");
//		}
//		IOCLR0 |= (1<<6);  //Input 1A (DOF1)
//		IOCLR0 |= (1<<9);  //Input 1B (DOF1)
//		IOCLR0 |= (1<<17); //Input 2A (DOF2)
//		IOCLR0 |= (1<<19); //Input 2B (DOF2)
//		IOCLR0 |= (1<<22); //Input 3A (DOF3)
//		IOCLR0 |= (1<<23); //Input 3B (DOF3)
//		for(int i=0; i<5000; i++)
//		{
//			asm volatile("NOP");
//		}
	}
}
