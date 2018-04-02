#include "FileGroup.h"

void setup()
{
	Serial.begin(115200);
	Serial.printf("Compiling Time: %s %s\r\n",__DATE__,__TIME__);
	pinMode(PC13,OUTPUT);
	pinMode(PA0,PWM);
}

void loop()
{
	Serial.println("STM32 Serial Test!");
	delay(2000);
	
	Serial.println("Digital Write TEST:");
	for(int i=0;i<5;i++)
	{
		digitalWrite(PC13,HIGH);
		delay(1000);
		digitalWrite(PC13,LOW);
		delay(1000);
	}
	
	Serial.println("Digital Read TEST:");
	pinMode(PC13,INPUT);
	uint8_t State = digitalRead(PC13);
	Serial.printf("PC13 State is:%d\r\n",State);
	pinMode(PC13,OUTPUT);
	
	Serial.println("Analog Write TEST:");
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<=1000;j++)
		{
			analogWrite(PA0,j);
			delay(10);
		}
		for(int j=1000;j>0;j--)
		{
			analogWrite(PA0,j);
			delay(10);
		}
	}
	
	Serial.println("PA0 Analog Read TEST:");
	pinMode(PA0,INPUT_ANALOG);
	uint16_t ADC_Val = analogRead(PA0);
	Serial.printf("PA0 ADC Val is:%d\r\n",ADC_Val);
	pinMode(PA0,PWM);
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
