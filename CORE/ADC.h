#ifndef __ADC_H
#define	__ADC_H

#include "stm32f10x.h"

/*****************************************************************
**开启六通道ADC
**IO为 PC0~PC5
*******************************************************************/



// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
/********************ADC1输入通道（引脚）配置**************************/
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_CLK                       RCC_APB2Periph_ADC1

#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOC  
#define    ADC_PORT                      GPIOA


// 转换通道个数
#define    NOFCHANEL					 8

#define    ADC_PIN1                      GPIO_Pin_0
#define    ADC_CHANNEL1                  ADC_Channel_0

#define    ADC_PIN2                      GPIO_Pin_1
#define    ADC_CHANNEL2                  ADC_Channel_1

#define    ADC_PIN3                      GPIO_Pin_2
#define    ADC_CHANNEL3                  ADC_Channel_2

#define    ADC_PIN4                      GPIO_Pin_3
#define    ADC_CHANNEL4                  ADC_Channel_3

#define    ADC_PIN5                      GPIO_Pin_4
#define    ADC_CHANNEL5                  ADC_Channel_4

#define    ADC_PIN6                      GPIO_Pin_5
#define    ADC_CHANNEL6                  ADC_Channel_5

#define    ADC_PIN7                      GPIO_Pin_6
#define    ADC_CHANNEL7                  ADC_Channel_6

#define    ADC_PIN8                      GPIO_Pin_7
#define    ADC_CHANNEL8                  ADC_Channel_7


// ADC1 对应 DMA1通道1，ADC3对应DMA2通道5，ADC2没有DMA功能
#define    ADC_x                         ADC1
#define    ADC_DMA_CHANNEL               DMA1_Channel1
#define    ADC_DMA_CLK                   RCC_AHBPeriph_DMA1


/**************************函数声明********************************/
void ADCx_Init(void);
 
/**********************************************************
** 函数名:	 GetADC(u8 channel)
** 功能描述: 获取ADC数值
** 输入参数: 通道号
** 输出参数: ADC读取值
***********************************************************/
uint16_t GetADC(u8 channel);


#endif /* __ADC_H */

