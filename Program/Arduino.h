#ifndef __ARDUINO_H
#define	__ARDUINO_H
#include "GPIO.h"
#include "binary.h"
#include "delay.h"

#define ARDUINO 111

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
//#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define lowByte(w) ((u8) ((w) & 0xff))
#define highByte(w) ((u8) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

//double fabs(double x);
long map(long x, long in_min, long in_max, long out_min, long out_max);
double fmap(double x, double in_min, double in_max, double out_min, double out_max);
long Limit(long data, long MIN, long MAX);
double fLimit(double data, double MIN, double MAX);

void pinMode(int pin, GPIOMode_TypeDef GPIOMode);
void digitalWrite(int pin,u8 val);
uint8_t digitalRead(int pin);
void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void shiftOut(int dataPin, int clockPin, u8 bitOrder, u8 value);
uint32_t shiftIn( int ulDataPin, int ulClockPin, uint32_t ulBitOrder );

#define INPUT GPIO_Mode_IN_FLOATING
#define OUTPUT GPIO_Mode_Out_PP
#define OUTPUT_OPEN_DRAIN GPIO_Mode_Out_OD
#define INPUT_PULLUP GPIO_Mode_IPU
#define INPUT_PULLDOWM GPIO_Mode_IPD
#define INPUT_ANALOG GPIO_Mode_AIN

//#define digitalWrite GPIO_WriteBit
//#define digitalRead GPIO_ReadInputDataBit
#define HIGH 0x1
#define LOW 0x0

#define analogRead GetADC

#define byte u8
#define boolean bool

#define delay delay_ms
#define delayMicroseconds delay_us

#define Serial_begin uart_init
#define Serial_print printf

#define interrupts() __set_PRIMASK(0)
#define noInterrupts() __set_PRIMASK(1)

#endif
