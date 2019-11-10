#include "FileGroup.h"

void setup()
{
	Serial.begin(115200);
	Serial2.begin(115200);
	Serial3.begin(115200);
}

void loop()
{
	Serial.println("USART1 Print...");
	Serial2.println("USART2 Print...");
	Serial3.println("USART3 Print...");
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
