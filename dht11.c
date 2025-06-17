#include "dht11.h"
#include "config.h" // Para usar _XTAL_FREQ

#define _XTAL_FREQ 8000000 // Garantir que a frequência seja definida para __delay_ms

// Função para ler um bit do DHT11
unsigned char DHT11_read_bit(void) {
    unsigned char timeout = 0;
    // Espera pelo pino ir para LOW (resposta do DHT11)
    while (DHT11_DATA_PORT == 0 && timeout < 100) {
        __delay_us(1);
        timeout++;
    }
    timeout = 0;
    // Espera pelo pino ir para HIGH (início do bit)
    while (DHT11_DATA_PORT == 1 && timeout < 100) {
        __delay_us(1);
        timeout++;
    }
    // Atraso de 40us: se o pino ainda estiver HIGH, é um '1', senão é um '0'
    __delay_us(40);
    if (DHT11_DATA_PORT == 1) {
        // Espera pelo pino ir para LOW novamente para garantir que o bit foi lido
        timeout = 0;
        while (DHT11_DATA_PORT == 1 && timeout < 100) {
            __delay_us(1);
            timeout++;
        }
        return 1;
    } else {
        return 0;
    }
}

// Função para ler um byte (8 bits) do DHT11
unsigned char DHT11_read_byte(void) {
    unsigned char i, byte = 0;
    for (i = 0; i < 8; i++) {
        if (DHT11_read_bit()) {
            byte |= (1 << (7 - i));
        }
    }
    return byte;
}

// Função para ler os dados completos do DHT11
unsigned char DHT11_read(DHT11_Data *data) {
    unsigned char timeout = 0;
    unsigned char buffer[5];
    unsigned char checksum;

    // 1. MCU envia sinal de start (pino LOW por 18ms, depois HIGH por 20-40us)
    DHT11_DATA_TRIS = 0; // Configura pino como saída
    DHT11_DATA_PORT = 0; // Pino LOW
    __delay_ms(18);      // Espera 18ms
    DHT11_DATA_PORT = 1; // Pino HIGH
    __delay_us(30);      // Espera 20-40us
    DHT11_DATA_TRIS = 1; // Configura pino como entrada (para ler a resposta do DHT11)

    // 2. DHT11 responde (LOW por 80us, depois HIGH por 80us)
    // Espera o pino ir para LOW (resposta do DHT11)
    timeout = 0;
    while (DHT11_DATA_PORT == 1 && timeout < 100) {
        __delay_us(1);
        timeout++;
    }
    if (timeout >= 100) return 0; // Timeout, sem resposta

    // Espera o pino ir para HIGH (início do pulso de resposta)
    timeout = 0;
    while (DHT11_DATA_PORT == 0 && timeout < 100) {
        __delay_us(1);
        timeout++;
    }
    if (timeout >= 100) return 0; // Timeout, sem resposta

    // Espera o pino ir para LOW novamente (fim do pulso de resposta)
    timeout = 0;
    while (DHT11_DATA_PORT == 1 && timeout < 100) {
        __delay_us(1);
        timeout++;
    }
    if (timeout >= 100) return 0; // Timeout, sem resposta

    // 3. Lê 5 bytes de dados
    for (unsigned char i = 0; i < 5; i++) {
        buffer[i] = DHT11_read_byte();
    }

    // 4. Calcula o checksum e verifica
    checksum = buffer[0] + buffer[1] + buffer[2] + buffer[3];
    if (checksum == buffer[4]) {
        data->humidity_int = buffer[0];
        data->humidity_dec = buffer[1];
        data->temperature_int = buffer[2];
        data->temperature_dec = buffer[3];
        data->checksum = buffer[4];
        return 1; // Sucesso na leitura
    } else {
        return 0; // Erro de checksum
    }
}

