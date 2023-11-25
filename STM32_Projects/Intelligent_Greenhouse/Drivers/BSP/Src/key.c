#include "key.h"
#include "relay.h"

uint8_t key0_status, key1_status, key_up_status;

void key_init_it(void)                                 /*��ʼ�����������жϷ�ʽ��*/
{
    __HAL_RCC_GPIOA_CLK_ENABLE();                   // ʹ��GPIOAʱ��
    __HAL_RCC_GPIOE_CLK_ENABLE();                   // ʹ��GPIOEʱ��

    GPIO_InitTypeDef gpio_init_struct;

    gpio_init_struct.Pin = KEY_UP_PIN;               // ָ��Ҫ���õ�GPIO����ΪKEY_UP������
    gpio_init_struct.Mode = GPIO_MODE_IT_RISING;     // �������Ź���ģʽΪ�������ж�
    gpio_init_struct.Pull = GPIO_PULLDOWN;           // ������������
    gpio_init_struct.Speed = GPIO_SPEED_HIGH;        // ���ù����ٶ�Ϊ����
    HAL_GPIO_Init(KEY_UP_PORT, &gpio_init_struct);   // ��ʼ��KEY_UP_PORT�˿�
    HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);         // �����ⲿ�ж����ȼ�
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);                 // ʹ���ⲿ�ж�

    gpio_init_struct.Pin = KEY0_PIN;                 // ָ��Ҫ���õ�GPIO����ΪKEY0������
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;    // �������Ź���ģʽΪ�½����ж�
    gpio_init_struct.Pull = GPIO_PULLUP;             // ������������
    gpio_init_struct.Speed = GPIO_SPEED_HIGH;        // ���ù����ٶ�Ϊ����
    HAL_GPIO_Init(KEY0_PORT, &gpio_init_struct);     // ��ʼ��KEY0_PORT�˿�
    HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);         // �����ⲿ�ж����ȼ�
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);                 // ʹ���ⲿ�ж�

    gpio_init_struct.Pin = KEY1_PIN;                 // ָ��Ҫ���õ�GPIO����ΪKEY1������
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;    // �������Ź���ģʽΪ�½����ж�
    gpio_init_struct.Pull = GPIO_PULLUP;             // ������������
    gpio_init_struct.Speed = GPIO_SPEED_HIGH;        // ���ù����ٶ�Ϊ����
    HAL_GPIO_Init(KEY1_PORT, &gpio_init_struct);     // ��ʼ��KEY1_PORT�˿�
    HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);         // �����ⲿ�ж����ȼ�
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);                 // ʹ���ⲿ�ж�
}

void EXTI0_IRQHandler(void) //KEY_UP�ⲿ�жϷ������
{
  HAL_GPIO_EXTI_IRQHandler(KEY_UP_PIN);
  __HAL_GPIO_EXTI_CLEAR_IT(KEY_UP_PIN); // ��һ���жϣ����ⰴ�������󴥷�
}

void EXTI4_IRQHandler(void) //KEY0�ⲿ�жϷ������
{
  HAL_GPIO_EXTI_IRQHandler(KEY0_PIN);
  __HAL_GPIO_EXTI_CLEAR_IT(KEY0_PIN); // ��һ���жϣ����ⰴ�������󴥷�
}

void EXTI3_IRQHandler(void) //KEY1�ⲿ�жϷ������
{
  HAL_GPIO_EXTI_IRQHandler(KEY1_PIN);
  __HAL_GPIO_EXTI_CLEAR_IT(KEY1_PIN); // ��һ���жϣ����ⰴ�������󴥷�
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    //HAL_Delay(20); // ��������
    switch (GPIO_Pin)
    {   
    case KEY_UP_PIN:
        if(KEY_UP == 1) // KEY_UP����ʱ����״̬Ϊ�ߵ�ƽ
        {
            // for(i=0; i<0x7fff; i++)
            //     if(KEY_UP == 0)
            //         return; // ���Ż�δ���㳤��ʱ��Ҫ��
            Relay_On(); // �̰�KEY_UPҪִ�еĲ�������ˮ��
        }
        break;

    case KEY0_PIN:
        if(KEY0 == 0) // KEY0����ʱ����״̬Ϊ�͵�ƽ
        {
            // for(i=0; i<0x7fff; i++)
            //     if(KEY0 == 1)
            //         return; // ���Ż�δ���㳤��ʱ��Ҫ��
            Relay_Off(); // �̰�KEY0Ҫִ�еĲ������ر�ˮ��
        }
        break;

    case KEY1_PIN:
        if(KEY1 == 0) // KEY1����ʱ����״̬Ϊ�͵�ƽ
        {
            // for(i=0; i<0x7fff; i++)
            //     if(KEY1 == 1)
            //         return; // ���Ż�δ���㳤��ʱ��Ҫ��
            Relay_Off(); // �̰�KEY1Ҫִ�еĲ������ر�ˮ��
        }
        break;
    }
}
