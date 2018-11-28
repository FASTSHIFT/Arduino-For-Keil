#ifndef __ADC_H
#define	__ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx.h"
#include "Arduino.h"

// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响

// 转换通道个数
#define ADC_CHANNEL_NUM 8
extern volatile uint16_t ADC_ConvertedValue[ADC_CHANNEL_NUM];//ADC DMA Buffer

/**************************函数声明********************************/
void ADCx_DMA_Init(void);
void ADCx_Init(ADC_TypeDef* ADCx);

uint16_t Get_DMA_ADC(uint8_t Channel);
uint16_t Get_ADC(ADC_TypeDef* ADCx, uint32_t ADC_Channel);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */
