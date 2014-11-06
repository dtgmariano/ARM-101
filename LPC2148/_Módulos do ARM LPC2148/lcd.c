/*
 * lcd.c
 *
 *  Created on: 12/03/2010
 *      Author: Fábio
 */

/*------------------------------------------------------------------------------------*/
/*                                      INCLUDES                                      */
/*------------------------------------------------------------------------------------*/

#include "LPC214x.h"
#include "delay_loop.h"
#include "lcd.h"

/*------------------------------------------------------------------------------------*/
/*                               DEFINITIONS AND MACROS                               */
/*------------------------------------------------------------------------------------*/

#define E			(1<<28)	 //!< Pino que liga/desliga o LCD, conectado no pino P0.20 do LPC2148
#define RS			(1<<22)	 //!< Informa ao LCD se a informação é um comando ou dado, conectado no pino P0.22 do LPC2148
#define RW			(1<<29)	 //!< Pino que informa se a operação é de leitura ou escrita, conectado no pino P0.29 do LPC2148 (0 = write to LCD module, 1 = read from LCD module)
//#define busyflag	(1<<13)	 //!< Verifica se o LCD está ocupado, isto é pronto para receber o próximo comando.

#define DB4		(1<<10)		//!< Pino de dado DB4 (LSB), conectado ao pino P0.10 do LPC2148
#define DB5		(1<<11)		//!< Pino de dado DB5, conectado ao pino P0.11 do LPC2148
#define DB6		(1<<12)		//!< Pino de dado DB6, conectado ao pino P0.12 do LPC2148
#define DB7		(1<<13)		//!< Pino de dado DB4 (MSB), conectado ao pino P0.13 do LPC2148
#define LUZ	    (1<<21)		//!< Liga/desliga a backlight do LCD, conectado ao pino P0.21 do LPC2148

#define LCD_DATA_MASK		(DB4 | DB5 | DB6 | DB7)
#define LCD_CONTROL_MASK 	(E | RW | RS)

/*------------------------------------------------------------------------------------*/
/*                          TYPEDEFS, CLASSES AND STRUCTURES                          */
/*------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/*                                     PROTOTYPES                                     */
/*------------------------------------------------------------------------------------*/
static void strobe_e(void);


//---------------------------------------------------------------------
void lcd_line1(void)
{
	lcd_cmd(0x80);
}

/**
 * Writes a string to LCD
 * @param[in] senpoint a pointer to a null-terminated string
 */
void lcd_string(char *senpoint)
{
	while(*senpoint != '\0')
	{
		lcd_char(*senpoint);
		senpoint++;
	}
}



//---------------------------------------------------------------------
void lcd_line2(void)
{
	lcd_cmd(0xc0);

}

//---------------------------------------------------------------------
void lcd_cmd(unsigned char letter)
{
	unsigned char temp;
	temp=letter;
	temp=temp>>4;
	lcd_nybble(temp,0);
	temp=letter;
	temp=temp&0x0f;
	lcd_nybble(temp,0);
}

//---------------------------------------------------------------------
void lcd_char(unsigned char letter)
{
	unsigned char temp;
	temp=letter;
	temp=temp>>4;
	lcd_nybble(temp,1);
	temp=letter;
	temp=temp&0x0f;
	lcd_nybble(temp,1);
}

//---------------------------------------------------------------------
void lcd_nybble(unsigned char nyb,unsigned char rs)
{
	int dat;
	if(rs)
		IOSET0 |= RS;	//set RS pin
	else
		IOCLR0 |= RS;	//clear RS pin
	dat = nyb;			//get the nybble in an int
	IOCLR0 = LCD_DATA_MASK;   //clear D4-D7
	IOPIN0 |= dat<<10;	//OR the bits in there
	strobe_e();			//latch data to LCD
}

//---------------------------------------------------------------------
void lcd_init(void)
{
	PINSEL0 &= ~((1<<20)|(1<<21)|(1<<22)|(1<<23)|(1<<24)|(1<<25)|(1<<26)|(1<<27)); // P0.10, P0.11, P0.12 e P0.13 como entrada/saída (2 bits de configuração cada pino)
	PINSEL1 &= ~((1<<10)|(1<<11)|(1<<12)|(1<<13)|(1<<24)|(1<<25)|(1<<26)|(1<<27)); // P0.21, P0.22, P0.28 e P0.29 como entrada/saída (2 bits de configuração cada pino)

	IODIR0 |= LCD_DATA_MASK|LCD_CONTROL_MASK|LUZ;  //Configura E, RS, RW, D7, D6, D5 e D4 como saída
	IOCLR0 = LCD_DATA_MASK|LCD_CONTROL_MASK;  // Força nível lógico baixo nos pinos E, RS, RW, D7, D6, D5 e D4

	delay_ms(500);		 //settle time delay
	lcd_nybble(0x03,0);	 //reset LCD
	delay_ms(5);
	strobe_e();
	delay_us(160);
	strobe_e();
	delay_us(160);
	lcd_nybble(0x02,0);
	delay_us(160);
	lcd_cmd(0x28);	//set 4-bit mode and 2 lines
	delay_us(160);
	lcd_cmd(0x10);	//cursor move & shift left
	delay_us(160);
	lcd_cmd(0x06);	//entry mode = increment
	delay_us(160);
	lcd_cmd(0x0e);	//display on - cursor blink on
	delay_us(160);
	lcd_cmd(0x01);	//clear display
	delay_ms(40);
	IOSET0 |= LUZ; 	// Liga a luz do display
}

//---------------------------------------------------------------------
//void lcd_busy(void)
//{
//	IODIR0 = LCD_CONTROL_MASK;	//D7, D6, D5 e D4 são entrada e R, RS, RW são saída
//	IOCLR0 |= RS;				//set RS low
//	IOSET0 |= RW;				//set R/W high
//	IOSET0 |= E;					//set E high
//	while(IOPIN0 & busyflag);		//wait for busy flag to go low
//	IOCLR0 |= E;					//set E low
//	IOCLR0 |= RW;				//set R/W low
//	IODIR0 = 0x107e0000;			//D7 is output again
//}

//---------------------------------------------------------------------

static void strobe_e(void)
{
	IOSET0 |= E;
	delay_us(1);
	IOCLR0 |= E;
	delay_us(160);
//	lcd_busy();
}
