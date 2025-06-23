// config.h

#ifndef CONFIG_H
#define CONFIG_H

#include <xc.h>

// Frequência do cristal do microcontrolador 
#define _XTAL_FREQ 8000000 // 8MHz 
#define INIT_INTERRUPTS() \
    do { \
        INTCONbits.GIE = 1; \
        INTCONbits.PEIE = 1; \
        INTCONbits.TMR0IE = 1; \
        INTCONbits.TMR0IF = 0; \
        PIE1bits.ADIE = 1; \
        PIR1bits.ADIF = 0; \
    } while (0)


#endif // CONFIG_H