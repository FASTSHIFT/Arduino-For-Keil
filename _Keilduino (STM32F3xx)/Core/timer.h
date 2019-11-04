/*
 * MIT License
 * Copyright (c) 2019 _VIFEXTech
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
#ifndef __TIMER_H
#define __TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mcu_type.h"

#define Timer_PreemptionPriority_Default 0
#define Timer_SubPriority_Default   3
#define IS_APB2_TIM(TIMx) (TIMx==TIM1||TIMx==TIM8||TIMx==TIM15||TIMx==TIM16||TIMx==TIM17||TIMx==TIM20)

typedef void(*Timer_CallbackFunction_t)(void);

void TimerClockCmd(TIM_TypeDef* TIMx, FunctionalState NewState);
void TimerSet(TIM_TypeDef* TIMx, uint32_t InterruptTime_us, Timer_CallbackFunction_t function);
void Timer_Init(TIM_TypeDef* TIMx, uint32_t InterruptTime_us, Timer_CallbackFunction_t function, uint8_t PreemptionPriority, uint8_t SubPriority);
void TimerSet_InterruptTimeUpdate(TIM_TypeDef* TIMx, uint32_t InterruptTime_us);

#ifdef __cplusplus
}
#endif

#endif

