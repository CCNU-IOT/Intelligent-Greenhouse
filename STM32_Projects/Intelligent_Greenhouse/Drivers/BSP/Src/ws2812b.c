#include "ws2812b.h"

// 定义TIM、DMA等相关参数
TIM_HandleTypeDef htim;
DMA_HandleTypeDef hdma;

// 定义LED数据缓存
uint8_t ledData[LED_COUNT * 24];

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