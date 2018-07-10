#include "Arduino.h"

void null_func(void){};

void pinMode(uint8_t Pin,uint8_t GPIO_Mode_x)
{
	if(IS_GPIO_MODE(GPIO_Mode_x))
	{
		GPIO_begin(PIN_MAP[Pin].GPIOx,PIN_MAP[Pin].GPIO_Pin_x,(GPIOMode_TypeDef)GPIO_Mode_x,GPIO_Speed_50MHz);
	}
	else if(GPIO_Mode_x == PWM)PWM_Init(Pin,1000,20000);
	else return;
}

void digitalWrite(uint8_t Pin,uint8_t val)
{
  if(val)
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
	return !!(PIN_MAP[Pin].GPIOx->IDR & PIN_MAP[Pin].GPIO_Pin_x);
}

uint16_t analogWrite(uint8_t Pin,uint16_t val)
{
	if(IS_PWM_PIN(Pin))
	{
		return pwmWrite(Pin,val);
	}
	else return 0;
}

uint16_t analogRead(uint8_t Pin)
{
	if(IS_ADC_PIN(Pin))
	{
		return Get_ADC(PIN_MAP[Pin].ADCx, PIN_MAP[Pin].ADC_Channel);
	}
	else return 0;
}

uint16_t analogRead_DMA(uint8_t Pin)
{
	if(IS_ADC_PIN(Pin))
	{
		return Get_DMA_ADC(PIN_MAP[Pin].ADC_Channel);
	}
	else return 0;
}

void togglePin(uint8_t Pin)
{
	PIN_MAP[Pin].GPIOx->ODR^=PIN_MAP[Pin].GPIO_Pin_x;
}

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t value)
{
	uint8_t i;
	digitalWrite_LOW(clockPin);
	for (i = 0; i < 8; i++) 
	{
		int bit = bitOrder == LSBFIRST ? i : (7 - i);
		digitalWrite(dataPin, (value >> bit) & 0x1);
		togglePin(clockPin);
		togglePin(clockPin);  
	}
}

uint32_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint32_t bitOrder)
{
  uint8_t value = 0 ;
  uint8_t i ;
  for ( i=0 ; i < 8 ; ++i )
  {
    digitalWrite_HIGH(clockPin) ;
    if (bitOrder == LSBFIRST )
    {
      value |= digitalRead(dataPin) << i ;
    }
    else
    {
      value |= digitalRead(dataPin) << (7 - i) ;
    }
    digitalWrite_LOW(clockPin);
  }

  return value ;
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double fmap(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint32_t pulseIn(uint32_t Pin, uint32_t State, uint32_t Timeout)
{
	uint8_t bit = digitalRead(Pin);
	uint8_t stateMask = (State ? bit : 0);

	unsigned long start;
	unsigned long startMicros = micros();

	// wait for any previous pulse to end
	while ((digitalRead(Pin) & bit) == stateMask)
	{
		if (micros() - startMicros > Timeout)
			return 0;
	}

	// wait for the pulse to start
	while ((digitalRead(Pin) & bit) != stateMask)
	{
		if (micros() - startMicros > Timeout)
			return 0;
	}

	start = micros();
	// wait for the pulse to stop
	while ((digitalRead(Pin) & bit) == stateMask)
	{
		if (micros() - startMicros > Timeout)
			return 0;
	}
	return micros() - start;
}
