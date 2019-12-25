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
  * @param  ADC_Channel: ADC通道
  * @retval 无
  */
uint16_t ADCx_GetValue(ADC_TypeDef* ADCx, uint32_t ADC_Channel)
{
    uint16_t ADCConvertedValue;
    //Select channel to scan
    ADC_ChannelConfig(ADCx, ADC_Channel, ADC_SampleTime_41_5Cycles);
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
