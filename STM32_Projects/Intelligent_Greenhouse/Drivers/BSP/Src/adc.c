#include "Drivers\BSP\Inc\adc.h"

// ��ʼ��ADC
void ADC_Init(void) {
    ADC_HandleTypeDef hadc;

    // ����ADCʱ��
    __HAL_RCC_ADC1_CLK_ENABLE();

    hadc.Instance = ADC1;

    // ADC��ʼ���ṹ������
    hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT; // ���ݶ��뷽ʽ����Ϊ�Ҷ���
    hadc.Init.ScanConvMode = DISABLE; // ��ͨ��ģʽ
    hadc.Init.ContinuousConvMode = DISABLE; // ����ת��
    hadc.Init.NbrOfConversion = 1; // ת����ͨ����Ϊ1
    hadc.Init.DiscontinuousConvMode = DISABLE; // ������ת��ģʽ����
    hadc.Init.NbrOfDiscConversion = 1; // ������ת����ͨ����Ϊ1
    hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START; // �ⲿ����ת��ʹ�����������ʽ����

    HAL_ADC_Init(&hadc);
}

// ��ȡADCֵ
uint16_t ADC_Read(uint32_t channel) 
{
    ADC_HandleTypeDef hadc;
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
