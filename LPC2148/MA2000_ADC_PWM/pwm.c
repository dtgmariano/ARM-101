#include "pwm.h"
#include "LPC214x.h"
#include "cpu_init.h"

void pwm_init()
{

   //PINSEL0 &= ~((3<<0)|(3<<2)|(3<<14));	//Seleciona funções PWM1, PWM2, PWM3
   //PINSEL0 |=  ((1<<1)|(1<<3)|(1<<15));
   PINSEL0 &= ~((1<<14)|(1<<16));	//Seleciona funções PWM2 e 4
   PINSEL0 |=  ((1<<15)|(1<<17));
   PINSEL1 &= ~(1<<11); //Seleciona função PWM5
   PINSEL1 |=  (1<<10);

   PWMTCR = 0x02;       // reset PWM
   PWMMR0 = 48000; 		// set PWM period (400uS -> 2,5khz)
   PWMPR  = 0;          // prescaler desativado
   PWMPCR |= ((1<<13)|(1<<12)|(1<<10));   // ativa PWM 2,4,5 como saída de borda simples
   PWMTCR = 00001001;   // counter and PWM enabled
   PWMMR2 = 12000; 		// seta PWM Duty Cicle
   PWMMR4 = 24000; 		// seta PWM Duty Cicle
   PWMMR5 = 36000; 		// seta PWM DOF1 Duty Cicle
   PWMMCR = 0x0000003;  // reset on Match 0
}

void setpwm2cycle(unsigned int raz)
{
	PWMMR2 = ((48000/1024)*(raz));
	PWMLER |= ((1<<2)|(1<<4)|(1<<5));
}

void setpwm4cycle(unsigned int raz)
{
	PWMMR4 = ((48000/1024)*(raz));
	PWMLER |= ((1<<2)|(1<<4)|(1<<5));
}

void setpwm5cycle(unsigned int raz)
{
	PWMMR5 = ((48000/1024)*(raz));
	PWMLER |= ((1<<2)|(1<<4)|(1<<5));
}

