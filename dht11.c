#include "dht11.h"
#include "config.h" // Para usar _XTAL_FREQ

// Função para ler um bit do DHT11
char DHT11_ReadData(void){
  char i,data = 0;  
    for(i=0;i<8;i++)
    {
        while(!(DHT11_DATA_PORT & 1));  /* wait till 0 pulse, this is start of data pulse */
        __delay_us(30);         
        if(DHT11_DATA_PORT & 1)  /* check whether data is 1 or 0 */    
          data = (char)(((data)<<1) | 1); 
        else
          data = (char)((data)<<1);  
        while(DHT11_DATA_PORT & 1);
    }
  return data;
}

void DHT11_CheckResponse(void){
    while(DHT11_DATA_PORT & 1);  /* wait till bus is High */     
    while(!(DHT11_DATA_PORT & 1));  /* wait till bus is Low */
    while(DHT11_DATA_PORT & 1);  /* wait till bus is High */
}

void DHT11_Init(void){    
    DHT11_DATA_TRIS = 0;  /* set as output port */
    DHT11_DATA_LAT = 0;  /* send low pulse of min. 18 ms width */
    __delay_ms(18);
    DHT11_DATA_LAT = 1;  /* pull data bus high */
    __delay_us(20);
    DHT11_DATA_TRIS = 1;  /* set as input port */    
    DHT11_CheckResponse();
}
