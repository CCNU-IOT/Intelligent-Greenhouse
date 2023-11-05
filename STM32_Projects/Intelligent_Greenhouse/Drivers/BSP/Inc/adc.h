#ifndef __ADC_H__
#define __ADC_H__
#include "main.h"

void ADC_Init(void);
uint16_t ADC_Read(uint32_t);
uint16_t ADC_getAverage(uint8_t, uint32_t);

#endif /*__ADC_H__*/
