/*
 * MIT License
 * Copyright (c) 2017 - 2022 _VIFEXTech
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
#include <stdbool.h>

#define ADC_DMA_REGMAX 18

#define IS_ADC_CHANNEL(channel) (channel <= ADC_CHANNEL_18)

/*引脚注册个数*/
static uint8_t ADC_DMA_RegCnt = 0;

/*ADC通道注册列表*/
static uint8_t ADC_DMA_RegChannelList[ADC_DMA_REGMAX] = {0};

/*ADC DMA缓存数组*/
static uint16_t ADC_DMA_ConvertedValue[ADC_DMA_REGMAX] = {0};

/**
  * @brief  搜索注册列表，找出ADC通道对应的索引号
  * @param  ADC_Channel:ADC通道号
  * @retval ADC通道注册列表对应索引号，-1:未找到对应通道号
  */
static int16_t ADC_DMA_SearchChannel(uint16_t ADC_Channel)
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
  * @brief  ADC 配置
  * @param  ADCx: ADC地址
  * @retval 无
  */
void ADCx_Init(adc_type* ADCx)
{
    adc_common_config_type adc_common_struct;
    adc_base_config_type adc_base_struct;

    if(ADCx == ADC1)
    {
        crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, TRUE);
    }
    else if(ADCx == ADC2)
    {
        crm_periph_clock_enable(CRM_ADC2_PERIPH_CLOCK, TRUE);
    }
    else if(ADCx == ADC3)
    {
        crm_periph_clock_enable(CRM_ADC3_PERIPH_CLOCK, TRUE);
    }
    else
    {
        return;
    }

    adc_common_default_para_init(&adc_common_struct);
    adc_common_struct.combine_mode = ADC_INDEPENDENT_MODE;
    adc_common_struct.div = ADC_HCLK_DIV_4;
    adc_common_struct.common_dma_mode = ADC_COMMON_DMAMODE_DISABLE;
    adc_common_struct.common_dma_request_repeat_state = FALSE;
    adc_common_struct.sampling_interval = ADC_SAMPLING_INTERVAL_5CYCLES;
    adc_common_struct.tempervintrv_state = FALSE;
    adc_common_struct.vbat_state = FALSE;
    adc_common_config(&adc_common_struct);

    adc_base_default_para_init(&adc_base_struct);
    adc_base_struct.sequence_mode = FALSE;
    adc_base_struct.repeat_mode = FALSE;
    adc_base_struct.data_align = ADC_RIGHT_ALIGNMENT;
    adc_base_struct.ordinary_channel_length = 1;
    adc_base_config(ADCx, &adc_base_struct);
    adc_resolution_set(ADCx, ADC_RESOLUTION_12B);

    adc_ordinary_conversion_trigger_set(ADCx, ADC_ORDINARY_TRIG_TMR1CH1, ADC_ORDINARY_TRIG_EDGE_NONE);

    adc_dma_mode_enable(ADCx, FALSE);
    adc_dma_request_repeat_enable(ADCx, FALSE);
    adc_interrupt_enable(ADCx, ADC_OCCO_INT, FALSE);

    adc_enable(ADCx, TRUE);
    while(adc_flag_get(ADCx, ADC_RDY_FLAG) == RESET);

    /* adc calibration */
    adc_calibration_init(ADCx);
    while(adc_calibration_init_status_get(ADCx));
    adc_calibration_start(ADCx);
    while(adc_calibration_status_get(ADCx));
}

/**
  * @brief  获取 ADC 值
  * @param  ADCx: ADC地址
  * @param  ADC_Channel: ADC通道
  * @retval 无
  */
uint16_t ADCx_GetValue(adc_type* ADCx, uint16_t ADC_Channel)
{
    adc_ordinary_channel_set(ADCx, (adc_channel_select_type)ADC_Channel, 1, ADC_SAMPLETIME_47_5);

    adc_ordinary_software_trigger_enable(ADCx, TRUE);
    while(!adc_flag_get(ADCx, ADC_OCCE_FLAG));

    return adc_ordinary_conversion_data_get(ADCx);
}

/**
  * @brief  注册需要DMA搬运的ADC通道
  * @param  ADC_Channel:ADC通道号
  * @retval 见ADC_DMA_Res_Type
  */
