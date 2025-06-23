/*
 * File:    pwm.c
 * Author: adria
 *
 * Created on 16 de Junho de 2025, 14:39
 */

#include <xc.h>
#include "pwm.h"
#include "config.h" // Para usar _XTAL_FREQ

#define FREQUENCIA_PWM 500  // Frequência do PWM em Hz
#define TIMER2_PRESCALER_VAL 16 // Corresponde a T2CONbits.T2CKPS = 0b10

void PWM_init(void){
    // Configuração do Período do PWM
    // PWM Period = [(PR2) + 1] * 4 * TOSC * (TMR2 Prescale Value)
    PR2 = (_XTAL_FREQ / (unsigned long)(FREQUENCIA_PWM * 4 * TIMER2_PRESCALER_VAL)) - 1;

    // Configuração da porta de saída PWM (RC2/CCP1)
    CCP1CONbits.P1M = 0b00; // Saída única de PWM (CCP1)
    TRISCbits.RC2 = 0;      // Configura RC2 como saída

    // Configuração do Timer2
    T2CONbits.T2CKPS = 0b10; // Prescaler = 16
    T2CONbits.TMR2ON = 1;    // Ativa o Timer2

    // Ativa o Modo PWM
    CCP1CONbits.CCP1M = 0b1100; // Modo PWM P1A, P1C ativas em alto (Single Output)
}

void PWM_control(int value){
    // O valor de 'value' deve estar entre 0 e 1023 para um PWM de 10 bits.
    if (value >= 0 && value <= 1023) {
        CCPR1L = (unsigned char)(value >> 2); // 8 bits mais significativos
        CCP1CONbits.DC1B = value & 0x03;     // 2 bits menos significativos
    }
}