#ifndef __ADC_H__
#define __ADC_H__
#include "main.h"

#define SENSER_PIN   GPIO_PIN_0
#define SENSER_PORT  GPIOA

void ADC_Init(void);
void adc_channel_set(uint32_t, uint32_t, uint32_t);
uint16_t ADC_getValue(void);
uint16_t ADC_getAverage(uint8_t);

#endif /*__ADC_H__*/
