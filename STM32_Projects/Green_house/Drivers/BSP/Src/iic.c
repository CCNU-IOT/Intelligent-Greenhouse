#include "iic.h"

I2C_HandleTypeDef hi2c;
GPIO_InitTypeDef GPIO_InitStruct;

void I2C_Init() 
{
    __HAL_RCC_GPIOB_CLK_ENABLE(); // ʹ��GPIOBʱ�ӣ���SCL��SDA����ʱ�ӣ�
    
    GPIO_InitStruct.Pin = GPIO_PIN_6;// SCL����
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // ��©���ģʽ
    GPIO_InitStruct.Pull = GPIO_PULLUP; // ����
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH; // ����
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7;// SDA����
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD; // ��©���ģʽ
    GPIO_InitStruct.Pull = GPIO_PULLUP; // ����
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH; // ����
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    hi2c.Instance = I2C1;
    hi2c.Init.ClockSpeed = 400000; // ���� I2C �����ٶ�Ϊ 400 kHz
    hi2c.Init.DutyCycle = I2C_DUTYCYCLE_2;// ���� I2C ʱ��ռ�ձ�Ϊ 2
    hi2c.Init.OwnAddress1 = 0;// ���� I2C ������ַ
    hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;// ���� I2C ��ַģʽΪ 7 λ��ַģʽ
    hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;// ���� I2C ˫��ַģʽ
    hi2c.Init.OwnAddress2 = 0;// ���� I2C �ڶ�������ַ
    hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;// ���� I2C �㲥��ַģʽ
    hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;// ���� I2C ʱ������
    HAL_I2C_Init(&hi2c);
    HAL_Delay(100); 
}

/**
 * @brief       ����ʱ����SCL�ĵ�ƽ
 * @param       i: 1-�ߵ�ƽ��0-�͵�ƽ
 * @retval      ��
 */
void I2C_set_SCL(uint8_t i) 
{   i?
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET):
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET); 
}

/**
 * @brief       ����������SDA�ĵ�ƽ
 * @param       i: 1-�ߵ�ƽ��0-�͵�ƽ
 * @retval      ��
 */                        
void I2C_set_SDA(uint8_t i) 
{   i?
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET):
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET); 
}

void I2C_Start(void)
{
    I2C_set_SDA(1);
    I2C_set_SCL(1);   
    HAL_Delay(1);
    I2C_set_SDA(0);
    HAL_Delay(1);
    I2C_set_SCL(0);
    HAL_Delay(1);
}

void I2C_Stop(void)
{
    I2C_set_SDA(0);
    HAL_Delay(1);
    I2C_set_SCL(1);
    HAL_Delay(1);
    I2C_set_SDA(1);
    HAL_Delay(1);
}

// HAL_StatusTypeDef I2C_SendData(uint8_t address, uint8_t* data, uint16_t size)
// {
//     return HAL_I2C_Master_Transmit(&hi2c, address, data, size, HAL_MAX_DELAY);
// }

// HAL_StatusTypeDef I2C_ReceiveData(uint8_t address, uint8_t* data, uint16_t size)
// {
//     return HAL_I2C_Master_Receive(&hi2c, address, data, size, HAL_MAX_DELAY);
// }
