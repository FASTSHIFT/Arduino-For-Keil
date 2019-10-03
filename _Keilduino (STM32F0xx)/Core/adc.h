#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mcu_type.h"

void ADCx_Init(ADC_TypeDef* ADCx);
uint16_t Get_ADC(ADC_TypeDef* ADCx, uint32_t ADC_Channel);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */
