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
    // Registrador TRISA: todos como entrada
    TRISA = 0b11111111;
    
    // ADCON0: Canal padrão (AN3), será alterado em ADC_read; Conversor A/D ativado
    ADCON0 = 0b00001101;
    
    // ADCON1: todas analogicas
    ADCON1 = 0b00000000;
    
    ADCON2 = 0b10111010; 
}

unsigned int ADC_read(unsigned char channel) {
    if (channel > 12) return 0; // AN0 to AN12 are valid

    // Clear CHS bits (bits 5:2) without touching ADON or GO_DONE
    ADCON0 &= 0b00000011;         // Clear CHS3:CHS0 (bits 5?2)
    ADCON0 |= (channel << 2);     // Set desired channel bits

    ADCON0bits.GODONE = 1;       // Start conversion
    while (ADCON0bits.GODONE);   // Wait until complete
    
    return (unsigned int)((ADRESH << 8) | ADRESL);
}

float ADC_read_lumi(unsigned int value){
    // Converte o valor bruto do ADC (0-1023) para uma tensão (0-5V)
    return (value * 5.0 / 1023.0);
}

float ADC_read_temp(unsigned int value){
    // Para LM35: 10mV por grau Celsius com Vref de 5V
    return (value * 5.0 * 100.0) / 1023.0;
}