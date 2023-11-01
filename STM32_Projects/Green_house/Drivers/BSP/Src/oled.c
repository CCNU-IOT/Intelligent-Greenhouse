#include "oled.h"

#define OLED_I2C_ADDRESS 0x78 // ʵ��7λI2C��ַ0x3c ����һλ�õ�0x78
#define OLED_CMD 0x00 // 0000 0000 ��������
#define OLED_DATA 0x80 // 1100 0000 ��������

I2C_HandleTypeDef hi2c;

static void OLED_WriteByte(uint8_t cmd , uint8_t data) { // ��OLEDд��һ���ֽ�
    uint8_t buffer[2]; // ���Ҫ�������ݵĻ�����
    buffer[0] = cmd;
    buffer[1] = data;
    HAL_I2C_Master_Transmit(&hi2c, OLED_I2C_ADDRESS, buffer, 2, HAL_MAX_DELAY);
}

void OLED_Init() 
{
    I2C_Init();

    OLED_WriteByte(OLED_CMD, 0xAE);  // �ر���ʾ
    OLED_WriteByte(OLED_CMD, 0xD5);  // ����ʱ�ӷ�Ƶ����
    OLED_WriteByte(OLED_CMD, 0x80);  // ��Ƶ����
    OLED_WriteByte(OLED_CMD, 0xA8);  // ���ö༫��ѹ����
    OLED_WriteByte(OLED_CMD, 0x3F);  // 1/64 ƫѹ
    OLED_WriteByte(OLED_CMD, 0xD3);  // ������ʾƫ��
    OLED_WriteByte(OLED_CMD, 0x00);
    OLED_WriteByte(OLED_CMD, 0x40);  // ������ʼ��
    OLED_WriteByte(OLED_CMD, 0x8D);  // ���õ�ɱ�
    OLED_WriteByte(OLED_CMD, 0x14);
    OLED_WriteByte(OLED_CMD, 0x20);  // �����ڴ��ַģʽ
    OLED_WriteByte(OLED_CMD, 0x00);  // ˮƽ��ַģʽ
    OLED_WriteByte(OLED_CMD, 0xA1);  // ���ض�λ
    OLED_WriteByte(OLED_CMD, 0xC8);  // ��ֱ�����ض�λ
    OLED_WriteByte(OLED_CMD, 0xDA);  // COMӲ������
    OLED_WriteByte(OLED_CMD, 0x12);
    OLED_WriteByte(OLED_CMD, 0x81);  // �Աȶȿ���
    OLED_WriteByte(OLED_CMD, 0xCF);  // ���öԱȶ�
    OLED_WriteByte(OLED_CMD, 0xD9);  // Ԥ�������
    OLED_WriteByte(OLED_CMD, 0xF1);
    OLED_WriteByte(OLED_CMD, 0xDB);  // ���� VCOMH ��ѹ����
    OLED_WriteByte(OLED_CMD, 0x40);
    OLED_WriteByte(OLED_CMD, 0xA4);  // ȫ����ʾ����
    OLED_WriteByte(OLED_CMD, 0xA6);  // ������ʾ��ʽ��������ʾ
    OLED_WriteByte(OLED_CMD, 0xAF);  // ����ʾ
    OLED_WriteByte(OLED_CMD, 0xAE);  // �ر���ʾ
}

void OLED_DisplayOff(void) // ����
{
    OLED_WriteByte(OLED_CMD, 0xAE);
}

void OLED_DisplayPixel(uint8_t x, uint8_t y) // ��������(x, y)������
{
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT) //�������곬����ʾ��Χ
        return; 

    uint16_t offset = x + (y / 8) * OLED_WIDTH; // �����ڻ������е�ƫ��

    // ��������
    OLED_WriteByte(OLED_CMD, 0x00); // ���д������
    OLED_WriteByte(OLED_CMD, offset % OLED_WIDTH); // �е�ַ
    OLED_WriteByte(OLED_CMD, 0xB0 + (y / 8)); // ҳ��ַ
    OLED_WriteByte(OLED_CMD, (1 << (y % 8))); // λ��ַ
    OLED_WriteByte(OLED_CMD, 0xAF); // ������ǰ����
}

void OLED_DisplayString(uint8_t x, uint8_t y, char *str) // ������(x, y)����ʾ�ַ���
{
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT) //�������곬����ʾ��Χ
        return; 

    uint16_t offset = x + (y / 8) * OLED_WIDTH; // �����ڻ������е�ƫ��

    // ��������
    OLED_WriteByte(OLED_CMD, 0x00); // ���д������
    OLED_WriteByte(OLED_CMD, offset % OLED_WIDTH); // �е�ַ
    OLED_WriteByte(OLED_CMD, 0xB0 + (y / 8)); // ҳ��ַ
    OLED_WriteByte(OLED_CMD, (1 << (y % 8))); // λ��ַ

    uint8_t data[OLED_WIDTH];
    for (int i = 0; str[i] != '\0'; i++) 
    {
        char character = str[i];
        for (int j = 0; j < 16; j++) 
            data[i * 16 + j] = font8x16[character * 16 + j]; // 8*16��ģ����
        HAL_I2C_Master_Transmit(&hi2c, OLED_DATA, &data[offset], strlen(str) * 8, HAL_MAX_DELAY);
    }
}
