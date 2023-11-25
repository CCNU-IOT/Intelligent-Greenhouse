#include "tim.h"

TIM_HandleTypeDef WS2812B_pwm_tim_handle = {0}; // 用于存储定时器相关配置
/**
 * @brief 初始化WS2812B
 * @param duty:PWM的占空比，用于控制亮度(范围0 ~ 99)
*/
void WS2812B_pwm_tim_init(uint32_t duty)   
{
    WS2812B_pwm_tim_handle.Instance = WS2812B_PWM_TIM;           // 选择定时器
    WS2812B_pwm_tim_handle.Init.Prescaler = WS2812B_PWM_TIM_PSC; // 预分频值
    WS2812B_pwm_tim_handle.Init.Period = WS2812B_PWM_TIM_ARR;    // 自动重装载值
    WS2812B_pwm_tim_handle.Init.CounterMode = TIM_COUNTERMODE_UP;// 递增计数模式
    WS2812B_pwm_tim_handle.Init.AutoReloadPreload = TIM_AUTOMATICOUTPUT_ENABLE; // 启用自动重装载寄存器预装载
    HAL_TIM_PWM_Init(&WS2812B_pwm_tim_handle); // 初始化定时器为PWM输出模式

    TIM_OC_InitTypeDef tim_oc_config_struct = {0}; // 用于存储定时器通道配置
    tim_oc_config_struct.OCMode = TIM_OCMODE_PWM1; // 模式选择PWM1（TIMx_CNT<TIMx_CCR1时通道1为有效电平，否则为无效电平）
    tim_oc_config_struct.Pulse = duty; // 比较值（占空比）
    tim_oc_config_struct.OCNPolarity = TIM_OCNPOLARITY_HIGH; // 设置通道极性为高电平有效
    HAL_TIM_PWM_ConfigChannel(&WS2812B_pwm_tim_handle, &tim_oc_config_struct, TIM_CHANNEL_1); // 配置定时器通道1为PWM模式
    // HAL_TIM_PWM_ConfigChannel(&WS2812B_pwm_tim_handle, &tim_oc_config_struct, TIM_CHANNEL_2); // 配置定时器通道2为PWM模式
    // HAL_TIM_PWM_ConfigChannel(&WS2812B_pwm_tim_handle, &tim_oc_config_struct, TIM_CHANNEL_3); // 配置定时器通道3为PWM模式
    // HAL_TIM_PWM_ConfigChannel(&WS2812B_pwm_tim_handle, &tim_oc_config_struct, TIM_CHANNEL_4); // 配置定时器通道4为PWM模式
    HAL_TIM_PWM_Start(&WS2812B_pwm_tim_handle, TIM_CHANNEL_1); // 使能定时器通道1
    // HAL_TIM_PWM_Start(&WS2812B_pwm_tim_handle, TIM_CHANNEL_2); // 使能定时器通道2
    // HAL_TIM_PWM_Start(&WS2812B_pwm_tim_handle, TIM_CHANNEL_3); // 使能定时器通道3
    // HAL_TIM_PWM_Start(&WS2812B_pwm_tim_handle, TIM_CHANNEL_4); // 使能定时器通道4
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim) // 此函数会被HAL_TIM_PWM_Init()调用
{
    if (htim->Instance == WS2812B_PWM_TIM)
    {
        __HAL_RCC_GPIOE_CLK_ENABLE(); // 为使用定时器1通道1（重映射至PE9），使能GPIOE时钟
        __HAL_RCC_TIM1_CLK_ENABLE(); // 使能TIM1时钟
        __HAL_RCC_AFIO_CLK_ENABLE(); // 使能AFIO时钟

        GPIO_InitTypeDef gpio_init_struct;
        gpio_init_struct.Pin = WS2812B_PWM_TIM_CH2_PIN; // 设置GPIO引脚为WS2812B_PWM_TIM_CH2_PIN（GPIO_PIN_9）
        gpio_init_struct.Mode = GPIO_MODE_AF_PP; // 复用推挽输出,作为TIM1的CH2输出
        gpio_init_struct.Pull = GPIO_NOPULL;           
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;     
        HAL_GPIO_Init(WS2812B_PWM_TIM_CH2_PORT, &gpio_init_struct);

        __HAL_AFIO_REMAP_TIM1_ENABLE(); // 启用TIM1的重映射
    }
}
