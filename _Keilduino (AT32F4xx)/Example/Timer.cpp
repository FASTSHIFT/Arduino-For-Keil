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

/**
  * @brief  Main Function
  * @param  None
  * @retval None
  */
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	GPIO_JTAG_Disable();
//  SysClock_Init(F_CPU_128MHz);
	Delay_Init();
	ADCx_Init(ADC1);
	setup();
	for(;;)loop();
}
