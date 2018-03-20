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
