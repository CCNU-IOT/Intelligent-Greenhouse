#ifndef __RELAY_H__
#define __RELAY_H__
#include "main.h"

//ʹ��PE0���ŽӼ̵������źŴ�����
#define RELAY_PIN   GPIO_PIN_0
#define RELAY_PORT  GPIOE

void Relay_Init(void);
void Relay_On(void);
void Relay_Off(void);

#endif /* __RELAY_H__ */

