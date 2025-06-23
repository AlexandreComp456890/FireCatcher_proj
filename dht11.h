#ifndef DHT11_H
#define DHT11_H

#include <xc.h>

// Define o pino de dados do DHT11 (ajuste conforme sua conexão)
#define DHT11_DATA_PORT PORTAbits.RA4
#define DHT11_DATA_TRIS TRISAbits.RA4
#define DHT11_DATA_LAT  LATAbits.LATA4

// Protótipo da função de leitura do DHT11
char DHT11_ReadData(void);
void DHT11_Init(void);

#endif // DHT11_H

