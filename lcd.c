#include <xc.h>
#include "lcd.h"
#define _XTAL_FREQ 8000000

// Define LCD control pins
#define RS PORTDbits.RD7
#define EN PORTDbits.RD1

void lcd_enable() {
    EN = 1;
    __delay_us(1);
    EN = 0;
    __delay_ms(2);
}

void lcd_cmd(unsigned char cmd) {
    RS = 0;
    PORTB = cmd;     // Full 8 bits to PORTC
    lcd_enable();
}

void lcd_data(unsigned char data) {
    RS = 1;
    PORTB = data;    // Full 8 bits to PORTC
    lcd_enable();
}

void lcd_init(void) {
    TRISB = 0x00;  // All PORTC as output (D0?D7)
    TRISD7 = 0;    // RS
    TRISD1 = 0;    // EN

    __delay_ms(20);    // Wait for LCD to power up
    lcd_cmd(0x38);     // 8-bit mode, 2 lines, 5x8 font
    lcd_cmd(0x0C);     // Display ON, cursor OFF
    lcd_cmd(0x06);     // Entry mode
    lcd_cmd(0x01);     // Clear display
}

void lcd_string(const char* str) {
    while(*str) lcd_data(*str++);
}

void lcd_set_cursor(unsigned char row, unsigned char col) {
    unsigned char positions[] = {0x80, 0xC0};
    lcd_cmd(positions[row] + col);
}

void lcd_clear(void) {
    lcd_cmd(0x01); // Limpa o display
    __delay_ms(2);
}