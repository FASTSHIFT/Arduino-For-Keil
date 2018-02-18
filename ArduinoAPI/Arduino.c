#include "Arduino.h"

void null_func(void){};

void pinMode(int pin,u8 GPIOMode)
{
	if(IS_GPIO_MODE(GPIOMode))
	{
		GPIO_begin((GPIO_TypeDef*)(pin&~0x0F),1<<(pin&0x0F),(GPIOMode_TypeDef)GPIOMode,GPIO_Speed_50MHz);
	}
	else if(GPIOMode == PWM)PWM_Init(pin);
	else return;
}

void digitalWrite(int pin,u8 val)
{
	if(val) GPIO_SetBits((GPIO_TypeDef*)(pin&~0x0F),1<<(pin&0x0F));
	else  GPIO_ResetBits((GPIO_TypeDef*)(pin&~0x0F),1<<(pin&0x0F));
	//GPIO_WriteBit((GPIO_TypeDef*)(pin&~0x0F),1<<(pin&0x0F), val);
}

uint8_t digitalRead(int pin)
{
	return (GPIO_ReadInputDataBit((GPIO_TypeDef*)(pin&~0x0F),1<<(pin&0x0F)));
}

u16 analogWrite(int pin,u16 val)
{
	return pwmWrite(pin,val);
}

u16 analogRead(int pin)
{
	return ADC_Pin_Read(pin);
}

void togglePin(int pin)
{
	GPIO_ToggleBits((GPIO_TypeDef*)(pin&~0x0F),1<<(pin&0x0F));
}

void shiftOut(int dataPin, int clockPin, u8 bitOrder, u8 value)
{
	int i;
	digitalWrite(clockPin, LOW);
	for (i = 0; i < 8; i++) 
	{
		int bit = bitOrder == LSBFIRST ? i : (7 - i);
		digitalWrite(dataPin, (value >> bit) & 0x1);
		togglePin(clockPin);
		togglePin(clockPin);  
	}
}

uint32_t shiftIn( int ulDataPin, int ulClockPin, uint32_t ulBitOrder )
{
  uint8_t value = 0 ;
  uint8_t i ;

  
  for ( i=0 ; i < 8 ; ++i )
  {
    digitalWrite( ulClockPin, HIGH ) ;

    if ( ulBitOrder == LSBFIRST )
    {
      value |= digitalRead( ulDataPin ) << i ;
    }
    else
    {
      value |= digitalRead( ulDataPin ) << (7 - i) ;
    }

    digitalWrite( ulClockPin, LOW ) ;
  }

  return value ;
}

void tone(int Pin,unsigned int freq,unsigned long time)
{
	unsigned long ring_time = millis() + time;
	unsigned int dlyus = 500000/freq;
	if(freq == 0 && time == 0)return;
	while(millis() <= ring_time)
	{
		digitalWrite(Pin,HIGH);
		delayMicroseconds(dlyus);
		digitalWrite(Pin,LOW);
		delayMicroseconds(dlyus);
	}
}

long Map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double fmap(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void limit(int *data, int MIN, int MAX)
{
  if (*data < MIN)*data = MIN;
  else if (*data > MAX)*data = MAX;
}

void flimit(double *data, double MIN, double MAX)
{
  if (*data < MIN)*data = MIN;
  else if (*data > MAX)*data = MAX;
}
