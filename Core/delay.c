<<<<<<< HEAD
#include "delay.h"

static uint32_t System_ms=0;

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
=======
#include "delay.h"
#include "core_cm3.h"
#include "misc.h"

static __IO u32 SYS_MS=0,SYS_moom=0;

void delay_init(void)
{
	SysTick->LOAD=72000; 
	SysTick->VAL =0x00;           
	SysTick->CTRL|=0x04; 		//时钟9mhz
	SysTick->CTRL|=0x02; 	
	SysTick->CTRL|=0x01; 	
}


void SysTick_Handler(void)
{
	SYS_MS++;
}

u32 millis(void)
{
//	if(SYS_MS>=0x9a7ec800)		//一个月时间计数
//	{
//		SYS_moom++;
//		SYS_MS=0;
//		return 0;
//	}
	
	return SYS_MS;
}

u32 micros(void)
{
	return SYS_MS*1000+(72000-SysTick->VAL)/72;
}

void delay_ms(u32 ms)
{
	u32 start=SYS_MS;
	while((SYS_MS-start)<=ms);
}

void delay_us(u32 us)
{
	u32 start=micros();
	while((micros()-start)<=us);
	//u32 n;
	//while(us--)for(n=0;n<9;n++);
}

/*********************************************END OF FILE**********************/
>>>>>>> 301cbab54594d1de84f6a7e456407b3a84e8d255
