#include "delay.h"
#include "core_cm3.h"
#include "misc.h"

static __IO u32 SYS_MS=0,SYS_moom=0;

void delay_init(void)
{
	SysTick->LOAD=72000; 
	SysTick->VAL =0x00;           
	SysTick->CTRL|=0x04; 		//时钟9mhz
	SysTick->CTRL|=0x02 ; 	
	SysTick->CTRL|=0x01 ; 	
}


void SysTick_Handler(void)
{
	SYS_MS++;
}

u32 millis(void)
{
	if(SYS_MS>=0x9a7ec800)		//一个月时间计数
	{
		SYS_moom++;
		SYS_MS=0;
		return 0;
	}
	
	return SYS_MS;
}

u32 micros(void)
{
	return millis()*1000+SysTick->VAL;
}

void delay_ms(u32 ms)
{
	u32 start=millis();
	while((millis()-start)<=ms);
}

void delay_us(u32 us)
{
	u32 start=SysTick->VAL;
	if(us<1000)
	{
		while((((SysTick->VAL)-start)<us));
	}
	else
	{
		start=micros();
		while((millis()-start)<=us);
	}
}

/*********************************************END OF FILE**********************/
