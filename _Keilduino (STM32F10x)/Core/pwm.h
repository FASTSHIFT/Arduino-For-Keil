#ifndef __PWM_H
#define __PWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mcu_type.h"

uint8_t PWM_Init(uint8_t Pin, uint16_t PWM_DutyCycle, uint32_t PWM_Frequency);
uint16_t pwmWrite(uint8_t Pin, uint16_t val);
void TIMx_OCxInit(TIM_TypeDef* TIMx, uint16_t arr, uint16_t psc, uint8_t TimerChannel);
uint16_t Get_TIM_Channel_x(uint8_t Pin);
uint16_t timer_get_compare(TIM_TypeDef* TIMx, u8 TimerChannel);
void timer_set_prescaler(TIM_TypeDef* TIMx, uint16_t psc);
void timer_set_reload(TIM_TypeDef* TIMx, uint16_t arr);
void timer_generate_update(TIM_TypeDef* TIMx);

#ifdef __cplusplus
}
#endif

#endif
