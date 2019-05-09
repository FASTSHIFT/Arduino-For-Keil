#include "adc.h"
#include "Arduino.h"

uint16_t ADC_ConvertedValue[ADC_CHANNEL_NUM];//ADC DMA Buffer

static void ADCx_GPIO_Config(void)
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

static void Init_ADC_DMA()
{
    DMA_InitTypeDef DMA_InitStructure;

    DMA_DeInit(DMA2_Stream0);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); /*DMA2的时钟使能*/
    while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE); /*等待DMA可以配置*/


    DMA_InitStructure.DMA_Channel = DMA_Channel_0;/*DMA通道0*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR; /*外设地址*/
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)ADC_ConvertedValue;/*存取器地址*/
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;/*方向从外设到内存*/
    DMA_InitStructure.DMA_BufferSize = ADC_CHANNEL_NUM;/*数据传输的数量为3*/
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/*地址不增加*/
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;/*地址不增加*/
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;/*数据长度半字*/
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;/*数据长度半字*/
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;/*高优先级*/
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;/*循环模式*/
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;/*禁止FIFO*/
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;/*FIFO的值*/
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;/*单次传输*/
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;/*单次传输*/
    DMA_Init(DMA2_Stream0, &DMA_InitStructure); /**/
    DMA_Cmd(DMA2_Stream0, ENABLE); //开启DMA传输
}

void ADCx_DMA_Config()
{
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC时钟

    Init_ADC_DMA();

    ADC_DeInit();
    
    /*通用控制寄存器的配置*/
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;/*DMA失能*/
    ADC_CommonInitStructure.ADC_Mode          = ADC_Mode_Independent;/*独立模式*/
    ADC_CommonInitStructure.ADC_Prescaler     = ADC_Prescaler_Div6;/*APB2的4分频 即84/4=21M*/
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;/*两个采样阶段的延时5个时钟*/
    ADC_CommonInit(&ADC_CommonInitStructure);/*初始化*/
    /*初始化ADC1*/
    ADC_InitStructure.ADC_Resolution  = ADC_Resolution_12b;/*12位模式*/
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;/*扫描模式*/
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;/*连续转换*/
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;/*禁止触发检测 使用软件触发*/
    ADC_InitStructure.ADC_DataAlign    = ADC_DataAlign_Right;/*右对齐*/
    ADC_InitStructure.ADC_NbrOfConversion = ADC_CHANNEL_NUM;/*只使用1通道 规则通为1*/
    ADC_Init(ADC1, &ADC_InitStructure); /*初始化*/

    ADC_Cmd(ADC1, ENABLE); /*开启转换*/
    
    ADC_RegularChannelConfig(ADC1, PIN_MAP[PA0].ADC_Channel, 1, ADC_SampleTime_480Cycles);
    ADC_RegularChannelConfig(ADC1, PIN_MAP[PA1].ADC_Channel, 2, ADC_SampleTime_480Cycles);
    ADC_RegularChannelConfig(ADC1, PIN_MAP[PA2].ADC_Channel, 3, ADC_SampleTime_480Cycles);
    ADC_RegularChannelConfig(ADC1, PIN_MAP[PA3].ADC_Channel, 4, ADC_SampleTime_480Cycles);
    ADC_RegularChannelConfig(ADC1, PIN_MAP[PA4].ADC_Channel, 5, ADC_SampleTime_480Cycles);
    ADC_RegularChannelConfig(ADC1, PIN_MAP[PA5].ADC_Channel, 6, ADC_SampleTime_480Cycles);
    ADC_RegularChannelConfig(ADC1, PIN_MAP[PA6].ADC_Channel, 7, ADC_SampleTime_480Cycles);
    ADC_RegularChannelConfig(ADC1, PIN_MAP[PA7].ADC_Channel, 8, ADC_SampleTime_480Cycles);
    
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE); //源数据变化时开启DMA传输
    ADC_DMACmd(ADC1, ENABLE); //使能ADC传输 /*设置规则通道2 1个序列 采样时间 */
    ADC_SoftwareStartConv(ADC1);/*启动软件转换*/
}



void ADCx_DMA_Init(void)
{
    ADCx_GPIO_Config();
	ADCx_DMA_Config();
}

uint16_t Get_DMA_ADC(uint8_t Channel)
{
    if(Channel < ADC_CHANNEL_NUM)
        return ADC_ConvertedValue[Channel];
    else
        return 0;
}

void ADCx_Init(ADC_TypeDef* ADCx)
{
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef       ADC_InitStructure;
    
    ADCx_GPIO_Config();

    if(ADCx == ADC1)
    {
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);	//复位结束
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟
    }
    else if(ADCx == ADC2)
    {
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, DISABLE);	//复位结束
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); //使能ADC2时钟
    }
    else if(ADCx == ADC3)
    {
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, DISABLE);	//复位结束
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); //使能ADC3时钟
    }

    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz
    ADC_CommonInit(&ADC_CommonInitStructure);//初始化

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
    ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1
    ADC_Init(ADCx, &ADC_InitStructure);//ADC初始化


    ADC_Cmd(ADCx, ENABLE);//开启AD转换器
}

uint16_t Get_ADC(ADC_TypeDef* ADCx, uint8_t ADC_Channel)
{
    ADC_RegularChannelConfig(ADCx, ADC_Channel, 1, ADC_SampleTime_144Cycles );
    ADC_SoftwareStartConv(ADCx);
    while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC));
    return ADC_GetConversionValue(ADCx);
}
