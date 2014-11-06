/* =============================== C/C++ HEADER FILE ================================ */
/**
	\file
	\description
	\copyright (c) LASEC www.lasec.feelt.ufu.br
	
*/
/* ================================================================================== */
/*
CHANGES:
---------------
Date            Author       Description
27/03/2011      felipe       v1.0 Released
07/04/2011      fabiovince   v1.1
*/

#include "cpu_init.h"

//--------------------------------------------------------------------------------------
//--- delay_ms implementado com loops --------------------------------------------------
#if (cpuMHz == cristal_12MHz_cpu_60MHz)
void delay_ms(int tempo)
{
  int j;
  while(tempo--)
  {
    for(j = 0 ; j <= 3295 ; j++)
      asm volatile ("NOP");
  }
}
#elif (cpuMHz == cristal_12MHz_cpu_48MHz)
void delay_ms(int tempo)
{
  int j;
  while(tempo--)
  {
    for(j = 0 ; j <= 2636; j++)
      asm volatile ("NOP");
  }
}
#elif (cpuMHz == cristal_12MHz_cpu_36MHz)
void delay_ms(int tempo)
{
  int j;
  while(tempo--)
  {
    for(j = 0 ; j <= 1977; j++)
      asm volatile ("NOP");
  }
}
#elif (cpuMHz == cristal_12MHz_cpu_24MHz)
void delay_ms(int tempo)
{
  int j;
  while(tempo--)
  {
    for(j = 0 ; j <= 1318; j++)
      asm volatile ("NOP");
  }
}
#elif (cpuMHz == cristal_12MHz_cpu_12MHz)
void delay_ms(int tempo)
{
  int j;
  while(tempo--)
  {
    for(j = 0 ; j <= 659; j++)
      asm volatile ("NOP");
  }
}
#endif


//--------------------------------------------------------------------------------------
//--- delay_us implementado com loops --------------------------------------------------
#if (cpuMHz == cristal_12MHz_cpu_60MHz)
void delay_us(int tempo)
{
  while(tempo--)
  {
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
  }
}
#elif (cpuMHz == cristal_12MHz_cpu_48MHz)
void delay_us(int tempo)
{
  while(tempo--)
  {
	  asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
  }
}
#elif (cpuMHz == cristal_12MHz_cpu_36MHz)
void delay_us(int tempo)
{
  while(tempo--)
  {
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
  }
}
#elif (cpuMHz == cristal_12MHz_cpu_24MHz)
void delay_us(int tempo)
{
	tempo = tempo/2;
	while(tempo--){
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	   asm volatile ("NOP");
	}
}
#elif (cpuMHz == cristal_12MHz_cpu_12MHz)
void delay_us(int tempo)
{
	tempo = tempo/2;
	while(tempo--){
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
      asm volatile ("NOP");
	}
}
#endif
