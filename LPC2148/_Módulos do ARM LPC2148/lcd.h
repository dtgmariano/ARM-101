/*
 * lcd.h
 *
 *  Created on: 12/03/2010
 *      Author: Fábio
 */

#ifndef LCD_H_
#define LCD_H_

/*------------------------------------------------------------------------------------*/
/*                                     PROTOTYPES                                     */
/*------------------------------------------------------------------------------------*/
void lcd_line1(void);
void lcd_string(char *senpoint);
void lcd_line2(void);
void lcd_cmd(unsigned char letter);
void lcd_char(unsigned char letter);
void lcd_nybble(unsigned char nyb,unsigned char rs);
void lcd_init(void);




#endif /* LCD_H_ */
