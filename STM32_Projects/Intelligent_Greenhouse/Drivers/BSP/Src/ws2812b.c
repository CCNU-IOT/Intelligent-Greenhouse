#include "ws2812b.h"

// 定义TIM、DMA等相关参数
TIM_HandleTypeDef htim;
DMA_HandleTypeDef hdma;

// 定义LED数据缓存
uint8_t ledData[LED_COUNT * 24];        // 一个灯24位CCR数据

void Timer1_init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE(); // 使能GPIOA时钟
    __HAL_RCC_TIM2_CLK_ENABLE();  // 使能TIM2时钟
    __HAL_RCC_DMA1_CLK_ENABLE();  // 使能DMA1时钟 

    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP; // PA0设置为复用推挽输出来输出PWM波
    gpio_init_struct.Pin = GPIO_PIN_0;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);

    TIM_Base_InitTypeDef tim_base_init_struct;
    tim_base_init_struct.Period = 90 - 1; // ARR
    tim_base_init_struct.Prescaler = 0;   // PSC 不分频
    tim_base_init_struct.ClockDivision = 0;
    tim_base_init_struct.CounterMode = TIM_COUNTERMODE_UP; // 向上计数
    HAL_TIM_Base_Init(&tim_base_init_struct);

    TIM_OC_InitTypeDef  tim_oc_init_struct;
    tim_oc_init_struct.OCMode = TIM_OCMODE_PWM1; // PWM1模式，小于比较值输出高电平，大于比较值输出低电平
    tim_oc_init_struct.Pulse = 0; // 比较值初始为0，一直输出低电平
    tim_oc_init_struct.OCPolarity = TIM_OCPOLARITY_HIGH;
    HAL_TIM_OC_Init(&tim_oc_init_struct);

    DMA_InitTypeDef dma_init_struct;
    dma_init_struct.MemDataAlignment = DMA_MDATAALIGN_BYTE;// 外设地址？
}

void WS2812B_Init(void) 
{
    // 初始化HAL库
    HAL_Init();

    // 初始化系统时钟等

    // 初始化TIM和DMA
    __HAL_RCC_TIMx_CLK_ENABLE();
    __HAL_RCC_DMAx_CLK_ENABLE();

    // 配置TIM为PWM模式
    htim.Instance = TIM1;
    htim.Init.Prescaler = 0;
    htim.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim.Init.Period = WS2812B_PWM_PERIOD - 1;
    htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim.Init.RepetitionCounter = 0;
    HAL_TIM_PWM_Init(&htim);

    // 配置DMA为Memory-to-Peripheral模式
    hdma.Instance = DMA1;
    hdma.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma.Init.MemInc = DMA_MINC_ENABLE;
    hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma.Init.Mode = DMA_NORMAL;
    hdma.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma);

    // 初始化LED数据缓存
    for (int i = 0; i < LED_COUNT * 24; i++) 
        ledData[i] = 0;

    // 启动TIM和DMA
    HAL_TIM_PWM_Start_DMA(&htim, TIM_CHANNEL, (uint32_t*)ledData, LED_COUNT * 24);
}

void WS2812B_SetColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness, uint8_t ledIndex) 
{
    // 计算亮度
    red = (uint8_t)((uint16_t)red * brightness / 255);
    green = (uint8_t)((uint16_t)green * brightness / 255);
    blue = (uint8_t)((uint16_t)blue * brightness / 255);

    // 计算在ledData中的偏移
    int offset = ledIndex * 24;

    // 设置LED数据
    for (int i = 0; i < 8; i++) 
    {
        if (green & (1 << (7 - i))) 
            ledData[offset + i] = WS2812B_PWM_PERIOD;
        else 
            ledData[offset + i] = WS2812B_PWM_PERIOD / 3;
    }

    for (int i = 0; i < 8; i++) 
    {
        if (red & (1 << (7 - i))) 
            ledData[offset + 8 + i] = WS2812B_PWM_PERIOD;
        else
            ledData[offset + 8 + i] = WS2812B_PWM_PERIOD / 3;
    }

    for (int i = 0; i < 8; i++) 
    {
        if (blue & (1 << (7 - i))) 
            ledData[offset + 16 + i] = WS2812B_PWM_PERIOD;
        else
            ledData[offset + 16 + i] = WS2812B_PWM_PERIOD / 3;
    }
}

void WS2812B_Update(void) 
{
    // 等待DMA传输完成
    HAL_DMA_PollForTransfer(&hdma, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
}