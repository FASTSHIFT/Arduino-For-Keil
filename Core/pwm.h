#ifndef __PWM_H
#define __PWM_H
#include "stm32f10x.h"

#define PWM_DutyCycle 1000
#define PWM_Frequency 3600

uint8_t GetPWM_TIMx_CHx(uint8_t Pin);
uint8_t PWM_Init(uint8_t Pin);
uint16_t pwmWrite(uint8_t Pin,u16 val);
void TIMx_Init(TIM_TypeDef* TIMx,u16 arr,u16 psc,uint8_t CHx);

#endif
