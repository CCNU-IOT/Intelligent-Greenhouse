#include "Drivers\BSP\Inc\adc.h"

// 初始化ADC
void ADC_Init(void) {
    ADC_HandleTypeDef hadc;

    // 启用ADC时钟
    __HAL_RCC_ADC1_CLK_ENABLE();

    hadc.Instance = ADC1;

    // ADC初始化结构体配置
    hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT; // 数据对齐方式设置为右对齐
    hadc.Init.ScanConvMode = DISABLE; // 单通道模式
    hadc.Init.ContinuousConvMode = DISABLE; // 单次转换
    hadc.Init.NbrOfConversion = 1; // 转换的通道数为1
    hadc.Init.DiscontinuousConvMode = DISABLE; // 不连续转换模式禁用
    hadc.Init.NbrOfDiscConversion = 1; // 不连续转换的通道数为1
    hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START; // 外部触发转换使用软件触发方式启动

    HAL_ADC_Init(&hadc);
}

// 读取ADC值
uint16_t ADC_Read(uint32_t channel) 
{
    ADC_HandleTypeDef hadc;
    ADC_ChannelConfTypeDef sConfig; //通道配置结构体变量

    // 配置ADC通道
    hadc.Instance = ADC1;
    sConfig.Channel = channel;
    sConfig.Rank = 1; // 设置通道配置结构体的排名为1
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5; // 设置通道配置结构体的采样时间

    HAL_ADC_ConfigChannel(&hadc, &sConfig);
    
    // 启动ADC转换
    HAL_ADC_Start(&hadc);

    // 等待转换完成
    HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);

    // 读取ADC值
    return HAL_ADC_GetValue(&hadc);
}
