#include "GPIO.h"
#include "delay.h"
#include "time_exit.h"   

int main(void)
{
	int i,j=0;
	
	GPIO_Config(); 	
	delay_init();
	TIM3_Int_Init(2000,7200);		//200ms定时
	
	TIM_Cmd(TIM3, DISABLE);  		//使能TIMx外设
	for(i=0;i<10;i++)
	{
		PEout(5)=~PEout(5);
		PEout(6)=~PEout(6);
		delay_ms(50);
	}
	
	delay_ms(100);
	PEout(5)=1;
	PEout(6)=0;
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
	
	while(1)
	{
		
	}
}
