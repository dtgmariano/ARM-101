/*
 * timer.c
 *
 *  Created on: 4 de Mar de 2013
 *      Author: ufu
 */

#include "LPC214x.h"
#include "timer.h"

#define Fclk 6000000
#define Ffreq 50
static unsigned long int tFreq;

void timer0Init(unsigned long int freq)
{
   tFreq=freq;
   T0PR=Fclk/Ffreq;         //Set up the prescaler for a 'freq' frequency
   T0CTCR=0;               //Timer Mode
   T0TCR |=0X01;            //Enable the clock
   //T0MCR=0x0003;            //Interrupt and Reset Timer on Match
   //T0MR0=(50000/TIMER_FREQ);
}

void timer0Match(int mr, unsigned long int freq, char mode)
{
   unsigned long int mfreq = tFreq/freq;      //Get the Match Register value by dividing timer frequency by request match frequency

   switch(mr)
   {
      case 0:
         T0MR0 = mfreq;
         T0MCR = (mode<<0);
         break;
      case 1:
         T0MR1 = mfreq;
         T0MCR = (mode<<3);
         break;
      case 2:
         T0MR2 = mfreq;
         T0MCR = (mode<<6);
         break;
      case 3:
         T0MR3 = mfreq;
         T0MCR = (mode<<9);
         break;
      default:
         break;
   }
}

char timer0IntFlag=0;
long int ms=0;

//Usage: None (Automatically Called by FW)
//Inputs: None
//This function is a global interrupt called by a match on the Timer 0 match.
void ISR_Timer0(void)
{
   printf("timer0 interrupt\n");
   //Interrupt Code Here
   timer0IntFlag+=1;

   //Keep track of the number of milliseconds
   ms++;

   //Clear the interrupt and update the VIC priority
   T0IR = 0x01;
   VICVectAddr =0;
}
