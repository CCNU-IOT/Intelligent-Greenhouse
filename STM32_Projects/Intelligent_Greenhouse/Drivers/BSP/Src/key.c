#include "key.h"
#include "relay.h"

uint8_t key0_status, key1_status, key_up_status;

void key_init_it(void)                                 /*初始化按键（以中断方式）*/
{
    __HAL_RCC_GPIOA_CLK_ENABLE();                   // 使能GPIOA时钟
    __HAL_RCC_GPIOE_CLK_ENABLE();                   // 使能GPIOE时钟

    GPIO_InitTypeDef gpio_init_struct;

    gpio_init_struct.Pin = KEY_UP_PIN;               // 指定要配置的GPIO引脚为KEY_UP的引脚
    gpio_init_struct.Mode = GPIO_MODE_IT_RISING;     // 设置引脚工作模式为上升沿中断
    gpio_init_struct.Pull = GPIO_PULLDOWN;           // 设置下拉电阻
    gpio_init_struct.Speed = GPIO_SPEED_HIGH;        // 设置工作速度为高速
    HAL_GPIO_Init(KEY_UP_PORT, &gpio_init_struct);   // 初始化KEY_UP_PORT端口
    HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);         // 设置外部中断优先级
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);                 // 使能外部中断

    gpio_init_struct.Pin = KEY0_PIN;                 // 指定要配置的GPIO引脚为KEY0的引脚
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;    // 设置引脚工作模式为下降沿中断
    gpio_init_struct.Pull = GPIO_PULLUP;             // 设置上拉电阻
    gpio_init_struct.Speed = GPIO_SPEED_HIGH;        // 设置工作速度为高速
    HAL_GPIO_Init(KEY0_PORT, &gpio_init_struct);     // 初始化KEY0_PORT端口
    HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);         // 设置外部中断优先级
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);                 // 使能外部中断

    gpio_init_struct.Pin = KEY1_PIN;                 // 指定要配置的GPIO引脚为KEY1的引脚
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;    // 设置引脚工作模式为下降沿中断
    gpio_init_struct.Pull = GPIO_PULLUP;             // 设置上拉电阻
    gpio_init_struct.Speed = GPIO_SPEED_HIGH;        // 设置工作速度为高速
    HAL_GPIO_Init(KEY1_PORT, &gpio_init_struct);     // 初始化KEY1_PORT端口
    HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);         // 设置外部中断优先级
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);                 // 使能外部中断
}

void EXTI0_IRQHandler(void) //KEY_UP外部中断服务程序
{
  HAL_GPIO_EXTI_IRQHandler(KEY_UP_PIN);
  __HAL_GPIO_EXTI_CLEAR_IT(KEY_UP_PIN); // 清一次中断，避免按键抖动误触发
}

void EXTI4_IRQHandler(void) //KEY0外部中断服务程序
{
  HAL_GPIO_EXTI_IRQHandler(KEY0_PIN);
  __HAL_GPIO_EXTI_CLEAR_IT(KEY0_PIN); // 清一次中断，避免按键抖动误触发
}

void EXTI3_IRQHandler(void) //KEY1外部中断服务程序
{
  HAL_GPIO_EXTI_IRQHandler(KEY1_PIN);
  __HAL_GPIO_EXTI_CLEAR_IT(KEY1_PIN); // 清一次中断，避免按键抖动误触发
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    //HAL_Delay(20); // 按键消抖
    switch (GPIO_Pin)
    {   
    case KEY_UP_PIN:
        if(KEY_UP == 1) // KEY_UP按下时引脚状态为高电平
        {
            // for(i=0; i<0x7fff; i++)
            //     if(KEY_UP == 0)
            //         return; // 干扰或未满足长按时间要求
            Relay_On(); // 短按KEY_UP要执行的操作：打开水泵
        }
        break;

    case KEY0_PIN:
        if(KEY0 == 0) // KEY0按下时引脚状态为低电平
        {
            // for(i=0; i<0x7fff; i++)
            //     if(KEY0 == 1)
            //         return; // 干扰或未满足长按时间要求
            Relay_Off(); // 短按KEY0要执行的操作：关闭水泵
        }
        break;

    case KEY1_PIN:
        if(KEY1 == 0) // KEY1按下时引脚状态为低电平
        {
            // for(i=0; i<0x7fff; i++)
            //     if(KEY1 == 1)
            //         return; // 干扰或未满足长按时间要求
            Relay_Off(); // 短按KEY1要执行的操作：关闭水泵
        }
        break;
    }
}
