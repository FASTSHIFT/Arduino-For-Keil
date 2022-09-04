/*
 * MIT License
 * Copyright (c) 2019 _VIFEXTech
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "adc.h"

#define ADC_DMA_REGMAX 18

/*引脚注册个数*/
static volatile uint8_t ADC_DMA_RegCnt = 0;

/*ADC通道注册列表*/
static volatile uint32_t ADC_DMA_RegChannelList[ADC_DMA_REGMAX] = {0};

/*ADC DMA缓存数组*/
static volatile uint16_t ADC_DMA_ConvertedValue[ADC_DMA_REGMAX] = {0};

/**
  * @brief  搜索注册列表，找出ADC通道对应的索引号
  * @param  ADC_Channel:ADC通道号
  * @retval ADC通道注册列表对应索引号，-1:未找到对应通道号
  */
static int16_t ADC_DMA_SearchChannel(uint32_t ADC_Channel)
{
    uint8_t index;

    for(index = 0; index < ADC_DMA_RegCnt; index++)
    {
        if(ADC_Channel == ADC_DMA_RegChannelList[index])
        {
            return index;
        }
    }
    return -1;
}

/**
  * @brief  注册需要DMA搬运的ADC通道
  * @param  ADC_Channel:ADC通道号
  * @retval 引脚注册列表对应索引号，-1:不支持ADC，-2:引脚重复注册，-3:超出最大注册个数
  */
int16_t ADC_DMA_Register(uint32_t ADC_Channel)
{
    /*初始化ADC通道列表*/
    static uint8_t IsInit = 0;
    if(!IsInit)
    {
        uint8_t i;
        for(i = 0; i < ADC_DMA_REGMAX; i++)
        {
            ADC_DMA_RegChannelList[i] = 0xFFFFFFFF;
        }
        IsInit = 1;
    }

    /*是否是合法ADC通道*/
    if(!IS_ADC_CHANNEL(ADC_Channel))
        return -1;

    /*是否已在引脚列表重复注册*/
    if(ADC_DMA_SearchChannel(ADC_Channel) != -1)
        return -2;

    /*是否超出最大注册个数*/
    if(ADC_DMA_RegCnt >= ADC_DMA_REGMAX)
        return -3;

    /*写入注册列表*/
    ADC_DMA_RegChannelList[ADC_DMA_RegCnt] = ADC_Channel;

    /*注册个数+1*/
    ADC_DMA_RegCnt++;

    return ADC_DMA_RegCnt - 1;
}

/**
  * @brief  ADC DMA 配置
  * @param  无
  * @retval 无
  */
void ADC_DMA_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    uint16_t index;

    // 打开DMA时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    // 复位DMA控制器
    DMA_DeInit(DMA1_Channel1);

    // 配置 DMA 初始化结构体
    // 外设基址为：ADC 数据寄存器地址
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&(ADC1->DR));

    // 存储器地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_DMA_ConvertedValue;

    // 数据源来自外设
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;

    // 缓冲区大小，应该等于数据目的地的大小
    DMA_InitStructure.DMA_BufferSize = ADC_DMA_RegCnt;

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

    /* ADC1 DeInit */  
    ADC_DeInit(ADC1);

    /* ADC1 Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    // ADC 模式配置
    ADC_StructInit(&ADC_InitStructure);

    // 分辨率
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;

    // 连续转换模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

    // 不用外部触发转换，软件开启即可
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;

    // 转换结果右对齐
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

    // 扫描方向
    ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;

    // 初始化ADC
    ADC_Init(ADC1, &ADC_InitStructure);

    // 配置ADC 通道的转换顺序和采样时间
    for(index = 0; index < ADC_DMA_RegCnt; index++)
    {
        ADC_ChannelConfig(
            ADC1,
            ADC_DMA_RegChannelList[index],
            ADC_SampleTime_41_5Cycles
        );

        if(ADC_DMA_RegChannelList[index] == ADC_Channel_TempSensor)
        {
            ADC_TempSensorCmd(ENABLE);
        }
        else if(ADC_DMA_RegChannelList[index] == ADC_Channel_Vrefint)
        {
            ADC_VrefintCmd(ENABLE);
        }
    }

    /* ADC校准 */
    ADC_GetCalibrationFactor(ADC1);

    /* ADC DMA request in circular mode */
    ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);

    /* Enable ADC_DMA */
    ADC_DMACmd(ADC1, ENABLE);

    /* Enable the ADC peripheral */
    ADC_Cmd(ADC1, ENABLE);

    /* Wait the ADRDY flag */
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY));

    /* ADC1 regular Software Start Conv */
    ADC_StartOfConversion(ADC1);
}

/**
  * @brief  获取DMA搬运的ADC值
  * @param  ADC_Channel:ADC通道号
  * @retval ADC值
  */
uint16_t ADC_DMA_GetValue(uint32_t ADC_Channel)
{
    int16_t index;

    if(!IS_ADC_CHANNEL(ADC_Channel))
        return 0;

    index = ADC_DMA_SearchChannel(ADC_Channel);
    if(index == -1)
        return 0;

    return ADC_DMA_ConvertedValue[index];
}

/**
  * @brief  ADC 配置
  * @param  ADCx: ADC地址
  * @retval 无
  */
void ADCx_Init(ADC_TypeDef* ADCx)
{
    ADC_InitTypeDef ADC_InitStructure;
    ADC_DeInit(ADCx);
    if(ADCx == ADC1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    }
    else
    {
        return;
    }
    ADC_StructInit(&ADC_InitStructure);//初始化ADC结构
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位精度
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //规定模式装换工作在连续模式
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据对其为右对齐
    ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward; //ADC的扫描方向
    ADC_Init(ADCx, &ADC_InitStructure);
    ADC_GetCalibrationFactor(ADCx);
    ADC_Cmd(ADCx, ENABLE);
    while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_ADEN));
}

/**
  * @brief  获取 ADC 值
  * @param  ADCx: ADC地址
  * @param  ADC_Channel: ADC通道
  * @retval 无
  */
uint16_t ADCx_GetValue(ADC_TypeDef* ADCx, uint32_t ADC_Channel)
{
    uint16_t ADCConvertedValue;
    //Select channel to scan
    ADCx->CHSELR = (uint32_t)ADC_Channel;
    ADC_ChannelConfig(ADCx, ADC_Channel, ADC_SampleTime_41_5Cycles);
    //ADC1 regular Software Start Conv
    ADC_StartOfConversion(ADCx);
    //Wait for end of conversion
    while(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET);
    //Get value
    ADCConvertedValue = ADC_GetConversionValue(ADCx);
    //Stop conversion and return ADC value.
    ADC_StopOfConversion(ADCx);
    return(ADCConvertedValue);
}
