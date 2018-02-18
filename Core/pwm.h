#ifndef __PWM_H
#define __PWM_H
#include "sys.h"

//extern TIM_TypeDef* TIMx[4];
//extern int PWM_Pin[4][4];

#define PWM_DutyCycle 1000
#define PWM_Frequency 3600

uint8_t GetPWM_TIMx_CHx(int Pin);
uint8_t PWM_Init(int Pin);
uint16_t pwmWrite(int pin,u16 val);
void TIMx_Init(TIM_TypeDef* TIMx,u16 arr,u16 psc,u8 CHx);

#endif
