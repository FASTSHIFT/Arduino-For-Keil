#include "Arduino.h"

void pinMode(int pin, GPIOMode_TypeDef GPIOMode)
{
	GPIO_begin((GPIO_TypeDef*)(pin&~0x0F),1<<(pin&0x0F),GPIOMode,GPIO_Speed_50MHz);
}

void digitalWrite(int pin,u8 val)
{
	if(val) GPIO_SetBits((GPIO_TypeDef*)(pin&~0x0F),1<<(pin&0x0F));
	else GPIO_ResetBits((GPIO_TypeDef*)(pin&~0x0F),1<<(pin&0x0F));
	//GPIO_WriteBit((GPIO_TypeDef*)(pin&~0x0F),1<<(pin&0x0F), val);
}

uint8_t digitalRead(int pin)
{
	return (GPIO_ReadInputDataBit((GPIO_TypeDef*)(pin&~0x0F),1<<(pin&0x0F)));
}

void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	GPIOx->ODR^=GPIO_Pin;
}

void shiftOut(int dataPin, int clockPin, u8 bitOrder, u8 value)
{
	int i;
	digitalWrite(clockPin, LOW);
	for (i = 0; i < 8; i++) 
	{
		int bit = bitOrder == LSBFIRST ? i : (7 - i);
		digitalWrite(dataPin, (value >> bit) & 0x1);
		GPIO_ToggleBits((GPIO_TypeDef*)(clockPin&~0x0F),1<<(clockPin&0x0F));// togglePin(clockPin);
		GPIO_ToggleBits((GPIO_TypeDef*)(clockPin&~0x0F),1<<(clockPin&0x0F));// togglePin(clockPin);  
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

//double fabs(double x)
//{
//	return((x)>0.0?(x):-(x));
//}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double fmap(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

long Limit(long data, long MIN, long MAX)
{
  if (data < MIN)return MIN;
  else if (data > MAX)return MAX;
  else return data;
}

double fLimit(double data, double MIN, double MAX)
{
  if (data < MIN)return MIN;
  else if (data > MAX)return MAX;
  else return data;
}
