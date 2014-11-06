/* =============================== C/C++ SOURCE FILE ================================ */
/**
  \file
  \description Rotina de configuração de conversor A/D utilizado no microcontrolador LPC214x
  \author Gilson Fonseca, STHS Tecnologia - http://www.sths.com.br
  \author Fábio Vincenzi, UFU - Universidade Federal de Uberlandia - http://www.ufu.br
*/
/* ================================================================================== */

/*------------------------------------------------------------------------------------*/
/*                                      INCLUDES                                      */
/*------------------------------------------------------------------------------------*/

#include "AD_CHx.h"


/*------------------------------------------------------------------------------------*/
/*                               DEFINITIONS AND MACROS                               */
/*------------------------------------------------------------------------------------*/
#define AD0_1		(1<<1)	 //!< Seleciona o AD0.1
#define AD0_2		(1<<2)	 //!< Seleciona o AD0.2
#define AD0_3		(1<<3)	 //!< Seleciona o AD0.3
#define AD0_4		(1<<4)	 //!< Seleciona o AD0.4
#define AD0_5		(1<<5)	 //!< Seleciona o AD0.5
#define AD0_6		(1<<6)	 //!< Seleciona o AD0.6
#define AD0_7		(1<<7)	 //!< Seleciona o AD0.7

#define AD1_0		(1<<0)	 //!< Seleciona o AD0.0
#define AD1_1		(1<<1)	 //!< Seleciona o AD0.1
#define AD1_2		(1<<2)	 //!< Seleciona o AD0.2
#define AD1_3		(1<<3)	 //!< Seleciona o AD0.3
#define AD1_4		(1<<4)	 //!< Seleciona o AD0.4
#define AD1_5		(1<<5)	 //!< Seleciona o AD0.5
#define AD1_6		(1<<6)	 //!< Seleciona o AD0.6
#define AD1_7		(1<<7)	 //!< Seleciona o AD0.7

//!< 15:8 CLKDIV - The VPB clock (PCLK) is divided by (this value plus one) to produce the clock
//!< for the A/D converter, which should be less than or equal to 4.5 MHz. Typically, software should
//!< program the smallest value in this field that yields a clock of 4.5 MHz or slightly less, but
//!< in certain cases (such as a high-impedance analog source) a slower clock may be desirable.

#define CLKDIV		 	(15<<8)	 //!< O clock dos periféricos (pclk) é dividido por este valor para se obter o clock do A/D
									 //!< Se o clock estiver configurado em 60 MHz, o A/D poderá realizar até
									 //!< (60 MHz/CLKDIV) amostras por segundo.

//!< 16 BURST = 1 - The AD converter does repeated conversions at the rate selected by the CLKS field,
//!< scanning (if necessary) through the pins selected by 1s in the SEL field. The first
//!< conversion after the start corresponds to the least-significant 1 in the SEL field, then
//!< higher numbered 1-bits (pins) if applicable. Repeated conversions can be terminated by
//!< clearing this bit, but the conversion that’s in progress when this bit is cleared will be
//!< completed.
//!< Important: START bits must be 000 when BURST = 1 or conversions will not start.
//!< BURST = 0 Conversions are software controlled and require 11 clocks.

#define BURST		(1<<16)

//!< 19:17 CLKS - This field selects the number of clocks used for each conversion in Burst mode,
//!< and the number of bits of accuracy of the result in the RESULT bits of ADDR, between 11 clocks
//!< (10 bits) and 4 clocks (3 bits).

//!< 000 11 clocks / 10 bits
//!< 001 10 clocks / 9bits
//!< 010 9 clocks / 8 bits
//!< 011 8 clocks / 7 bits
//!< 100 7 clocks / 6 bits
//!< 101 6 clocks / 5 bits
//!< 110 5 clocks / 4 bits
//!< 111 4 clocks / 3 bits

#define CLKS 		(0<<17)

