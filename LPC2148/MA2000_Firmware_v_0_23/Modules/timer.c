/*
 * timer.c
 *
 *  Created on: 18/03/2013
 *      Author: Daniel
 */

/*------------------------------------------------------------------------------------*/
/*                                      INCLUDES                                      */
/*------------------------------------------------------------------------------------*/
#include "Modules/timer.h"
#include "stdio.h"

/*------------------------------------------------------------------------------------*/
/*                               DEFINITIONS AND MACROS                               */
/*------------------------------------------------------------------------------------*/
#define PRESCALEREGISTER 30
// Tempo = (PCLK/T0PR) * T0MR0, Ex: Tempo = (60 MHz/30) * 1000000 / 2 (Dois ciclos) = 1s
//#define MATCHREGISTER0 4
#define MATCHREGISTER0 8


/*------------------------------------------------------------------------------------*/
/*                                     PROTOTYPES                                     */
/*------------------------------------------------------------------------------------*/
void T0isr(void) __attribute__ ((interrupt("IRQ")));//atribui a funcao a interrução IRQ


/*------------------------------------------------------------------------------------*/
/*                              FUNCTION  IMPLEMENTATION                              */
/*------------------------------------------------------------------------------------*/


void (*mptrfunc)(void);

void T0isr(void)
{
	if(mptrfunc != NULL)
	{
		mptrfunc();
	}

	T0IR 		|= 0x00000001;			//Clear match 0 interrupt
	VICVectAddr  = 0x00000000;			//Dummy write to signal end of interrupt
}

void Timer_Init(void)
{
	T0PR = PRESCALEREGISTER; //incrementa o contador, quando o contador prescale atinge o valor de PR.

	T0TCR = 0x00000002;	    //desabilita o contador e reseta o
	T0MCR = 0x00000003;	    //On match reset the counter and generate an interrupt
	T0MR0 = MATCHREGISTER0;  // T0MR0 = Match Register 0,
						    // Tempo = (CLOCK dos periféricos (PCLK) / Prescaler (T0PR)) *  Match Register (T0MR0)

	T0MR1 = 0x00000000;		// Set duty cycle to zero
//	T0TCR = 0x00000001;		//enable timer

	VICVectAddr4 = (unsigned)T0isr;		//Set the timer ISR vector address
	VICVectCntl4 = 0x00000024;			//Set channel
	VICIntSelect &= ~(1<<4);			//seleciona a se a interrupção sera FIQ ou IRQ para o timmer0
	//VICIntEnable |= 0x00000010;			//habilita a interrupção de timer 0
}

void Timer_RegisterCallback(void (*ptrfunc)(void))
{
	//VICVectaddr4 = ptrfunc;
	mptrfunc = ptrfunc;
}

void Timer_Enable()
{
	VICIntEnable |= 0x00000010;
	T0TCR = 0x00000001;
}
