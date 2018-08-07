#include "delay.h"
#include "Arduino.h"

#define SysTick_LoadValue (F_CPU / 1000U)

volatile uint32_t System_ms=0;

void Delay_Init(void)
{
//	SysTick->LOAD = SysTick_LoadValue; 
//	SysTick->VAL  = 0x00;           
//	SysTick->CTRL|= 0x04;
//	SysTick->CTRL|= 0x02; 	
//	SysTick->CTRL|= 0x01;
	SysTick_Config(SysTick_LoadValue);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
}


void SysTick_Handler(void)
{
	System_ms++;
}

uint32_t millis(void)
{	
	return System_ms;
}

uint32_t micros(void)
{
	return (System_ms*1000 + (SysTick_LoadValue - SysTick->VAL) / CYCLES_PER_MICROSECOND);
}

void delay_ms(uint32_t ms)
{
	uint32_t Stop_TimePoint = System_ms + ms;
	while(System_ms < Stop_TimePoint);
}

void delay_us(uint32_t us)
{
	uint32_t total = 0;
	uint32_t target = CYCLES_PER_MICROSECOND * us;
	int last = SysTick->VAL;
	int now = last;
	int diff = 0;
start:
	now = SysTick->VAL;
	diff = last - now;
	if(diff > 0)
	{
		total += diff;
	}
	else
	{
		total += diff + SysTick_LoadValue;
	}
	if(total > target)
	{
		return;
	}
	last = now;
	goto start;
}
