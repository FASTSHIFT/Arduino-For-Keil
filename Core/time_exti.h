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
