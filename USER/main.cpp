#include "FileGroup.h"

#define SCL_Pin PB6
#define SDA_Pin PB7

HardwareSerial Serial(USART1);//TX-PA9 RX-PA10
HardwareSerial Serial2(USART2);//TX-PA2 RX-PA3
HardwareSerial Serial3(USART3);//TX-PB10 RX-PB11
HardwareSPI SPI(SPI1);//SCK-PB13 MISO-PB14 MOSI-PB15
HardwareSPI SPI_2(SPI2);//SCK-PA5 MISO-PA6 MOSI-PA7
TwoWire Wire(SCL_Pin, SDA_Pin, SOFT_FAST);

void setup()
{
	
}

void loop()
{
  
}

//*****************MAIN******************//
int main(void)
{
	delay_init();
	//ADCx_Init();
	//ADCx_DMA_Init();
	setup();
	for(;;)loop();
}
