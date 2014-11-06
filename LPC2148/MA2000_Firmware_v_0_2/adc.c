
#include "LPC214x.h"
#include "adc.h"
#include <stdint.h>


#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (1)
#endif

#define Fosc	12000000UL

/* System frequence,should be (1~32)multiples of Fosc,and should be equal or
less than 60MHz. */
#define Fcclk	(Fosc * 5)

/* CCO frequence,should be 2/4/8/16 multiples of Fcclk, ranged from 156MHz to
320MHz. */
#define Fcco	(Fcclk * 4)

/* VPB clock frequence , must be 1/2/4 multiples of (Fcclk / 4). */
#define Fpclk	(Fcclk / 1) * 1

volatile uint32_t ADC0Value[ADC_NUM], ADC1Value[ADC_NUM];
volatile uint32_t ADC0IntDone = 0, ADC1IntDone = 0;

/*****************************************************************************
** Function name:		ADCInit
**
** Descriptions:		initialize ADC channel
**
** parameters:			ADC clock rate
** Returned value:		true or false
** 
*****************************************************************************/
uint32_t ADCInit( uint32_t ADC_Clk )
{
    /* all the related pins are set to ADC inputs, AD0.1~4, AD1.0~7 */	
    //PINSEL0 = 0x0F333F00;
    //PINSEL1 = 0x15541800;

	PINSEL1 &= ~((1<<19)|(1<<25)|(1<<27)|(1<<29));
	PINSEL1 |=  ((1<<18)|(1<<24)|(1<<26)|(1<<28));	//Seleciona função AD0.1~4

    AD0CR = ( 0x01 << 0 ) | 	// SEL=1,select channel 0, 1 to 4 on ADC0
	( ( Fpclk / ADC_Clk - 1 ) << 8 ) |  // CLKDIV = Fpclk / 1000000 - 1 
	( 0 << 16 ) | 		// BURST = 0, no BURST, software controlled
	( 0 << 17 ) |  		// CLKS = 0, 11 clocks/10 bits 
	( 1 << 21 ) |  		// PDN = 1, normal operation 
	( 0 << 22 ) |  		// TEST1:0 = 00 
	( 0 << 24 ) |  		// START = 0 A/D conversion stops
	( 0 << 27 );		/* EDGE = 0 (CAP/MAT singal falling,trigger A/D 
				conversion) */
    AD1CR = ( 0x01 << 0 ) | 	// SEL=1,select channel 0, 0 to 7 on ADC1
	( ( Fpclk / ADC_Clk - 1 ) << 8 ) |  // CLKDIV = Fpclk / 1000000 - 1 
	( 0 << 16 ) | 		// BURST = 0, no BURST, software controlled 
	( 0 << 17 ) |  		// CLKS = 0, 11 clocks/10 bits 
	( 1 << 21 ) |  		// PDN = 1, normal operation 
	( 0 << 22 ) |  		// TEST1:0 = 00 
	( 0 << 24 ) |  		// START = 0 A/D conversion stops
	( 0 << 27 );		/* EDGE = 0 (CAP/MAT singal falling,trigger A/D 
				conversion) */
    /* If POLLING, no need to do the following */
    return (TRUE);
}

/*****************************************************************************
** Function name:		ADC0Read
**
** Descriptions:		Read ADC0 channel
**
** parameters:			Channel number
** Returned value:		Value read, if interrupt driven, return channel #
** 
*****************************************************************************/
uint32_t ADC0Read( uint8_t channelNum )
{
#if !ADC_INTERRUPT_FLAG
    uint32_t regVal, ADC_Data;
#endif

    /* channel number is 0 through 7 */
    if ( channelNum >= ADC_NUM )
    {
	channelNum = 0;		/* reset channel number to 0 */
    }
    AD0CR &= 0xFFFFFF00;
    AD0CR |= (1 << 24) | (1 << channelNum);	
				/* switch channel,start A/D convert */
#if !ADC_INTERRUPT_FLAG
    while ( 1 )			/* wait until end of A/D convert */
    {
	regVal = *(volatile unsigned long *)(AD0_BASE_ADDR 
			+ ADC_OFFSET + ADC_INDEX * channelNum);
				/* read result of A/D conversion */
	if ( regVal & ADC_DONE )
	{
	    break;
	}
    }	
        
    AD0CR &= 0xF8FFFFFF;	/* stop ADC now */    
    if ( regVal & ADC_OVERRUN )	/* save data when it's not overrun
				otherwise, return zero */
    {
	return ( 0 );
    }
    ADC_Data = ( regVal >> 6 ) & 0x3FF;
    return ( ADC_Data );	/* return A/D conversion value */
#else
    return ( channelNum );	/* if it's interrupt driven, the 
				ADC reading is done inside the handler.
				so, return channel number */
#endif
}

/*****************************************************************************
** Function name:		ADC1Read
**
** Descriptions:		Read ADC1 channel
**
** parameters:			Channel number
** Returned value:		Value read, if interrupt driven, return channel #
** 
*****************************************************************************/
uint32_t ADC1Read( uint8_t channelNum )
{
#if !ADC_INTERRUPT_FLAG
    uint32_t regVal;
    uint32_t ADC_Data;
#endif

    /* channel number is 0 through 7 */
    if ( channelNum >= ADC_NUM )
    {
	channelNum = 0;		/* reset channel number to 0 */
    }
    AD1CR &= 0xFFFFFF00;
    AD1CR |= (1 << 24) | (1 << channelNum);	
				/* switch channel,start A/D convert */
#if !ADC_INTERRUPT_FLAG
    while ( 1 )			/* wait until end of A/D convert */
    {
	regVal = *(volatile unsigned long *)(AD1_BASE_ADDR 
			+ ADC_OFFSET + ADC_INDEX * channelNum);
				/* read result of A/D conversion */
	if ( regVal & ADC_DONE )
	{
	    break;
	}
    }	
        
    AD1CR &= 0xF8FFFFFF;	/* stop ADC now */
    if ( regVal & ADC_OVERRUN )	/* save data when it's not overrun
				otherwise, return zero */
    {
	return ( 0 );
    }
    
    ADC_Data = ( regVal >> 6 ) & 0x3FF;
    return ( ADC_Data );	/* return A/D conversion value */
#else
    return ( channelNum );
#endif
}
