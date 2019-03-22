#include "FileGroup.h"

void LED1()
{
	Serial.println("Timer 1 Interrupt!");
	togglePin(PA0);
}

void LED2()
{
	Serial.println("Timer 2 Interrupt!");
	togglePin(PA1);
}

void setup()
{
	Serial.begin(115200);
	pinMode(PA0,OUTPUT);
	pinMode(PA1,OUTPUT);
	TimerSet(TIM1,1000000,LED1);
	TimerSet(TIM2,100000,LED2);
	TIM_Cmd(TIM1,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}

void loop()
{
}

//*****************MAIN******************//
int main(void)
{
	Delay_Init();
	ADCx_Init(ADC1);
//	ADCx_DMA_Init();
	setup();
	for(;;)loop();
}
