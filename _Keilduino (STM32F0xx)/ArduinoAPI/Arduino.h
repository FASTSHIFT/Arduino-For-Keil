#ifndef __ARDUINO_H
#define	__ARDUINO_H

#ifdef __cplusplus
extern "C"{
#endif

#include "gpio.h"
//#include "exti.h"
#include "pwm.h"
#include "adc.h"
#include "binary.h"
#include "delay.h"

#define __STM32F0__
#define __KEILDUINO_VERSION__ 100
#define ARDUINO 111
#define F_CPU 48000000U
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

#define analogInPinToBit(P) (P)

#define digitalWrite_HIGH(Pin) (PIN_MAP[Pin].GPIOx->BSRR = PIN_MAP[Pin].GPIO_Pin_x)
#define digitalWrite_LOW(Pin)  (PIN_MAP[Pin].GPIOx->BRR  = PIN_MAP[Pin].GPIO_Pin_x)
#define togglePin(Pin)				 (PIN_MAP[Pin].GPIOx->ODR ^= PIN_MAP[Pin].GPIO_Pin_x)

#define boolean bool
typedef unsigned char byte;

typedef enum {LOW = 0, HIGH = !LOW} GPIO_State_Type;
typedef enum {Off = 0, On = !Off} _Switch_Type;
typedef enum {OFF = 0, ON = !OFF} _SWITCH_Type;

void pinMode(uint8_t Pin,pinMode_TypeDef pinMode_x);
void digitalWrite(uint8_t Pin,uint8_t val);
uint8_t digitalRead(uint8_t Pin);
uint16_t analogWrite(uint8_t Pin,uint16_t val);
uint16_t analogRead(uint8_t Pin);
uint16_t analogRead_DMA(uint8_t Pin);
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t value);
uint32_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint32_t bitOrder);
uint32_t pulseIn(uint32_t Pin, uint32_t State, uint32_t Timeout);

long map(long x, long in_min, long in_max, long out_min, long out_max);
double fmap(double x, double in_min, double in_max, double out_min, double out_max);

typedef void(*void_func_point)(void);
void null_func(void);

#ifdef __cplusplus
}// extern "C"
#endif

#endif
