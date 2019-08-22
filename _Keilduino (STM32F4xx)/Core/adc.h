#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mcu_type.h"

void ADC_DMA_Init(void);
int16_t ADC_DMA_Register(uint8_t ADC_Channel);
uint16_t ADC_DMA_GetValue(uint8_t ADC_Channel);

void ADCx_Init(ADC_TypeDef* ADCx);
uint16_t Get_ADC(ADC_TypeDef* ADCx, uint8_t ADC_Channel);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */
