#include "Arduino.h"

void null_func(void){};

void pinMode(uint8_t Pin,uint8_t GPIOMode)
{
	if(IS_GPIO_MODE(GPIOMode))
	{
		GPIO_begin(PIN_MAP[Pin].GPIOx,PIN_MAP[Pin].GPIO_Pin_x,(GPIOMode_TypeDef)GPIOMode,GPIO_Speed_50MHz);
	}
	else if(GPIOMode == PWM)PWM_Init(Pin);
	else return;
}

void digitalWrite(uint8_t Pin,uint8_t BitVal)
{
  if(BitVal)
  {
    PIN_MAP[Pin].GPIOx->BSRR = PIN_MAP[Pin].GPIO_Pin_x;
  }
  else
  {
    PIN_MAP[Pin].GPIOx->BRR = PIN_MAP[Pin].GPIO_Pin_x;
  }
}

uint8_t digitalRead(uint8_t Pin)
{
	uint8_t bitstatus = 0x00;
  
  if ((PIN_MAP[Pin].GPIOx->IDR & PIN_MAP[Pin].GPIO_Pin_x) != (uint32_t)Bit_RESET)
  {
    bitstatus = (uint8_t)Bit_SET;
  }
  else
  {
    bitstatus = (uint8_t)Bit_RESET;
  }
  return bitstatus;
}

u16 analogWrite(uint8_t Pin,u16 val)
{
	return pwmWrite(Pin,val);
}

u16 analogRead(uint8_t Pin)
{
	if(IS_ADC_PIN(Pin))
	{
		return Get_ADC(PIN_MAP[Pin].ADC_Channel);
	}
	else return 0;
}

void togglePin(uint8_t Pin)
{
	PIN_MAP[Pin].GPIOx->ODR^=PIN_MAP[Pin].GPIO_Pin_x;
}

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t value)
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

uint32_t shiftIn(uint8_t ulDataPin, uint8_t ulClockPin, uint32_t ulBitOrder)
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

void tone(uint8_t Pin,unsigned int freq,unsigned long time)
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

long map(long x, long in_min, long in_max, long out_min, long out_max)
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
