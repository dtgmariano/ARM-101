/*
 * ma2000main.c
 *
 *  Created on: 12/06/2012
 *      Author: Daniel
 */

#include "LPC214x.h"
#include "cpu_init.h"
#include "adc.h"
#include "pwm.h"
#include "uart.h"

#define rel 500000

void Config_P022_LED (void)
{
	PINSEL1 &= ~((1<<12)|(1<<13)); // Configura o pino P0.22 como entrada/saída (I/O)
	IODIR0 |= (1<<22); // Configura o pino correspondente ao bit 22 para saída
}

void Config_MCC_PINS (void)
{
	//P0.3 P0.5 P0.10 P0.12 P0.17 P0.19

	PINSEL0 &= ~((1<<6)|(1<<7)|(1<<10)|(1<<11)|(1<<20)|(1<<21)|(1<<24)|(1<<25));
	PINSEL1 &= ~((1<<2)|(1<<3)|(1<<6)|(1<<7));
	IODIR0 |= ((1<<3)|(1<<5)|(1<<10)|(1<<12)|(1<<17)|(1<<19));
}

int main (void)
{
	cpu_init();

	//ADCInit( ADC_CLK );

	unsigned int resultado1 = 0;
	unsigned int resultado2 = 0;
	unsigned int resultado3 = 0;
	unsigned int resultado4 = 0;
	unsigned int dif = 0;

	Config_MCC_PINS();

	pwm_init();
	int j=0;

	UART_Init(0,9600);
	ADCInit( ADC_CLK );
	IOSET0 = ((1<<3)|(1<<5)|(1<<10)|(1<<12)|(1<<17)|(1<<19));
	//IOCLR0 = (1<<19);
	while (1)
	{
		/*IOSET0 = (1<<3);
		IOSET0 = (1<<5);
		IOSET0 = (1<<10);
		IOSET0 = (1<<12);
		IOSET0 = (1<<17);
		IOSET0 = (1<<19);
		for (j = 0; j < rel; j++ ) asm volatile ("NOP");
		IOCLR0 = (1<<3);
		IOCLR0 = (1<<5);
		IOCLR0 = (1<<10);
		IOCLR0 = (1<<12);
		IOCLR0 = (1<<17);
		IOCLR0 = (1<<19);
		for (j = 0; j < rel; j++ ) asm volatile ("NOP");*/

		resultado1 = ADC0Read(1);
		resultado2 = ADC0Read(2);
		resultado3 = ADC0Read(3);
		resultado4 = ADC0Read(4);
		iprintf("ADC: %d\r\n", resultado1);

		if(resultado1 > resultado4)
		{
			IOCLR0 = (1<<19);
			IOSET0 = (1<<17);
			dif = resultado1 - resultado4;
		}

		if(resultado1 < resultado4)
		{
			IOCLR0 = (1<<17);
			IOSET0 = (1<<19);
			dif = resultado4 - resultado1;
		}

		setpwm5cycle(dif);




		//setpwm5cycle(resultado4); //PWM5 referente ao Fullbridge do DOF1
		//setpwm2cycle(resultado4); //PWM2 referente ao Fullbridge do DOF2
		//setpwm4cycle(resultado4); //PWM4 referente ao Fullbridge do DOF3

	}
	return(0);
}
