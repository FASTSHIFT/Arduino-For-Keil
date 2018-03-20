#include "FileGroup.h"

void setup()
{

}

void loop()
{
	
}

//*****************MAIN******************//
int main(void)
{
	Delay_Init();
	ADCx_Init();
//	ADCx_DMA_Init();
	setup();
	for(;;)loop();
}
