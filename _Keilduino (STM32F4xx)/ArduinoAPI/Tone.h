#ifndef __TONE_H
#define __TONE_H

#include "Arduino.h"
#include "timer.h"

#define ToneVolumn_Default 80
#define ToneTimer_Default TIM1

void toneSetTimer(TIM_TypeDef* TIMx);
void tone(uint8_t Pin, uint32_t freq, uint32_t Time_ms);
void tone(uint8_t Pin, uint32_t freq);
void noTone(uint8_t Pin);

void toneBlock(uint8_t Pin, uint32_t freq, uint32_t Time_ms);
void toneBlock_Volumn(uint8_t Pin, uint32_t freq, uint32_t Time_ms, uint32_t vol);
void toneBlock_Volumn_us(uint8_t Pin, uint32_t freq, uint32_t Time_us, uint32_t vol);

#endif
