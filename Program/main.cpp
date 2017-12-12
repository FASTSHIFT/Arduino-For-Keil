extern "C"{
#include "FileGroup.h"
}
#include "Serial.h"
#include "L3G.h"
#include "Wire.h"

TwoWire Wire(PB6, PB7, SOFT_STANDARD);//SCL SDA
Serial Serial;

L3G gyro;

void setup(void) 
{
	Serial.begin(USART1,115200);
	Wire.begin();
	
	if (!gyro.init())
  {
    Serial.println("Failed to autodetect gyro type!");
    while (1);
  }

  gyro.enableDefault();
}

void loop(void)
{
	gyro.read();
	printf("X: %d  Y: %d  Z: %d\r\n",(int)gyro.g.x,(int)gyro.g.y,(int)gyro.g.z);
	delay(100);
}

//*****************MAIN******************//
int main(void)
{
	delay_init();
	//ADCx_Init();
	setup();
	for(;;)loop();
}
