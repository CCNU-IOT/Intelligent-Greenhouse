#include "key.h"
#include "btim.h"
#include "stm32f1xx_hal_gpio.h"

void key_init(void)                                 /*��ʼ������*/
{
    GPIO_InitTypeDef gpio_init_struct;

    __HAL_RCC_GPIOA_CLK_ENABLE();                   // ʹ��GPIOAʱ��
    __HAL_RCC_GPIOE_CLK_ENABLE();                   // ʹ��GPIOEʱ��

    gpio_init_struct.Pin = KEY_UP_PIN;               // ָ��Ҫ���õ�GPIO����ΪKEY_UP������
    gpio_init_struct.Mode = GPIO_MODE_INPUT;         // �������Ź���ģʽΪ����ģʽ
    gpio_init_struct.Pull = GPIO_PULLDOWN;           // ������������
    gpio_init_struct.Speed = GPIO_SPEED_HIGH;        // ���ù����ٶ�Ϊ����
    HAL_GPIO_Init(KEY_UP_PORT, &gpio_init_struct);   // ��ʼ��KEY_UP_PORT�˿�

    gpio_init_struct.Pin = KEY0_PIN;                 // ָ��Ҫ���õ�GPIO����ΪKEY0������
    gpio_init_struct.Mode = GPIO_MODE_INPUT;         // �������Ź���ģʽΪ����ģʽ
    gpio_init_struct.Pull = GPIO_PULLUP;             // ������������
    gpio_init_struct.Speed = GPIO_SPEED_HIGH;        // ���ù����ٶ�Ϊ����
    HAL_GPIO_Init(KEY0_PORT, &gpio_init_struct);     // ��ʼ��KEY0_PORT�˿�

    gpio_init_struct.Pin = KEY1_PIN;                 // ָ��Ҫ���õ�GPIO����ΪKEY1������
    gpio_init_struct.Mode = GPIO_MODE_INPUT;         // �������Ź���ģʽΪ����ģʽ
    gpio_init_struct.Pull = GPIO_PULLUP;             // ������������
    gpio_init_struct.Speed = GPIO_SPEED_HIGH;        // ���ù����ٶ�Ϊ����
    HAL_GPIO_Init(KEY1_PORT, &gpio_init_struct);     // ��ʼ��KEY1_PORT�˿�
}

uint8_t key_get_status()
{
    uint8_t keyCurrentState;
        if(KEY_UP == 1) keyCurrentState = KEY_UP_PRESSED_SHORT;
        if(KEY0 == 0) keyCurrentState = KEY0_PRESSED_SHORT;
        if(KEY1 == 0) keyCurrentState = KEY1_PRESSED_SHORT;
    return keyCurrentState;
}

uint8_t key_scan(void)
{
    static uint8_t previousState = NO_KEY_PRESSED;  // ��һ������״̬����ʼΪδ����
    static uint32_t debounceTimer = 0;              // ����������ʱ��
    static uint32_t longPressTimer = 0;             // ������ʱ��

    uint32_t currentTimer = 0;                      // ���õ�ǰʱ��Ϊ0
    uint8_t keyCurrentState = NO_KEY_PRESSED;       // ��ǰ����״̬��Ĭ��δ����

    currentTimer = getCurrentTime();                // ��ȡ��ǰʱ��(����millis();)

    if ((currentTimer - debounceTimer) >= DEBOUNCE_DELAY_MS)        // ������10MS���ٶ�ȡ����״̬
    {
        
        keyCurrentState = key_get_status();                         // ����Ƿ��а���������
        
        if (keyCurrentState != previousState)                       // ��ⰴ��״̬�仯
        {
            debounceTimer = currentTimer;                           // ������״̬���䣬���°���������ʱ��
        }
        else
        {                                                           // ������״̬�ı䣬��ⰴ������һ��ʱ����״̬
            if ((keyCurrentState != NO_KEY_PRESSED) && ((currentTimer - debounceTimer) >= LONG_PRESS_DELAY_MS))
            {
                longPressTimer = currentTimer;                      // ��⵽�а���������Ϊ���������³�����ʱ��
                keyCurrentState += 3;                               // ������״̬תΪ����
            }
        }
        previousState = keyCurrentState;                            // ������һ������״̬
    }

/*    if (keyCurrentState != NO_KEY_PRESSED && (currentTimer - debounceTimer) < LONG_PRESS_DELAY_MS)
            {}                                                      // ���̰�(�ƺ�û��Ҫ)
*/
    return keyCurrentState;
}
