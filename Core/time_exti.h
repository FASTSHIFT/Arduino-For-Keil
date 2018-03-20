<<<<<<< HEAD
#ifndef __TIMER_H
#define __TIMER_H

#ifdef __cplusplus
extern "C"{
#endif

#include "stm32f10x.h"
#include "Arduino.h"

enum{TIMER1,TIMER2,TIMER3,TIMER4,TIMER5,TIMER6,TIMER7,TIMER8};

void TimerSet_NVIC(u8 TimerNum,u32 time_us,void_func_point f,u8 NVIC_IRQ_CPP, u8 NVIC_IRQ_CSP);
void TimerSet(u8 TimerNum,u32 time_us,void_func_point f);

#ifdef __cplusplus
}
#endif
 
#endif
=======
#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f10x.h"

#define TIM3_start() TIM_Cmd(TIM3, ENABLE)
#define TIM3_stop() TIM_Cmd(TIM3, DISABLE)

#define TIM2_start() TIM_Cmd(TIM2, ENABLE)
#define TIM2_stop() TIM_Cmd(TIM2, DISABLE)

enum{TIMER1,TIMER2,TIMER3,TIMER4,TIMER5,TIMER6,TIMER7,TIMER8};

void TimerSet(u8 TimerNum,u32 us,void (*f)());
 
#endif
>>>>>>> 301cbab54594d1de84f6a7e456407b3a84e8d255
