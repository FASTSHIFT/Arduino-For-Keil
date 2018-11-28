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

//*****************MAIN******************//
int main(void)
{
	Delay_Init();
	ADCx_Init(ADC1);
//	ADCx_DMA_Init();
	setup();
	for(;;)loop();
}
