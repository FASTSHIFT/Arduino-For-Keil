#include "adc.h"

volatile uint16_t ADC_ConvertedValue[ADC_CHANNEL_NUM];//ADC DMA Buffer

/**
  * @brief  将PA0~PA7配置为模拟输入模式，用于DMA
  * @param  无
  * @retval 无
  */
void ADCx_GPIO_Config(void)
{
    pinMode(PA0, INPUT_ANALOG);
    pinMode(PA1, INPUT_ANALOG);
    pinMode(PA2, INPUT_ANALOG);
    pinMode(PA3, INPUT_ANALOG);
    pinMode(PA4, INPUT_ANALOG);
    pinMode(PA5, INPUT_ANALOG);
    pinMode(PA6, INPUT_ANALOG);
    pinMode(PA7, INPUT_ANALOG);
}

/**
  * @brief  ADC DMA 配置
  * @param  无
  * @retval 无
  */
void ADCx_DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    // 打开DMA时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    // 打开ADC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    // 复位DMA控制器
    DMA_DeInit(DMA1_Channel1);

    // 配置 DMA 初始化结构体
    // 外设基址为：ADC 数据寄存器地址
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));

    // 存储器地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_ConvertedValue;

    // 数据源来自外设
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;

    // 缓冲区大小，应该等于数据目的地的大小
    DMA_InitStructure.DMA_BufferSize = ADC_CHANNEL_NUM;

    // 外设寄存器只有一个，地址不用递增
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

    // 存储器地址递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

    // 外设数据大小为半字，即两个字节
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;

    // 内存数据大小也为半字，跟外设数据大小相同
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;

    // 循环传输模式
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;

    // DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;

    // 禁止存储器到存储器模式，因为是从外设到存储器
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    // 初始化DMA
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    // 使能 DMA 通道
    DMA_Cmd(DMA1_Channel1, ENABLE);

    DMA_Cmd(DMA1_Channel1, ENABLE);/* DMA1 Channel1 enable */
    ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular); /* Enable ADC_DMA */
    ADC_DMACmd(ADC1, ENABLE);

    // ADC 模式配置
    ADC_DeInit(ADC1);
    ADC_StructInit(&ADC_InitStructure);//初始化ADC结构
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位精度
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //规定模式装换工作在连续模式
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据对其为右对齐
    ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward; //ADC的扫描方向
    ADC_Init(ADC1, &ADC_InitStructure);

    // 配置ADC 通道的转换顺序和采样时间
    ADC_ChannelConfig(ADC1, PIN_MAP[PA0].ADC_Channel, ADC_SampleTime_239_5Cycles);
    ADC_ChannelConfig(ADC1, PIN_MAP[PA1].ADC_Channel, ADC_SampleTime_239_5Cycles);
    ADC_ChannelConfig(ADC1, PIN_MAP[PA2].ADC_Channel, ADC_SampleTime_239_5Cycles);
    ADC_ChannelConfig(ADC1, PIN_MAP[PA3].ADC_Channel, ADC_SampleTime_239_5Cycles);
    ADC_ChannelConfig(ADC1, PIN_MAP[PA4].ADC_Channel, ADC_SampleTime_239_5Cycles);
    ADC_ChannelConfig(ADC1, PIN_MAP[PA5].ADC_Channel, ADC_SampleTime_239_5Cycles);
    ADC_ChannelConfig(ADC1, PIN_MAP[PA6].ADC_Channel, ADC_SampleTime_239_5Cycles);
    ADC_ChannelConfig(ADC1, PIN_MAP[PA7].ADC_Channel, ADC_SampleTime_239_5Cycles);

    ADC_GetCalibrationFactor(ADC1); /* ADC Calibration */
    ADC_Cmd(ADC1, ENABLE);  /* Enable ADCperipheral[PerIdx] */
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN)); /* Wait the ADCEN falg */
    ADC_StartOfConversion(ADC1); /* ADC1 regular Software Start Conv */
}

/**
  * @brief  ADC DMA 初始化
  * @param  无
  * @retval 无
  */
void ADCx_DMA_Init(void)
{
    ADCx_GPIO_Config();
    ADCx_DMA_Config();
}

/**
  * @brief  获取 ADC DMA 值
  * @param  Channel: ADC通道号
  * @retval 无
  */
uint16_t Get_DMA_ADC(uint8_t Channel)
{
    return ADC_ConvertedValue[Channel];
}

/**
  * @brief  ADC 配置
  * @param  ADCx: ADC地址
  * @retval 无
  */
void ADCx_Init(ADC_TypeDef* ADCx)
{
    ADC_InitTypeDef ADC_InitStructure;
    if(ADCx == ADC1)RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    ADC_DeInit(ADCx);
    ADC_StructInit(&ADC_InitStructure);//初始化ADC结构
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位精度
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //规定模式装换工作在连续模式
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据对其为右对齐
    ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward; //ADC的扫描方向
    ADC_Init(ADCx, &ADC_InitStructure);
    ADC_Cmd(ADCx, ENABLE);
    while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_ADEN));
}

/**
  * @brief  获取 ADC 值
  * @param  ADCx: ADC地址
			ADC_Channel: ADC通道
  * @retval 无
  */
uint16_t Get_ADC(ADC_TypeDef* ADCx, uint32_t ADC_Channel)
{
    uint16_t ADCConvertedValue;
    //Select channel to scan
    ADC_ChannelConfig(ADCx, ADC_Channel, ADC_SampleTime_239_5Cycles);
    // ADC1 regular Software Start Conv
    ADC_StartOfConversion(ADCx);
    // Wait for end of conversion
    while(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET);
    //Get value
    ADCConvertedValue = ADC_GetConversionValue(ADCx);
    //Stop conversion and return ADC value.
    ADC_StopOfConversion(ADCx);
    return(ADCConvertedValue);
}
