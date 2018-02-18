#ifndef __ARDUINO_H
#define	__ARDUINO_H

#include "GPIO.h"
#include "pwm.h"
#include "ADC.h"
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

#define CHANGE EXTI_Trigger_Rising_Falling//1
#define FALLING EXTI_Trigger_Falling//2
#define RISING EXTI_Trigger_Rising//3

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

#define byte uint8_t
#define boolean bool

#define delay delay_ms
#define delayMicroseconds delay_us

#define interrupts() __set_PRIMASK(0)
#define noInterrupts() __set_PRIMASK(1)

#define HIGH 0x1
#define LOW 0x0

#define INPUT GPIO_Mode_IN_FLOATING
#define OUTPUT GPIO_Mode_Out_PP
#define OUTPUT_OPEN_DRAIN GPIO_Mode_Out_OD
#define INPUT_PULLUP GPIO_Mode_IPU
#define INPUT_PULLDOWN GPIO_Mode_IPD
#define INPUT_ANALOG GPIO_Mode_AIN
#define PWM 0x20

void pinMode(int pin,u8 GPIOMode);
void digitalWrite(int pin,u8 val);
uint8_t digitalRead(int pin);
u16 analogWrite(int pin,u16 val);
u16 analogRead(int pin);
void togglePin(int pin);
void shiftOut(int dataPin, int clockPin, u8 bitOrder, u8 value);
uint32_t shiftIn( int ulDataPin, int ulClockPin, uint32_t ulBitOrder );

void tone(int Pin,unsigned int freq,unsigned long time);
long Map(long x, long in_min, long in_max, long out_min, long out_max);
double fmap(double x, double in_min, double in_max, double out_min, double out_max);
void limit(int *data, int MIN, int MAX);
void flimit(double *data, double MIN, double MAX);

typedef void(*void_func_point)(void);
void null_func(void);

#endif
