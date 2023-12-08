#ifndef __SENSOR_H__
#define __SENSOR_H__
#include "main.h"

#define PA0 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)  //读取GPIOA的P0口电平（0/1）

void sensor_init(void);
uint8_t sensor_read(void);

#endif /*__SENSOR_H__*/
