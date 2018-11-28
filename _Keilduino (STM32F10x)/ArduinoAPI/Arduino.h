#ifndef __ARDUINO_H
#define	__ARDUINO_H

//访问 https://www.arduino.cc/reference/en/ 获得更完整的语法介绍

#ifdef __cplusplus
extern "C" {
#endif

#include "stdlib.h"
#include "stdbool.h"
#include "binary.h"
#include "avr/pgmspace.h"

#include "mcu_type.h"	
#include "gpio.h"
#include "pwm.h"
#include "adc.h"
#include "exti.h"
#include "delay.h"

#define __STM32__
#define ARDUINO 111
#define CYCLES_PER_MICROSECOND	(F_CPU / 1000000U)

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0x0
#define MSBFIRST 0x1

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define ABS(x) (((x)>0)?(x):-(x))//abs(x) is define in stdlib.h
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#define delay delay_ms
#define delayMicroseconds delay_us

#define interrupts() __set_PRIMASK(0)
#define noInterrupts() __set_PRIMASK(1)
#define sei() __set_PRIMASK(0)
#define cli() __set_PRIMASK(1)

#define analogInPinToBit(Pin)	(Pin)
#define digitalPinToPort(Pin)	(PIN_MAP[Pin].GPIOx)
#define digitalPinToBitMask(Pin) (PIN_MAP[Pin].GPIO_Pin_x)
#define portInputRegister(Port)  (&(Port->IDR))
#define portOutputRegister(Port) (&(Port->ODR))

#define NOT_A_PIN 0
#define NOT_A_PORT 0
#define NOT_AN_INTERRUPT -1

#define boolean bool
typedef unsigned char byte;
typedef void(*CallbackFunction_t)(void);

typedef enum {LOW = 0, HIGH = !LOW} GPIO_State_Type;
typedef enum {Off = 0, On = !Off} _Switch_Type;
typedef enum {OFF = 0, ON = !OFF} _SWITCH_Type;

void pinMode(uint8_t Pin, uint8_t GPIO_Mode_x);
void digitalWrite(uint8_t Pin, uint8_t val);
uint8_t digitalRead(uint8_t Pin);
uint16_t analogWrite(uint8_t Pin, uint16_t val);
uint16_t analogRead(uint8_t Pin);
uint16_t analogRead_DMA(uint8_t Pin);
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t value);
uint32_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint32_t bitOrder);
uint32_t pulseIn(uint32_t Pin, uint32_t State, uint32_t Timeout);

long map(long x, long in_min, long in_max, long out_min, long out_max);
double fmap(double x, double in_min, double in_max, double out_min, double out_max);

#ifdef __cplusplus
}// extern "C"
#endif

#ifdef __cplusplus
	#include "WCharacter.h"
//	#include "WString.h"
//	#include "HardwareSerial.h"
#endif

#endif
