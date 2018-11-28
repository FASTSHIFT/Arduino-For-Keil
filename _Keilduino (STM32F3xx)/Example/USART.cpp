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

//*****************MAIN******************//
int main(void)
{
	Delay_Init();
	ADCx_Init(ADC1);
//	ADCx_DMA_Init();
	setup();
	for(;;)loop();
}
