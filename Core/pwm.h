<<<<<<< HEAD
#ifndef __PWM_H
#define __PWM_H

#ifdef __cplusplus
extern "C"{
#endif
	
#include "stm32f10x.h"
#include "Arduino.h"

uint8_t PWM_Init(uint8_t Pin, uint16_t PWM_DutyCycle, uint16_t PWM_Frequency);
uint16_t pwmWrite(uint8_t Pin,uint16_t val);
void TIMx_Init(TIM_TypeDef* TIMx,uint16_t arr,uint16_t psc,uint8_t CHx);

uint16_t timer_get_compare(TIM_TypeDef* TIMx,u8 TimerChannel);
void timer_set_prescaler(TIM_TypeDef* TIMx,uint16_t psc);
void timer_set_reload(TIM_TypeDef* TIMx,uint16_t arr);
void timer_generate_update(TIM_TypeDef* TIMx);
	
#ifdef __cplusplus
}
#endif

#endif
=======
#ifndef __PWM_H
#define __PWM_H
#include "stm32f10x.h"

#define PWM_DutyCycle 1000
#define PWM_Frequency 3600

uint8_t PWM_Init(uint8_t Pin);
uint16_t pwmWrite(uint8_t Pin,u16 val);
void TIMx_Init(TIM_TypeDef* TIMx,u16 arr,u16 psc,uint8_t CHx);

uint16_t timer_get_compare(TIM_TypeDef* TIMx,u8 TimerChannel);
void timer_set_prescaler(TIM_TypeDef* TIMx,uint16_t psc);
void timer_set_reload(TIM_TypeDef* TIMx,uint16_t arr);
void timer_generate_update(TIM_TypeDef* TIMx);

#endif
>>>>>>> 301cbab54594d1de84f6a7e456407b3a84e8d255
