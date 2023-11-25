#include "../Inc/adc.h"

ADC_HandleTypeDef hadc;

void ADC_Init(void) 
{
    // ADC初始化结构体配置
    hadc.Instance = ADC1;                               // 寄存器基地址（选择ADC1）
    hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;          // 数据对齐方式设置为右对齐
    hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;          // 单通道,非扫描模式
    hadc.Init.ContinuousConvMode = DISABLE;             // 单次转换,非连续转换
    hadc.Init.NbrOfConversion = 1;                      // 转换的通道数为1
    hadc.Init.DiscontinuousConvMode = DISABLE;          // 规则通道组间断模式禁用
    hadc.Init.NbrOfDiscConversion = 0;                  // 间断模式的规则通道个数（禁用规则通道组间断模式后可忽略）
    hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;    // 触发转换方式为软件触发

    HAL_ADC_Init(&hadc);                                // 会调用回调函数HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
    HAL_ADCEx_Calibration_Start(&hadc);                 // 执行ADC自动自我校准
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc) //会被HAL_ADC_Init函数所调用的MSP回调函数，用来存放使能ADC和通道对应IO的时钟和初始化IO口等代码
{
if(hadc->Instance == ADC1)
 {
    GPIO_InitTypeDef gpio_init_struct;
    RCC_PeriphCLKInitTypeDef adc_clk_init_struct = {0};
    
    __HAL_RCC_ADC1_CLK_ENABLE(); // 使能ADC1通道时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // 设置 ADC 外设时钟
    adc_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_ADC; // 选择 ADC 外设时钟为要配置的外设时钟
    adc_clk_init_struct.AdcClockSelection = RCC_ADCPCLK2_DIV6;     /*ADC 的输入时钟频率最大值是14MHz，如果超过这个值将会导致 ADC 的转换结果准确度下降。
    此处将ADC的时钟分频设置为6，这意味着ADC的输入时钟频率将为主时钟（72MHz）除以6，即12MHz。*/
    HAL_RCCEx_PeriphCLKConfig(&adc_clk_init_struct); // 将ADC的时钟配置应用到硬件，告诉系统使用12MHz的时钟频率来驱动ADC模块。

    // 设置 AD 采集通道对应 IO 引脚（PA1）工作模式
    gpio_init_struct.Pin = GPIO_PIN_1; // 选择引脚1为要配置的GPIO引脚
    gpio_init_struct.Mode = GPIO_MODE_ANALOG; // 设置GPIO引脚的工作模式为模拟模式。在模拟模式下，该引脚将用于连接到ADC通道，以便进行模拟信号的采集
    
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);
 }
}

// 设置ADC采样通道、序列（单通道DMA则设置为ADC_REGULAR_RANK_1）和时间
void adc_channel_set(ADC_HandleTypeDef *adc_handle, uint32_t channel, uint32_t rank, uint32_t stime)
{
    ADC_ChannelConfTypeDef adc_ch_conf_struct;
    
    adc_ch_conf_struct.Channel = channel; // 通道
    adc_ch_conf_struct.Rank = rank; // 序列
    adc_ch_conf_struct.SamplingTime = stime; // 采样时间
    HAL_ADC_ConfigChannel(adc_handle, &adc_ch_conf_struct); // 配置ADC的通道，将通道与相应的参数相关联
}

// 读取ADC转换结果
uint16_t ADC_Read(uint32_t channel) 
{
    adc_channel_set(&hadc, channel, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_239CYCLES_5); // 设置通道，序列和采样时间
    HAL_ADC_Start(&hadc); // 启动ADC转换
    HAL_ADC_PollForConversion(&hadc, 10); // 等待转换完成
    return (uint16_t)HAL_ADC_GetValue(&hadc); // 返回最近一次ADC1规则组的转换结果
}

// 求多次读取平均值
uint16_t ADC_getAverage( uint8_t times, uint32_t channel)
{
    uint8_t sum = 0;
        for (uint8_t i = 0; i < times; i++)     /* 获取times次数据 */
    {
        sum += ADC_Read(channel);
        HAL_Delay(5);
    }
    return sum/times;
}
