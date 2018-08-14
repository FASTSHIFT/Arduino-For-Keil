#include "Tone.h"

TIM_TypeDef* ToneTimer = ToneTimer_Default;
uint8_t tone_State = Off;
uint8_t tone_Pin;
uint32_t tone_StopTimePoint;

void tone_Cmd(uint8_t State)
{
	tone_State = State;
}

void tone_TimerHandler()
{
	togglePin(tone_Pin);
	if(millis() > tone_StopTimePoint && !tone_State)
	{
		noTone(tone_Pin);
	}
}

void toneSetTimer(TIM_TypeDef* TIMx)
{
	ToneTimer = TIMx;
}

void tone(uint8_t Pin, uint32_t freq,uint32_t Time_ms)
{
	if(freq == 0 || Time_ms == 0) return;
	tone(Pin, freq);
	tone_StopTimePoint = millis() + Time_ms;
	tone_State = Off;
}

void tone(uint8_t Pin, uint32_t freq)
{
	if(freq == 0 || freq > 500000) return;
	tone_Pin = Pin;
	tone_State = On;
	
	pinMode(tone_Pin, OUTPUT);
	TimerSet_NVIC(ToneTimer, (500000 / freq), tone_TimerHandler,0,0);
	TIM_Cmd(ToneTimer,ENABLE);
}

void noTone(uint8_t Pin)
{
	TIM_Cmd(ToneTimer,DISABLE);
	digitalWrite_LOW(Pin);
	tone_State = Off;
}

void toneBlock(uint8_t Pin, uint32_t freq, uint32_t Time_ms)
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

void toneBlock_Volumn(uint8_t Pin,uint32_t freq,uint32_t Time_ms,uint32_t vol)
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

void toneBlock_Volumn_us(uint8_t Pin,uint32_t freq,uint32_t Time_us,uint32_t vol)
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
