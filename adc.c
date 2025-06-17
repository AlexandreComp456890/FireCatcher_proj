/*
 * File:    adc.c
 * Author: adria
 *
 * Created on 16 de Junho de 2025, 15:31
 */

#include <xc.h>
#include "adc.h" // Inclui o header do ADC para as declarações de função
#include "config.h" // Para usar _XTAL_FREQ

void ADC_init(void){
    // Registrador TRISA: RA2 (AN2) e RA3 (AN3) como entradas, o resto como saída digital
    TRISA = 0b00001100;
    
    // ADCON0: Canal padrão (AN0), será alterado em ADC_read; Conversor A/D ativado
    ADCON0bits.CHS = 0b000;
    ADCON0bits.ADON = 1;
    
    // ADCON1: AN3, AN2 como analógicos; o resto digital
    ADCON1 = 0b00001100;
    
    // ADCON2: Justificado à direita (10 bits); Tempo de aquisição 2TAD; Frequência de amostragem FOSC/2
    ADCON2bits.ADFM = 1;
    ADCON2bits.ACQT = 0b001;
    ADCON2bits.ADCS = 0b000;
}

unsigned int ADC_read(unsigned char channel){
    if (channel > 13) return 0;  // Verificação de segurança (PIC18F4520 tem até AN12)

    ADCON0 &= 0xC5;             // Limpa os bits de seleção de canal (CHS<3:0>)
    ADCON0 |= channel << 2;     // Define o canal desejado (CHS<3:0>)
    __delay_us(5);              // Tempo de aquisição

    GO_DONE = 1;                // Inicia a conversão
    while (GO_DONE);            // Espera a conversão terminar

    return (unsigned int)((ADRESH << 8) + ADRESL); // Retorna o resultado de 10 bits
}

float ADC_read_lumi(unsigned int value){
    // Converte o valor bruto do ADC (0-1023) para uma tensão (0-5V)
    return (value * 5.0) / 1023.0;
}

float ADC_read_temp(unsigned int value){
    // Para LM35: 10mV por grau Celsius com Vref de 5V
    return (value * 5.0 * 100.0) / 1023.0;
}