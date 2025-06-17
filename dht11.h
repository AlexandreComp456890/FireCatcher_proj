#ifndef DHT11_H
#define DHT11_H

#include <xc.h>

// Define o pino de dados do DHT11 (ajuste conforme sua conexão)
#define DHT11_DATA_PORT PORTCbits.RC0
#define DHT11_DATA_TRIS TRISCbits.TRISC0

// Estrutura para armazenar os dados do DHT11
typedef struct {
    unsigned char humidity_int;
    unsigned char humidity_dec;
    unsigned char temperature_int;
    unsigned char temperature_dec;
    unsigned char checksum;
} DHT11_Data;

// Protótipo da função de leitura do DHT11
unsigned char DHT11_read(DHT11_Data *data);

#endif // DHT11_H

