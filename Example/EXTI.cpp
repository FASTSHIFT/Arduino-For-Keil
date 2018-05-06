#include "FileGroup.h"

void LED()
{
	Serial.println("PA0 FALLING!");
	togglePin(PA0);
}

void setup()
{
	Serial.begin(115200);
	pinMode(PA0,OUTPUT);
	attachInterrupt(PA0,LED,FALLING);
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
