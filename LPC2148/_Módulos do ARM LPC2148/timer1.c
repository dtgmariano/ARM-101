/* =============================== C/C++ SOURCE FILE ================================ */
/**
  \file
  \description Rotina para o uC LPC214x que espera a convers�o do A/D
  \author Gilson Fonseca, STHS Tecnologia - http://www.sths.com.br
  \author F�bio Vincenzi, UFU - Universidade Federal de Uberlandia - http://www.ufu.br
*/
/* ================================================================================== */

/*------------------------------------------------------------------------------------*/
/*                                      INCLUDES                                      */
/*------------------------------------------------------------------------------------*/

#include "LPC214x.h"
#include "iprintf.h"
#include "delay.h"
#include "lcd.h"
#include "timer1.h"


/*------------------------------------------------------------------------------------*/
/*                               DEFINITIONS AND MACROS                               */
/*------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/*                          TYPEDEFS, CLASSES AND STRUCTURES                          */
/*------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/*                                     PROTOTYPES                                     */
/*------------------------------------------------------------------------------------*/
// - Os prot�tipos que forem declarados no *.h n�o precisam ser declarados no *.C
// - No *.c devem ser declarados os prot�tipos restantes que n�o foram declarados no *.h
// sempre com o comando static para evitar que as rotinas sejam usadas globalmente, porque
// as fun��es (rotinas) s�o definidas por padr�o globalmente (extern).
// Isto �, as rotinas restantes, presentes no *.c devem ser utilizadas de prefer�ncia localmente.
// Quando se deseja definir uma vari�vel global em um arquivo *.h � mandat�rio o uso de extern
// porque se isso n�o ocorrer esta vari�vel ser� definida multiplas vezes em cada um dos arquivos
// que incluir aquele *.h

//static void T1_Periodico(void) __attribute__ ((interrupt("IRQ")));

//static void T1_Espera(void) __attribute__ ((interrupt("IRQ")));

/*------------------------------------------------------------------------------------*/
/*                                  GLOBAL VARIABLES                                  */
/*------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/*                                  LOCAL  VARIABLES                                  */
/*------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/*                              FUNCTION  IMPLEMENTATION                              */
/*------------------------------------------------------------------------------------*/

//void T1_Periodico(void)
//{
//
//	static int i = 0;
//
//	 		lcd_line1();
//			iprintf("Contador:%-4d",++i);
//	//
//	//		lcd_line2();
//	//		iprintf("A/D:%-4d",ValorDoAd);
//	//
//	//		delay_ms(200);
//	//		i++;
//
//	T1IR 		|= 0x00000002;			//Clear match 1 interrupt
//	VICVectAddr = 0x00000000;			//Dummy write to signal end of interrupt
//}
//
//
//void Timer1_Periodico(unsigned int tempo_us)
//{
//	T1PR 		= 60;        	//Load prescaler
//	T1TCR 		= 0x00000002;	//Reset counter and prescaler
//	T1MCR 		= 0x00000003;	//On match reset the counter and generate an interrupt
//
//	T1MR1		= tempo_us;	    // T1MR1 = Match Register 1,
//								// Tempo = (Prescaler (T1PR) / CLOCK dos perif�ricos (PCLK)) *  Match Register (T1MR1)
//								// Tempo = (T0PR/PCLK) * T0MR0, Ex: Tempo = (60/60 MHz) * 1000000 = 1s
//
////	T1MR1		= 0x00000000;	// Set duty cycle to zero
//	T1TCR 		= 0x00000001;	//enable timer
//
//	VICVectAddr4 = (unsigned)T1_Periodico;	// Configura o endere�o do vetor de interrup��o (ISR)
//	VICVectCntl4 = 0x00000024;		// Seleciona o vetor de controle de interrup��o (O vetor VICVectCntl0 possui a prioridade mais alta)
//	VICIntSelect &= ~(1<<4);        // Garante uma IRQ ao inv�s de uma FIQ
//	VICIntEnable |= 0x00000020;		// Habilita a interrup��o do TIMER1
//}

//------------------------------------------

void T1_Espera(void)
{

	lcd_line2();
	iprintf("Tempo Esgotado");


	T1TCR 		= 0x00000000;	//disable timer
	T1IR 		|= 0x00000002;	//Clear match 0 interrupt

	VICVectAddr = 0x00000000;	//Dummy write to signal end of interrupt
}



void Timer1_Inicializa_Espera(unsigned int tempo_us)
{
	T1PR 		= 60;        	//Load prescaler
	T1TCR 		= 0x00000002;	//Reset counter and prescaler
	T1MCR 		= 0x00000018;	//On match reset the counter and generate an interrupt

	T1MR1		= tempo_us;	    // T1MR1 = Match Register 0,
								// Tempo = (Prescaler (T0PR) / CLOCK dos perif�ricos (PCLK)) *  Match Register (T0MR0)
								// Tempo = (T1PR/PCLK) * T1MR0, Ex: Tempo = (60/60 MHz) * 1000000 = 1s

	T1TCR 		= 0x00000001;	//enable timer

	VICVectAddr5 = (unsigned)T1_Espera;	// Configura o endere�o do vetor de interrup��o (ISR)
	VICVectCntl5 = 0x00000025;		// Seleciona o vetor de controle de interrup��o (O vetor VICVectCntl0 possui a prioridade mais alta)
	VICIntSelect &= ~(1<<5);        // Garante uma IRQ ao inv�s de uma FIQ
	VICIntEnable |= 0x00000020;		// Habilita a interrup��o do TIMER1
}


//- Fun��o que l� o valor do Timer0 --------------------------

unsigned int Timer1_Le_Valor(void)
{
	return(T1TC);
}



/*------------------------------------------------------------------------------------*/
/*                                          EOF                                       */
/*------------------------------------------------------------------------------------*/