ADC_DMA_Res_Type ADC_DMA_Register(uint8_t ADC_Channel)
{
    /*初始化ADC通道列表*/
    static bool isInit = false;
    if(!isInit)
    {
        uint8_t i;
        for(i = 0; i < ADC_DMA_REGMAX; i++)
        {
            ADC_DMA_RegChannelList[i] = 0xFF;
        }
        isInit = true;
    }

    /*是否是合法ADC通道*/
    if(!IS_ADC_CHANNEL(ADC_Channel))
        return ADC_DMA_RES_NOT_ADC_CHANNEL;

    /*是否已在引脚列表重复注册*/
    if(ADC_DMA_SearchChannel(ADC_Channel) != -1)
        return ADC_DMA_RES_DUPLICATE_REGISTRATION;

    /*是否超出最大注册个数*/
    if(ADC_DMA_RegCnt >= ADC_DMA_REGMAX)
        return ADC_DMA_RES_MAX_NUM_OF_REGISTRATIONS_EXCEEDED;

    /*写入注册列表*/
    ADC_DMA_RegChannelList[ADC_DMA_RegCnt] = ADC_Channel;

    /*注册个数+1*/
    ADC_DMA_RegCnt++;

    return ADC_DMA_RES_OK;
}

/**
  * @brief  获取已注册的ADC DMA通道数量
  * @param  无
  * @retval ADC DMA通道数量
  */
uint8_t ADC_DMA_GetRegisterCount(void)
{
    return ADC_DMA_RegCnt;
}

/**
  * @brief  ADC DMA 配置
  * @param  无
  * @retval 无
  */
void ADC_DMA_Init(void)
{
    dma_init_type dma_init_structure;
    adc_common_config_type adc_common_struct;
    adc_base_config_type adc_base_struct;
    uint8_t index;

    crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);

    dma_reset(DMA1_CHANNEL1);

    dma_default_para_init(&dma_init_structure);
    dma_init_structure.buffer_size = ADC_DMA_RegCnt;
    dma_init_structure.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
    dma_init_structure.memory_base_addr = (uint32_t)ADC_DMA_ConvertedValue;
    dma_init_structure.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
    dma_init_structure.memory_inc_enable = TRUE;
    dma_init_structure.peripheral_base_addr = (uint32_t) (&(ADC1->odt));
    dma_init_structure.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
    dma_init_structure.peripheral_inc_enable = FALSE;
    dma_init_structure.priority = DMA_PRIORITY_HIGH;
    dma_init_structure.loop_mode_enable = TRUE;

    dma_init(DMA1_CHANNEL1, &dma_init_structure);

    dmamux_enable(DMA1, TRUE);
    dmamux_init(DMA1MUX_CHANNEL1, DMAMUX_DMAREQ_ID_ADC1);

    adc_reset();

    adc_common_default_para_init(&adc_common_struct);
    adc_common_struct.combine_mode = ADC_INDEPENDENT_MODE;
    adc_common_struct.div = ADC_HCLK_DIV_4;
    adc_common_struct.common_dma_mode = ADC_COMMON_DMAMODE_DISABLE;
    adc_common_struct.common_dma_request_repeat_state = FALSE;
    adc_common_struct.sampling_interval = ADC_SAMPLING_INTERVAL_5CYCLES;
    adc_common_struct.tempervintrv_state = FALSE;
    adc_common_struct.vbat_state = FALSE;
    adc_common_config(&adc_common_struct);

    adc_base_default_para_init(&adc_base_struct);

    adc_base_struct.sequence_mode = TRUE;
    adc_base_struct.repeat_mode = TRUE;
    adc_base_struct.data_align = ADC_RIGHT_ALIGNMENT;
    adc_base_struct.ordinary_channel_length = ADC_DMA_RegCnt;
    adc_base_config(ADC1, &adc_base_struct);
    adc_resolution_set(ADC1, ADC_RESOLUTION_12B);

    for(index = 0; index < ADC_DMA_RegCnt; index++)
    {
        adc_ordinary_channel_set(
            ADC1,
            (adc_channel_select_type)ADC_DMA_RegChannelList[index],
            index + 1,
            ADC_SAMPLETIME_47_5
        );
    }

    adc_ordinary_conversion_trigger_set(ADC1, ADC_ORDINARY_TRIG_TMR1CH1, ADC_ORDINARY_TRIG_EDGE_NONE);

    adc_dma_mode_enable(ADC1, TRUE);

    adc_dma_request_repeat_enable(ADC1, TRUE);

    adc_interrupt_enable(ADC1, ADC_OCCO_INT, FALSE);

    adc_enable(ADC1, TRUE);
    while(adc_flag_get(ADC1, ADC_RDY_FLAG) == RESET);

    adc_calibration_init(ADC1);
    while(adc_calibration_init_status_get(ADC1));
    adc_calibration_start(ADC1);
    while(adc_calibration_status_get(ADC1));
}

/**
  * @brief  获取DMA搬运的ADC值
  * @param  ADC_Channel:ADC通道号
  * @retval ADC值
  */
uint16_t ADC_DMA_GetValue(uint8_t ADC_Channel)
{
    int16_t index;

    if(!IS_ADC_CHANNEL(ADC_Channel))
        return 0;

    index = ADC_DMA_SearchChannel(ADC_Channel);
    if(index == -1)
        return 0;

    return ADC_DMA_ConvertedValue[index];
}
