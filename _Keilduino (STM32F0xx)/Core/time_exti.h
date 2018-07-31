#ifndef __TIMER_H
#define __TIMER_H

#ifdef __cplusplus
extern "C"{
#endif

#include "stm32f0xx.h"
#include "Arduino.h"

void TimerSet			(TIM_TypeDef* TIMx,uint32_t InterruptTime_us,void_func_point f);
void TimerSet_NVIC(TIM_TypeDef* TIMx,uint32_t InterruptTime_us,void_func_point f,uint8_t ChannelPriority);

#ifdef __cplusplus
}
#endif
 
#endif
