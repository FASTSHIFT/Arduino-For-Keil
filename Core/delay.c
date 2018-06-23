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
	return System_ms*1000 + (72000 - SysTick->VAL) / 72;
}

void delay_ms(uint32_t ms)
{
	uint32_t start = System_ms;
	while((System_ms-start) <= ms);
}

void delay_us(uint32_t us)
{
	uint32_t start = micros();
	while((micros() - start) <= us);
}

/*********************************************END OF FILE**********************/
