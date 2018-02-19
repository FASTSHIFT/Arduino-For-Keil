#ifndef __EXTI_H
#define __EXTI_H	 
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "Arduino.h"

void EXTIX_Init(void);//IO初始化
void attachInterrupt(uint8_t Pin,void (*f)(),EXTITrigger_TypeDef Trigger_Mode,uint8_t SubPriority);		 					    
#endif

