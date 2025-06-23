// adc.h

#ifndef ADC_H
#define	ADC_H

#include <xc.h>

void ADC_init(void);
unsigned int ADC_read(unsigned char channel);
float ADC_read_lumi(unsigned int value);
float ADC_read_temp(unsigned int value);

#endif	/* ADC_H */