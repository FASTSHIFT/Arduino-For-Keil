#ifndef __EXTI_H
#define __EXTI_H	 
#include "sys.h"
#include "stm32f10x_exti.h"

void EXTIX_Init(void);//IO初始化
void attachInterrupt(int Pin,void (*f)(),EXTITrigger_TypeDef Trigger_Mode,uint8_t SubPriority);		 					    
#endif

