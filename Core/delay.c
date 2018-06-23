#include "delay.h"

volatile uint32_t System_ms=0;

void Delay_Init(void)
{
	SysTick->LOAD=72000; 
	SysTick->VAL =0x00;           
	SysTick->CTRL|=0x04;
	SysTick->CTRL|=0x02; 	
	SysTick->CTRL|=0x01; 	
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
	return System_ms*1000 + (72000U - SysTick->VAL) / 72;
}

void delay_ms(uint32_t ms)
{
	uint32_t start = System_ms;
	while((System_ms-start) <= ms);
}

void delay_us(uint32_t us)
{
	uint32_t total = 0;
	uint32_t target = 72 * us;
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
		total += diff + 72000;
	}
	if(total > target)
	{
		return;
	}
	last = now;
	goto start;
}
