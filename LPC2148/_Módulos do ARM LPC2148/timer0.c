/* =============================== C/C++ SOURCE FILE ================================ */
/**
  \file
  \description Rotina para o uC LPC214x que espera a conversão do A/D
  \author Gilson Fonseca, STHS Tecnologia - http://www.sths.com.br
  \author Fábio Vincenzi, UFU - Universidade Federal de Uberlandia - http://www.ufu.br
*/
/* ================================================================================== */

/*------------------------------------------------------------------------------------*/
/*                                      INCLUDES                                      */
/*------------------------------------------------------------------------------------*/

#include "LPC214x.h"
#include "iprintf.h"
#include "delay.h"
#include "lcd.h"
#include "timer0.h"


/*------------------------------------------------------------------------------------*/
/*                               DEFINITIONS AND MACROS                               */
/*------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/*                          TYPEDEFS, CLASSES AND STRUCTURES                          */
/*------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/*                                     PROTOTYPES                                     */
/*------------------------------------------------------------------------------------*/
// - Os protótipos que forem declarados no *.h não precisam ser declarados no *.C
// - No *.c devem ser declarados os protótipos restantes que não foram declarados no *.h
// sempre com o comando static para evitar que as rotinas sejam usadas globalmente, porque
// as funções (rotinas) são definidas por padrão globalmente (extern).
// Isto é, as rotinas restantes, presentes no *.c devem ser utilizadas de preferência localmente.
// Quando se deseja definir uma variável global em um arquivo *.h é mandatório o uso de extern
// porque se isso não ocorrer esta variável será definida multiplas vezes em cada um dos arquivos
// que incluir aquele *.h

static void T0_Periodico(void) __attribute__ ((interrupt("IRQ")));

static void T0_Espera(void) __attribute__ ((interrupt("IRQ")));

/*------------------------------------------------------------------------------------*/
/*                                  GLOBAL VARIABLES                                  */
/*------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/*                                  LOCAL  VARIABLES                                  */
/*------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/*                              FUNCTION  IMPLEMENTATION                              */
/*------------------------------------------------------------------------------------*/

void T0_Periodico(void)
{
//	T0EMR 		|= 0x00000002;			//Set MAT1 high for begining of the cycle
	static int i = 0;

	 		lcd_line1();
			iprintf("Contador:%-4d",++i);
	//
	//		lcd_line2();
	//		iprintf("A/D:%-4d",ValorDoAd);
	//
	//		delay_ms(200);
	//		i++;

	T0IR 		|= 0x00000001;			//Clear match 0 interrupt
	VICVectAddr = 0x00000000;			//Dummy write to signal end of interrupt
}


void Timer0_Periodico(unsigned int tempo_us)
{
	T0PR 		= 60;        	//Load prescaler
	T0TCR 		= 0x00000002;	//Reset counter and prescaler
	T0MCR 		= 0x00000003;	//On match reset the counter and generate an interrupt

	T0MR0		= tempo_us;	    // T0MR0 = Match Register 0,
								// Tempo = (Prescaler (T0PR) / CLOCK dos periféricos (PCLK)) *  Match Register (T0MR0)
								// Tempo = (T0PR/PCLK) * T0MR0, Ex: Tempo = (60/60 MHz) * 1000000 = 1s

//	T0MR1		= 0x00000000;	// Set duty cycle to zero
	T0TCR 		= 0x00000001;	//enable timer

	VICVectAddr4 = (unsigned)T0_Periodico;	// Configura o endereço do vetor de interrupção (ISR)
	VICVectCntl4 = 0x00000024;		// Seleciona o vetor de controle de interrupção (O vetor VICVectCntl0 possui a prioridade mais alta)
	VICIntSelect &= ~(1<<4);        // Garante uma IRQ ao invés de uma FIQ
	VICIntEnable |= 0x00000010;		// Habilita a interrupção do TIMER0
}

//------------------------------------------

void T0_Espera(void)
{
//	static int i = 0;

	// 		lcd_line1();
	//		iprintf("Contador:%-4d",++i);
	//
			lcd_line2();
			iprintf("Tempo Esgotado");
	//		iprintf("A/D:%-4d",ValorDoAd);
	//
	//		delay_ms(200);
	//		i++;


	T0TCR 		= 0x00000000;	//enable timer
	T0IR 		|= 0x00000001;	//Clear match 0 interrupt
	VICVectAddr = 0x00000000;	//Dummy write to signal end of interrupt
}



void Timer0_Inicializa_Espera(unsigned int tempo_us)
{
	T0PR 		= 60;        	//Load prescaler
	T0TCR 		= 0x00000002;	//Reset counter and prescaler
	T0MCR 		= 0x00000003;	//On match reset the counter and generate an interrupt

	T0MR0		= tempo_us;	    // T0MR0 = Match Register 0,
					    // Tempo = (Prescaler (T0PR) / CLOCK dos periféricos (PCLK)) *  Match Register (T0MR0)
					    // Tempo = (T0PR/PCLK) * T0MR0, Ex: Tempo = (60/60 MHz) * 1000000 = 1s

	T0TCR 		= 0x00000001;	//enable timer

	VICVectAddr4 = (unsigned)T0_Espera;	// Configura o endereço do vetor de interrupção (ISR)
	VICVectCntl4 = 0x00000024;		// Seleciona o vetor de controle de interrupção (O vetor VICVectCntl0 possui a prioridade mais alta)
	VICIntSelect &= ~(1<<4);                // Garante uma IRQ ao invés de uma FIQ
	VICIntEnable |= 0x00000010;		// Habilita a interrupção do TIMER0
}


//- Função que lê o valor do Timer0 --------------------------

unsigned int Timer0_Le_Valor(void)
{
	return(T0TC);
}



/*------------------------------------------------------------------------------------*/
/*                                          EOF                                       */
/*------------------------------------------------------------------------------------*/
