#ifndef __EXTI_H
#define __EXTI_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "mcu_type.h"

#define	CHANGE EXTI_Trigger_Rising_Falling
#define	FALLING EXTI_Trigger_Falling
#define	RISING EXTI_Trigger_Rising
	
#define EXTI_PreemptionPriority_Default 2
#define EXTI_SubPriority_Default 1
#define Get_EXTI_Line_x(Pin) (1 << Pin)
	
typedef void(*EXTI_CallbackFunction_t)(void);

void EXTIx_Init(uint8_t Pin, EXTI_CallbackFunction_t function, EXTITrigger_TypeDef EXTI_Trigger_x, uint8_t PreemptionPriority, uint8_t SubPriority);
void attachInterrupt(uint8_t Pin, EXTI_CallbackFunction_t function, EXTITrigger_TypeDef EXTI_Trigger_x);
void detachInterrupt(uint8_t Pin);

#ifdef __cplusplus
}
#endif

#endif
