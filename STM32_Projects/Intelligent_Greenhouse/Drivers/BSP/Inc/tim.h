#ifndef __TIM_H__
#define __TIM_H__
#include "main.h"

#define WS2812B_PWM_TIM                           TIM2
#define WS2812B_PWM_TIM_PSC                       (72U - 1U)              /*72MHz -> 1MHz <---> 1us*/
#define WS2812B_PWM_TIM_ARR                       (100U - 1U)             /*PWM 1M / 100 = 10KHz*/
#define WS2812B_PWM_TIM_CH2_PORT                  GPIOB
#define WS2812B_PWM_TIM_CH2_PIN                   GPIO_PIN_3

void WS2812B_pwm_init(uint32_t duty);

#endif /*__TIM_H__*/
