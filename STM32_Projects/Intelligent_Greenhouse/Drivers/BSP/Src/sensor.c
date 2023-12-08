#include "sensor.h"

void sensor_init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Pin = GPIO_PIN_0;
    gpio_init_struct.Mode = GPIO_MODE_AF_INPUT;

}

uint8_t sensor_read(void)
{
	if(PA0 == 1) // PA0¿ÚµçÆ½£¨0/1£©
	{ 
		HAL_Delay(10);
		return 1;
	}
	return 0;
}