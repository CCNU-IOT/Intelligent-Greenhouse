#ifndef __WS2812B_H__
#define __WS2812B_H__

#include "main.h"


#define LED_COUNT 8 // ����LED�Ƶ�����
#define WS2812B_PWM_PERIOD 60  // ����Ϊ 30 ?s ��������WS2812B ��Э���У�һ�� LED �����ݴ�������Ϊ 30 ?s��
#define TIM_CHANNEL TIM_CHANNEL_1


void WS2812B_Init(void);
void WS2812B_SetColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness, uint8_t ledIndex);
void WS2812B_Update(void);

#endif /* __WS2812B_H__ */
