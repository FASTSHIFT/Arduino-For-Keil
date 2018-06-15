#ifndef __EXTI_H
#define __EXTI_H

#ifdef __cplusplus
extern "C"{
#endif

#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "Arduino.h"
	
#define	CHANGE EXTI_Trigger_Rising_Falling
#define	FALLING EXTI_Trigger_Falling
#define	RISING EXTI_Trigger_Rising

void attach_EXTI(uint8_t Pin, void_func_point f, EXTITrigger_TypeDef Trigger_Mode, uint8_t PreemptionPriority, uint8_t SubPriority);
void attachInterrupt(uint8_t Pin, void_func_point f, EXTITrigger_TypeDef Trigger_Mode);
	
#ifdef __cplusplus
}
#endif

#endif
