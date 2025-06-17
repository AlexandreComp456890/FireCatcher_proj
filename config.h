// config.h

#ifndef CONFIG_H
#define CONFIG_H

#include <xc.h>

// Frequência do cristal do microcontrolador (ou oscilador interno)
#define _XTAL_FREQ 8000000 // 8MHz (ajuste se for diferente)

// Definições de pinos para o LCD (EXEMPLO - AJUSTE PARA OS SEUS PINOS REAIS)
// Assumindo que você conectou o LCD na PORTD (ou altere para PORTA, PORTB, PORTC)
#define LCD_RS          PORTDbits.RD0   // Register Select pin (Exemplo: RD0)
#define LCD_EN          PORTDbits.RD1   // Enable pin (Exemplo: RD1)
#define LCD_D4          PORTDbits.RD2   // Data pin 4 (Exemplo: RD2)
#define LCD_D5          PORTDbits.RD3   // Data pin 5 (Exemplo: RD3)
#define LCD_D6          PORTDbits.RD4   // Data pin 6 (Exemplo: RD4)
#define LCD_D7          PORTDbits.RD5   // Data pin 7 (Exemplo: RD5)

// Direção dos pinos (TRIS registers)
#define LCD_RS_TRIS     TRISDbits.TRISD0
#define LCD_EN_TRIS     TRISDbits.TRISD1
#define LCD_D4_TRIS     TRISDbits.TRISD2
#define LCD_D5_TRIS     TRISDbits.TRISD3
#define LCD_D6_TRIS     TRISDbits.TRISD4
#define LCD_D7_TRIS     TRISDbits.TRISD5

#endif // CONFIG_H