#include "Drivers\BSP\Inc\adc.h"

    ADC_HandleTypeDef hadc;

void ADC_Init(void) 
{
    hadc.Instance = ADC1; //ѡ��ADC1

    // ADC��ʼ���ṹ������
    hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;          // ���ݶ��뷽ʽ����Ϊ�Ҷ���
    hadc.Init.ScanConvMode = DISABLE;                   // ��ͨ��ģʽ
    hadc.Init.ContinuousConvMode = DISABLE;             // ����ת��,������ת��
    hadc.Init.NbrOfConversion = 1;                      // ת����ͨ����Ϊ1
    hadc.Init.DiscontinuousConvMode = DISABLE;          // ������ת��ģʽ����
    hadc.Init.NbrOfDiscConversion = 1;                  // ������ת����ͨ����Ϊ1
    hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;    // �ⲿ����ת��ʹ�����������ʽ����

    HAL_ADC_Init(&hadc);
    HAL_ADCEx_Calibration_Start(&hadc);
}

HAL_ADC_MspInit(ADC_HandleTypeDef* hadc) //�ᱻHAL_ADC_Init���������õ�MSP�ص��������������ʹ��ADC��ͨ����ӦIO��ʱ�Ӻͳ�ʼ��IO�ڵȴ���
{
if(hadc->Instance == ADC1)
 {
    RCC_PeriphCLKInitTypeDef adc_clk_init = {0};
    GPIO_InitTypeDef gpio_init_struct;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_ADC1_CLK_ENABLE();

    // ���� ADC ����ʱ��
    adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC; // ѡ�� ADC ����ʱ��ΪҪ���õ�����ʱ��
    adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6; // ��ADC��ʱ�ӷ�Ƶ����Ϊ6������ζ��ADC��ʱ��Ƶ�ʽ�Ϊ��ʱ�ӣ�72MHz������6����12MHz��
    HAL_RCCEx_PeriphCLKConfig(&adc_clk_init); // ��ADC��ʱ������Ӧ�õ�Ӳ��������ϵͳʹ��12MHz��ʱ��Ƶ��������ADCģ�顣

    // ���� AD �ɼ�ͨ����Ӧ IO ���Ź���ģʽ
    gpio_init_struct.Pin = GPIO_PIN_1; // ѡ������1ΪҪ���õ�GPIO����
    gpio_init_struct.Mode = GPIO_MODE_ANALOG; // ����GPIO���ŵĹ���ģʽΪģ��ģʽ����ģ��ģʽ�£������Ž��������ӵ�ADCͨ�����Ա����ģ���źŵĲɼ�
    
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);
 }
}

// ��ȡADCֵ
uint16_t ADC_Read(ADC_HandleTypeDef hadc, uint32_t channel) 
{
    ADC_ChannelConfTypeDef sConfig; //ͨ�����ýṹ�����

    // ����ADCͨ��
    hadc.Instance = ADC1;
    sConfig.Channel = channel;
    sConfig.Rank = 1; // ����ͨ�����ýṹ�������Ϊ1
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5; // ����ͨ�����ýṹ��Ĳ���ʱ��

    HAL_ADC_ConfigChannel(&hadc, &sConfig);
    
    // ����ADCת��
    HAL_ADC_Start(&hadc);

    // �ȴ�ת�����
    HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);

    // ��ȡADCֵ
    return HAL_ADC_GetValue(&hadc);
}
