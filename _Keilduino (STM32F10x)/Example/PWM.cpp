#include "FileGroup.h"

#define PWM_Pin PA0

void setup()
{
	PWM_Init(PWM_Pin,1000,10000);//PWM DutyCycle:0~1000 -> 0~100% , PWM Frequency: 10KHz
}

void loop()
{
	for(int i = 0; i <= 1000; i++)
	{
		pwmWrite(PWM_Pin,i);
		//or use analogWrite(Pin,DutyCycle);
		delay(1);
	}
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
