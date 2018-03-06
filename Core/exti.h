#ifndef __EXTI_H
#define __EXTI_H	 
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "Arduino.h"

void attach_EXTI(uint8_t Pin, void_func_point f, EXTITrigger_TypeDef Trigger_Mode, uint8_t PreemptionPriority, uint8_t SubPriority);
void attachInterrupt(uint8_t Pin, void_func_point f, EXTITrigger_TypeDef Trigger_Mode);		 					    
#endif

