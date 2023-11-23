#include "ws2812b.h"

// ����TIM��DMA����ز���
TIM_HandleTypeDef htim;
DMA_HandleTypeDef hdma;

// ����LED���ݻ���
uint8_t ledData[LED_COUNT * 24];        // һ����24λCCR����

void Timer1_init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE(); // ʹ��GPIOAʱ��
    __HAL_RCC_TIM2_CLK_ENABLE();  // ʹ��TIM2ʱ��
    __HAL_RCC_DMA1_CLK_ENABLE();  // ʹ��DMA1ʱ�� 

    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP; // PA0����Ϊ����������������PWM��
    gpio_init_struct.Pin = GPIO_PIN_0;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);

    TIM_Base_InitTypeDef tim_base_init_struct;
    tim_base_init_struct.Period = 90 - 1; // ARR
    tim_base_init_struct.Prescaler = 0;   // PSC ����Ƶ
    tim_base_init_struct.ClockDivision = 0;
    tim_base_init_struct.CounterMode = TIM_COUNTERMODE_UP; // ���ϼ���
    HAL_TIM_Base_Init(&tim_base_init_struct);

    TIM_OC_InitTypeDef  tim_oc_init_struct;
    tim_oc_init_struct.OCMode = TIM_OCMODE_PWM1; // PWM1ģʽ��С�ڱȽ�ֵ����ߵ�ƽ�����ڱȽ�ֵ����͵�ƽ
    tim_oc_init_struct.Pulse = 0; // �Ƚ�ֵ��ʼΪ0��һֱ����͵�ƽ
    tim_oc_init_struct.OCPolarity = TIM_OCPOLARITY_HIGH;
    HAL_TIM_OC_Init(&tim_oc_init_struct);

    DMA_InitTypeDef dma_init_struct;
    dma_init_struct.MemDataAlignment = DMA_MDATAALIGN_BYTE;// �����ַ��
}

void WS2812B_Init(void) 
{
    // ��ʼ��HAL��
    HAL_Init();

    // ��ʼ��ϵͳʱ�ӵ�

    // ��ʼ��TIM��DMA
    __HAL_RCC_TIMx_CLK_ENABLE();
    __HAL_RCC_DMAx_CLK_ENABLE();

    // ����TIMΪPWMģʽ
    htim.Instance = TIM1;
    htim.Init.Prescaler = 0;
    htim.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim.Init.Period = WS2812B_PWM_PERIOD - 1;
    htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim.Init.RepetitionCounter = 0;
    HAL_TIM_PWM_Init(&htim);

    // ����DMAΪMemory-to-Peripheralģʽ
    hdma.Instance = DMA1;
    hdma.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma.Init.MemInc = DMA_MINC_ENABLE;
    hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma.Init.Mode = DMA_NORMAL;
    hdma.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma);

    // ��ʼ��LED���ݻ���
    for (int i = 0; i < LED_COUNT * 24; i++) 
        ledData[i] = 0;

    // ����TIM��DMA
    HAL_TIM_PWM_Start_DMA(&htim, TIM_CHANNEL, (uint32_t*)ledData, LED_COUNT * 24);
}

void WS2812B_SetColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness, uint8_t ledIndex) 
{
    // ��������
    red = (uint8_t)((uint16_t)red * brightness / 255);
    green = (uint8_t)((uint16_t)green * brightness / 255);
    blue = (uint8_t)((uint16_t)blue * brightness / 255);

    // ������ledData�е�ƫ��
    int offset = ledIndex * 24;

    // ����LED����
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
    // �ȴ�DMA�������
    HAL_DMA_PollForTransfer(&hdma, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
}