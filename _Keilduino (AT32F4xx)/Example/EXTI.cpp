#include "FileGroup.h"

void LED()
{
	Serial.println("PA1 FALLING!");
	togglePin(PA0);
}

void setup()
{
	Serial.begin(115200);
	pinMode(PA0,OUTPUT);
	attachInterrupt(PA1,LED,FALLING);
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
