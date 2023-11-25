#ifndef __ADC_H__
#define __ADC_H__
#include "main.h"

#define SENSER_PIN   GPIO_PIN_0
#define SENSER_PORT  GPIOE

void ADC_Init(void);
uint16_t ADC_Read(uint32_t channel);
uint16_t ADC_getAverage(uint8_t, uint32_t);

#endif /*__ADC_H__*/
