// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef _LCD_H
#define	_LCD_H

#include <xc.h> // include processor files - each processor file is guarded.
#include "dateandtime.h"

#define _XTAL_FREQ 64000000             // Note intrinsic _delay function is 62.5ns at 64,000,000Hz  

// Define Pins Used with the LCD Screen
#define LCD_RS LATCbits.LATC6           // H: Data, L: Instruction code
#define LCD_E LATCbits.LATC2            // Chip enable signal (LCD enable bit)
#define LCD_DB4 LATBbits.LATB3          // Data bus line
#define LCD_DB5 LATBbits.LATB2          // Data bus line
#define LCD_DB6 LATEbits.LATE3          // Data bus line
#define LCD_DB7 LATEbits.LATE1          // Data bus line

// Include all functions from the corresponding source file
void LCD_E_TOG(void);
void LCD_sendnibble(unsigned char number);
void LCD_sendbyte(unsigned char Byte, char type);
void LCD_init(void);
void LCD_setline (char line);	
void LCD_sendstring(char *string);
void LCD_scroll(void);
void LCD_clear(void);
void ADC2String(char *buf, unsigned int number);

#endif	/* _LCD_H */