//!<  21 PDN  1 The A/D converter is operational.
//!<  	      0 The A/D converter is in power-down mode.

#define PDN 	    (1<<21)


//!< 26:24 START When the BURST bit is 0, these bits control whether and when an A/D conversion is started:
//!<  	   000 No start (this value should be used when clearing PDN to 0).
//!<  	   001 Start conversion now.
//!<  	   010 Start conversion when the edge selected by bit 27 occurs on P0.16/EINT0/MAT0.2/CAP0.2 pin.
//!<  	   011 Start conversion when the edge selected by bit 27 occurs on P0.22/TD3/CAP0.0/MAT0.0 pin.
//!<  	   100 Start conversion when the edge selected by bit 27 occurs on MAT0.1.
//!<  	   101 Start conversion when the edge selected by bit 27 occurs on MAT0.3.
//!<  	   110 Start conversion when the edge selected by bit 27 occurs on MAT1.0.
//!<  	   111 Start conversion when the edge selected by bit 27 occurs on MAT1.1.

#define START      (1<<24)

//!<	This bit is significant only when the START field contains 010-111. In these cases:
//!<	1 Start conversion on a falling edge on the selected CAP/MAT signal.
//!<    0 Start conversion on a rising edge on the selected CAP/MAT signal.

#define EDGE      (0<<27)

//!<------------------------------------------------------------------------------------
//!<--- Configura a interrupção do A/D

#define INT_VECT_NUM_ADC 1

// Configure ADC_START_BURST em 1 para modo BURST, em 0 para modo normal (necessário chamar o start novamente)
#define ADC_START_BURST  0

/*------------------------------------------------------------------------------------*/
/*                          TYPEDEFS, CLASSES AND STRUCTURES                          */
/*------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/*                                     PROTOTYPES                                     */
/*------------------------------------------------------------------------------------*/

static void ADC0_Interrupt(void) __attribute__ ((interrupt("IRQ")));

/*------------------------------------------------------------------------------------*/
/*                                  GLOBAL VARIABLES                                  */
/*------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/*                                  LOCAL  VARIABLES                                  */
/*------------------------------------------------------------------------------------*/
 unsigned int ValorDoAd = 0;
/*------------------------------------------------------------------------------------*/
/*                              FUNCTION  IMPLEMENTATION                              */
/*------------------------------------------------------------------------------------*/


//!< -----------------------------------------------------------------
//!< -- Rotina de Interrupção ----------------------------------------
void ADC0_Interrupt(void)
{
	static int i=0;
//	unsigned int val;
	ValorDoAd = AD0DR3; // Read A/D Data Register
	ValorDoAd = (ValorDoAd >> 6) & 0x03FF; // bit 6:15 is 10 bit AD value

	lcd_line1();
	iprintf("Contador:%-4d",i++);

	lcd_line2();
	iprintf("A/D:%-4d",ValorDoAd);

	#if	!ADC_START_BURST
		AD0CR  |= START; //0x01000000;    // Restart A/D Conversion
	#endif



	VICVectAddr = 0x00;   /* Acknowledge Interrupt */
}

//unsigned int lerad(void)
//{
//	return ValorDoAd;
//}

static inline unsigned __get_cpsr(void)
{
  unsigned long retval;
  asm volatile (" mrs  %0, cpsr" : "=r" (retval) : /* no inputs */  );
  return retval;
}

static inline void __set_cpsr(unsigned val)
{
  asm volatile (" msr  cpsr, %0" : /* no outputs */ : "r" (val)  );
}

unsigned enableIRQ(void)
{
  unsigned _cpsr;

  _cpsr = __get_cpsr();
  __set_cpsr(_cpsr & ~IRQ_MASK);
  return _cpsr;
}


//!<-----------------------------------------------------------------------
//static unsigned short ADC_Read(unsigned char ch)
//{
//unsigned int i;
//ADCR = 0x00200300 | ch;

