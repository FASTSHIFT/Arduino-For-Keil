#include "FileGroup.h"

#define LED_Pin PC13
#define PWM_Pin PA0
#define ADC_Pin PA0

void setup()
{
	Serial.begin(115200);
	Serial.printf("C++ Version: %d\r\n",__cplusplus);
	Serial.printf("Compiling Time: %s %s\r\n",__DATE__,__TIME__);
	pinMode(LED_Pin,OUTPUT);
	pinMode(PWM_Pin,PWM);
}

void loop()
{
	Serial.println("STM32 Serial Test!");
	delay(2000);
	
	Serial.println("Digital Write TEST:");
	for(int i=0;i<5;i++)
	{
		digitalWrite(LED_Pin,HIGH);
		delay(1000);
		digitalWrite(LED_Pin,LOW);
		delay(1000);
	}
	
	Serial.println("Digital Read TEST:");
	pinMode(LED_Pin,INPUT);
	uint8_t State = digitalRead(LED_Pin);
	Serial.printf("LED_Pin State is:%d\r\n",State);
	pinMode(LED_Pin,OUTPUT);
	
	Serial.println("Analog Write TEST:");
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<=1000;j++)
		{
			analogWrite(PWM_Pin,j);
			delay(1);
		}
		for(int j=1000;j>0;j--)
		{
			analogWrite(PWM_Pin,j);
			delay(1);
		}
	}
	
	Serial.println("ADC_Pin Analog Read TEST:");
	pinMode(ADC_Pin,INPUT_ANALOG);
	uint16_t ADC_Val = analogRead(ADC_Pin);
	Serial.printf("ADC_Pin ADC Val is:%d\r\n",ADC_Val);
	pinMode(PWM_Pin,PWM);
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
