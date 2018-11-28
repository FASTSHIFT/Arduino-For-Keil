#ifndef __TIMER_H
#define __TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx.h"
#include "Arduino.h"
	
#define Timer_ChannelPriority_Default	2
	
typedef void(*Timer_CallbackFunction_t)(void);

void TimerSet(TIM_TypeDef* TIMx, uint32_t InterruptTime_us, Timer_CallbackFunction_t function);
void Timer_Init(TIM_TypeDef* TIMx, uint32_t InterruptTime_us, Timer_CallbackFunction_t function, uint8_t ChannelPriority);
void TimerSet_InterruptTimeUpdate(TIM_TypeDef* TIMx, uint32_t InterruptTime_us);
	
#ifdef __cplusplus
}
#endif

#endif

