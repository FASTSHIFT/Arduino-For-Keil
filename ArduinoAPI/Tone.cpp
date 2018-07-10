#include "Tone.h"

uint8_t tone_State = On;
uint8_t tone_Pin;
uint32_t tone_StopTimePoint;

void tone_Cmd(uint8_t State)
{
	tone_State = State;
}

void tone_RingHandler()
{
	PIN_MAP[tone_Pin].GPIOx->ODR^=PIN_MAP[tone_Pin].GPIO_Pin_x;
}

void tone_DelayHandler()
{
	if(millis() > tone_StopTimePoint)
	{
		noTone(tone_Pin);
	}
}

void tone(uint8_t Pin, uint32_t freq,uint32_t Time_ms)
{
	if(freq == 0 || Time_ms == 0) return;
	tone(Pin, freq);
	
	tone_StopTimePoint = millis() + Time_ms;
	TimerSet(ToneDelayTimer_Default, 1000, tone_DelayHandler);
	TIM_Cmd(ToneDelayTimer_Default,ENABLE);
}

void tone(uint8_t Pin, uint32_t freq)
{
	if(freq == 0) return;
	tone_Pin = Pin;
	
	if(IS_PWM_PIN(tone_Pin))
	{
		PWM_Init(tone_Pin, 101, freq);
		pwmWrite(tone_Pin, ToneVolumn_Default);
	}
	else
	{
		pinMode(tone_Pin, OUTPUT);
		uint32_t SetTime_us = 500000 / freq;
		TimerSet(ToneRingTimer_Default, SetTime_us, tone_RingHandler);
		TIM_Cmd(ToneRingTimer_Default, ENABLE);
	}
}

void noTone(uint8_t Pin)
{
	if(IS_PWM_PIN(Pin))
	{
		pwmWrite(Pin, 0);
		TIM_Cmd(PIN_MAP[Pin].TIMx, DISABLE);
	}
	else
	{
		TIM_Cmd(ToneRingTimer_Default,DISABLE);
		digitalWrite_LOW(Pin);
	}
	
	TIM_Cmd(ToneDelayTimer_Default,DISABLE);
}

void Block_tone(uint8_t Pin, uint32_t freq, uint32_t Time_ms)
{
	uint32_t TimePoint = millis() + Time_ms;
	uint32_t dlyus = 500000 / freq;
	if(tone_State == Off)return;
	if(freq == 0)return;
	do
	{
		digitalWrite_HIGH(Pin);
		delayMicroseconds(dlyus);
		digitalWrite_LOW(Pin);
		delayMicroseconds(dlyus);
	}while(millis() < TimePoint);
	digitalWrite_LOW(Pin);
}

void Block_toneVolumn(uint8_t Pin,uint32_t freq,uint32_t Time_ms,uint32_t vol)
{
	uint32_t TimePoint = millis() + Time_ms;
	uint32_t dlyus = 500000 / freq;
	uint32_t dlHigh = dlyus * vol / 100;
	uint32_t dlLow = 2 * dlyus - dlHigh;
	if(freq == 0)return;
	do
	{
		digitalWrite_HIGH(Pin);
		delayMicroseconds(dlHigh);
		digitalWrite_LOW(Pin);
		delayMicroseconds(dlLow);
	}while(millis() < TimePoint);
	digitalWrite_LOW(Pin);
}

void Block_toneVolumn_us(uint8_t Pin,uint32_t freq,uint32_t Time_us,uint32_t vol)
{
	uint32_t TimePoint = micros() + Time_us;
	uint32_t dlyus = 500000 / freq;
	uint32_t dlHigh = dlyus * vol / 100;
	uint32_t dlLow = 2 * dlyus - dlHigh;
	if(freq == 0)return;
	do
	{
		digitalWrite_HIGH(Pin);
		delayMicroseconds(dlHigh);
		digitalWrite_LOW(Pin);
		delayMicroseconds(dlLow);
	}while(micros() < TimePoint);
	digitalWrite_LOW(Pin);
}
