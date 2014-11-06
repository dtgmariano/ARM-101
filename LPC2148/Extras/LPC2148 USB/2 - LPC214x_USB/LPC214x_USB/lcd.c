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
#include "delay.h"
#include "lcd.h"
/*------------------------------------------------------------------------------------*/
/*                               DEFINITIONS AND MACROS                               */
/*------------------------------------------------------------------------------------*/

#define E			(1<<28)	 //!< LCD enable pin connected to LPC2148 pin P0.28
#define RS			(1<<22)	 //!< LCD register select (Command/Data) pin  connected to LPC2148 pin P0.22
#define RW			(1<<29)	 //!< LCD Read/Write pin connected to LPC2148 pin P0.29 (0 = write to LCD module, 1 = read from LCD module)
#define busyflag	(1<<13)	 //!< Does busy checking. This way is better and usually faster. Pin connected to LPC2148 pin P0.13	= D7

#define DB4		(1<<10)		//!< LCD LSB data pin connected to LPC2148 pin P0.10	35
#define DB5		(1<<11)		//!< LCD data pin connected to LPC2148 pin P0.11	37
#define DB6		(1<<12)		//!< LCD data pin connected to LPC2148 pin P0.12	38
#define DB7		(1<<13)		//!< LCD MSB data pin connected to LPC2148 pin P0.13	39
#define LUZ	    (1<<21)		//!< LCD backlight enable pin connected to LPC2148 pin P0.21	1

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
void Inicializar_LCD(void)
{
	IODIR0 |= LCD_DATA_MASK|LCD_CONTROL_MASK|LUZ;  //Configura E, RS, RW, D7, D6, D5 e D4 como saída
	IOCLR0 = LCD_DATA_MASK|LCD_CONTROL_MASK;  //Configura E, RS, RW, D7, D6, D5 e D4 como saída

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

static void strobe_e(void)
{
	IOSET0 |= E;
	delay_us(1);
	IOCLR0 |= E;
	delay_us(160);
}
