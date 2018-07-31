#ifndef __TONE_H
#define __TONE_H

#include "Arduino.h"
#include "time_exti.h"

#define ToneVolumn_Default 80
#define ToneRingTimer_Default TIM1
#define ToneDelayTimer_Default TIM3

void tone_Cmd(uint8_t State);
void tone(uint8_t Pin, uint32_t freq,uint32_t Time_ms);
void tone(uint8_t Pin, uint32_t freq);
void noTone(uint8_t Pin);

void Block_tone(uint8_t Pin,uint32_t freq,uint32_t Time_ms);
void Block_toneVolumn(uint8_t Pin,uint32_t freq,uint32_t Time_ms,uint32_t vol);
void Block_toneVolumn_us(uint8_t Pin,uint32_t freq,uint32_t Time_us,uint32_t vol);

#endif
