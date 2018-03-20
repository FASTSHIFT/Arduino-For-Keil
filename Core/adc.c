/**多通道ADC读取**/
#include "ADC.h"

uint16_t ADC_ConvertedValue[ADC_CHANNEL_NUM];//ADC DMA Buffer

void ADCx_GPIO_Config(void)
{
	pinMode(PA0,INPUT_ANALOG);
	pinMode(PA1,INPUT_ANALOG);
	pinMode(PA2,INPUT_ANALOG);
	pinMode(PA3,INPUT_ANALOG);
	pinMode(PA4,INPUT_ANALOG);
	pinMode(PA5,INPUT_ANALOG);
	pinMode(PA6,INPUT_ANALOG);
	pinMode(PA7,INPUT_ANALOG);
}

/**
  * @brief  配置ADC工作模式
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
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( ADC1->DR ) );
	
	// 存储器地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;
	
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
	DMA_Cmd(DMA1_Channel1 , ENABLE);
	
	// ADC 模式配置
	// 只使用一个ADC，属于单模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// 扫描模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 

	// 连续转换模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

	// 不用外部触发转换，软件开启即可
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

	// 转换结果右对齐
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// 转换通道个数
	ADC_InitStructure.ADC_NbrOfChannel = ADC_CHANNEL_NUM;	
		
	// 初始化ADC
	ADC_Init(ADC1, &ADC_InitStructure);
	
	// 配置ADC时钟Ｎ狿CLK2的8分频，即9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	// 配置ADC 通道的转换顺序和采样时间
	ADC_RegularChannelConfig(ADC1, PIN_MAP[PA0].ADC_Channel, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, PIN_MAP[PA1].ADC_Channel, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, PIN_MAP[PA2].ADC_Channel, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, PIN_MAP[PA3].ADC_Channel, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, PIN_MAP[PA4].ADC_Channel, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, PIN_MAP[PA5].ADC_Channel, 6, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, PIN_MAP[PA6].ADC_Channel, 7, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, PIN_MAP[PA7].ADC_Channel, 8, ADC_SampleTime_55Cycles5);
	
	// 使能ADC DMA 请求
	ADC_DMACmd(ADC1, ENABLE);
	
	// 开启ADC ，并开始转换
	ADC_Cmd(ADC1, ENABLE);
	
	// 初始化ADC 校准寄存器  
	ADC_ResetCalibration(ADC1);
	// 等待校准寄存器初始化完成
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	// ADC开始校准
	ADC_StartCalibration(ADC1);
	// 等待校准完成
	while(ADC_GetCalibrationStatus(ADC1));
	
	// 由于没有采用外部触发，所以使用软件触发ADC转换 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/**
  * @brief  ADC初始化
  * @param  无
  * @retval 无
  */
void ADCx_DMA_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_DMA_Config();
}
 
/**********************************************************
** 函数名:	 GetADC(u8 channel)
** 功能描述: 获取ADC数值
** 输入参数: 通道号
** 输出参数: ADC读取值
***********************************************************/
uint16_t Get_DMA_ADC(uint8_t channel)
{
	return ADC_ConvertedValue[channel];
}


/*********************************ADC No DMA**********************************/

void ADCx_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1	, ENABLE );
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}				  


uint16_t Get_ADC(uint16_t channel)   
{
	unsigned int result=0;
	unsigned char i;

	ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_239Cycles5 );
  for(i=0;i<8;i++)
	{
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
		result+=ADC_GetConversionValue(ADC1);
	}
	result=result/8;
	return result;
}

uint16_t ADC_Pin_Read(uint8_t Pin)
{
	if(IS_ADC_PIN(Pin))
	{
		return Get_ADC(PIN_MAP[Pin].ADC_Channel);
	}
	else return 0;
}
