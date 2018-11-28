#include "FileGroup.h"

void setup()
{
	Serial.begin(115200);
}

void loop()
{
	int ADC_Val = analogRead_DMA(PA0);
	Serial.printf("PA0 ADC:%d\r\n",ADC_Val);
}

//*****************MAIN******************//
int main(void)
{
	Delay_Init();
//	ADCx_Init(ADC1);
	ADCx_DMA_Init();
	setup();
	for(;;)loop();
}
