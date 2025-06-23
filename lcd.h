// lcd.h

#ifndef LCD_H
#define	LCD_H

#include <xc.h>

// Defini��o das fun��es p�blicas do LCD
void lcd_init(void);
void lcd_string(const char*);
void lcd_set_cursor(unsigned char row, unsigned char col);
void lcd_clear(void);

#endif	/* LCD_H */