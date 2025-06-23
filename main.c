/*
 * File: Big'ol Fuck
 * Author: Pingus
 *
 * Created on 25 de Maio de 2025, 19:33
 */

#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "adc.h"
#include "lcd.h"
#include "pwm.h"
#include "dht11.h"

// Define _XTAL_FREQ se não estiver definido em config.h
#include "config.h"

//Variaveis de controle
unsigned int flag_analog_value = 0;
unsigned int flow = 0;
unsigned int timer;
//LM35 e LDR
unsigned int ldr_raw_value, lm35_raw_value;
float ldr_value, lm35_value;

void main() {
    //PWM
    int pwm_duty_cycle;
    //DHT11
    char RH_Decimal,RH_Integral,T_Decimal,T_Integral;
    char Checksum;
    
    
    char buffer[16];
    
    OSCCON = 0b01110111; // Configura o oscilador interno para 8MHz
    INIT_INTERRUPTS();
    
    TMR0H = 0xF0; // High byte of (65536 - 3906)
    TMR0L = 0xBE; // Low byte of (65536 - 3906)
    T0CON = 0b10000111;; // Timer0 config
    
    ADC_init(); // Inicializa o ADC
    PWM_init(); // Inicializa o PWM
    lcd_init(); // Inicializa o LCD

    lcd_set_cursor(0, 2);
    lcd_string("Iniciando");
    for (unsigned char i = 11; i < 14; i++){
        lcd_set_cursor(0, i);
        lcd_string(".");
        __delay_ms(500);
    }

    while(1){
        lcd_clear();

        if (flow == 0){
            // === Abertura ===
            lcd_set_cursor(0,2);
            lcd_string("LM35 and LDR");
            __delay_ms(1000);
            lcd_clear();

            // === Leitura do LM35 (AN2) ===
            if (flag_analog_value == 0){
                lm35_raw_value = ADC_read(2); // RA2
                lm35_value = ADC_read_temp(lm35_raw_value); // em °C
                // === Exibe no LCD LM35 ===
                lcd_set_cursor(0, 0);
                sprintf(buffer, "LM35: %.2f\xB0"" C ", lm35_value);
                lcd_string(buffer);
            }

            // === Leitura do LDR (AN3) ===
            if (flag_analog_value == 1){
                ldr_raw_value = ADC_read(4); // RA3, mudar para RA4 na simulação (simulador defeituoso) 
                ldr_value = ADC_read_lumi(ldr_raw_value); // em Volts (ou proporção) 
                // === Exibe no LCD LDR ===
                lcd_set_cursor(1, 0);
                if (ldr_value <= 1.5) lcd_string("-------Ok-------");
                else if(ldr_value > 1.5 && ldr_value < 3.0) lcd_string("----Atencao!----");
                else lcd_string("****!Perigo!****");
            }
            
        }else{
            // === Iniciando e pegando as informações do DHT11 ===
            DHT11_Init();

            RH_Integral = DHT11_ReadData();  // Valor inteiro da umidade
            RH_Decimal = DHT11_ReadData();   // Valor decimal da umidade
            T_Integral = DHT11_ReadData();   // Valor inteiro da temperatura
            T_Decimal = DHT11_ReadData();    // Valor decimal da temperatura
            Checksum = DHT11_ReadData();     // Soma dos valores digitais 

            lcd_set_cursor(0,2);
            lcd_string("DHT11 Values");
            __delay_ms(1000);
            lcd_clear();

            // === Informações da umidade para o LCD ===
            lcd_set_cursor(0,0);
            sprintf(buffer,"DHT11 Hum: %d.%d",RH_Integral, RH_Decimal);
            lcd_string(buffer);

            // === Informações da temperatura para o LCD ===
            lcd_set_cursor(1,0);
            sprintf(buffer,"DHT11 Tem: %d.%d",T_Integral, T_Decimal);
            lcd_string(buffer);
        }
        
        // === Sinal de emergencia ===
        if ((lm35_value >= 85.0 && ldr_value > 3.0)) {
            int up = 1;
            pwm_duty_cycle = 0;

            while (1) {
                // === Update sensor values ===
                lm35_raw_value = ADC_read(2);
                lm35_value = ADC_read_temp(lm35_raw_value);

                ldr_raw_value = ADC_read(4);
                ldr_value = ADC_read_lumi(ldr_raw_value);
                
                // === Saída do loop ===
                if (lm35_value < 60.0 || ldr_value <= 2.5) break;
                
                // === Sirene com buzzer ===
                PWM_control(pwm_duty_cycle);
                
                if (up) {
                    pwm_duty_cycle++;
                    if (pwm_duty_cycle >= 1023)
                        up = 0;
                } else {
                    pwm_duty_cycle--;
                    if (pwm_duty_cycle <= 0)
                        up = 1;
                }

                __delay_ms(5);
            }
        }else PWM_control(0);

        __delay_ms(2000);
    }
    return;
}

void __interrupt() isr(void){
    //Se flag ADIF==1 conversão A/D foi finalizada
    if(PIR1bits.ADIF){
        PIR1bits.ADIF = 0;
        flag_analog_value = !flag_analog_value;
    }
    
    if (INTCONbits.TMR0IF){
        INTCONbits.TMR0IF = 0;
        TMR0H = 0xF0; // High byte of (65536 - 3906)
        TMR0L = 0xBE; // Low byte of (65536 - 3906)
        timer++;
        if (timer >= 6){
            timer = 0;
            flow = !flow;
        }
    }
}