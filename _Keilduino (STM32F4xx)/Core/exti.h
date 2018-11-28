#ifndef __EXTI_H
#define __EXTI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
#include "stm32f4xx_exti.h"
#include "Arduino.h"

#define Get_EXTI_Linex(Pin) (1 << Pinx)
#define Get_EXTI_PortSourceGPIOx(Pin) (Get_GPIOx(Pin))
#define Get_EXTI_PinSourcex(Pin) (Get_Pinx(Pin))

#define	CHANGE EXTI_Trigger_Rising_Falling
#define	FALLING EXTI_Trigger_Falling
#define	RISING EXTI_Trigger_Rising
	
#define EXTI_PreemptionPriority_Default 2
#define EXTI_SubPriority_Default 1
	
typedef void(*EXTI_CallbackFunction_t)(void);

void EXTIx_Init(uint8_t Pin, EXTI_CallbackFunction_t function, EXTITrigger_TypeDef Trigger_Mode, uint8_t PreemptionPriority, uint8_t SubPriority);
void attachInterrupt(uint8_t Pin, EXTI_CallbackFunction_t function, EXTITrigger_TypeDef Trigger_Mode);
void detachInterrupt(uint8_t Pin);

#ifdef __cplusplus
}
#endif

#endif
