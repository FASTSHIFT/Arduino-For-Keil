#include "FileGroup.h"

void setup(){
	// put your setup code here, to run once:
}

void loop(){
	// put your setup code here, to run once:
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
