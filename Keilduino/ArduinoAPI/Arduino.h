/*
 * MIT License
 * Copyright (c) 2017 - 2024 _VIFEXTech
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef ARDUINO_H
#define ARDUINO_H

/*********************
 *      INCLUDES
 *********************/

#include "avr/pgmspace.h"
#include "binary.h"
#include <mcu_core.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
#include "WCharacter.h"
#include "WMath.h"
#include "WString.h"
#include <HardwareSerial.h>
#include <Tone.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      DEFINES
 *********************/

#define PI 3.1415926535897932384626433832795f
#define HALF_PI 1.5707963267948966192313216916398f
#define TWO_PI 6.283185307179586476925286766559f
#define DEG_TO_RAD 0.017453292519943295769236907684886f
#define RAD_TO_DEG 57.295779513082320876798154814105f
#define EULER 2.718281828459045235360287471352f

#define SERIAL 0x0
#define DISPLAY 0x1

#define LSBFIRST 0x0
#define MSBFIRST 0x1

#define LOW 0x0
#define HIGH 0x1

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b)) // min() is define in math.h
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b)) // max() is define in math.h
#endif

#ifndef ABS
#define ABS(x) (((x) > 0) ? (x) : -(x)) // abs(x) is define in stdlib.h
#endif

#ifndef ROUND
#define ROUND(x) ((x) >= 0 ? (long)((x) + 0.5) : (long)((x) - 0.5)) // round() is define in math.h
#endif

#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

#define radians(deg) ((deg) * DEG_TO_RAD)
#define degrees(rad) ((rad) * RAD_TO_DEG)
#define sq(x) ((x) * (x))

#define lowByte(w) ((uint8_t)((w) & 0xff))
#define highByte(w) ((uint8_t)((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

#define clockCyclesPerMicrosecond() (F_CPU / 1000000L)
#define clockCyclesToMicroseconds(a) (((a) * 1000L) / (F_CPU / 1000L))
#define microsecondsToClockCycles(a) ((a) * (F_CPU / 1000000L))

#define delay(ms) delay_ms(ms)
#define delayMicroseconds(us) delay_us(us)

#define interrupts() sei()
#define noInterrupts() cli()

#define NOT_A_PIN 0xFF
#define NOT_A_PORT 0xFF
#define NOT_AN_INTERRUPT -1

#define boolean bool

/**********************
 *      TYPEDEFS
 **********************/

typedef unsigned char byte;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * @brief Set the pin mode of the specified pin.
 * @param pin The pin number.
 * @param mode The pin mode.
 */
void pinMode(uint8_t pin, PinMode_TypeDef mode);

/**
 * @brief Write a digital value to the specified pin.
 * @param pin The pin number.
 * @param value The digital value to write.
 */
void digitalWrite(uint8_t pin, uint8_t value);

/**
 * @brief Read a digital value from the specified pin.
 * @param pin The pin number.
 * @return The digital value read.
 */
uint8_t digitalRead(uint8_t pin);

/**
 * @brief Write a PWM value to the specified pin.
 * @param pin The pin number.
 * @param value The PWM value to write.
 */
void analogWrite(uint8_t pin, uint16_t value);

/**
 * @brief Read an analog value from the specified pin.
 * @param pin The pin number.
 * @return The analog value read.
 */
uint16_t analogRead(uint8_t pin);

/**
 * @brief Read an analog value from the specified pin using DMA.
 * @param pin The pin number.
 * @return The analog value read.
 */
uint16_t analogRead_DMA(uint8_t pin);

/**
 * @brief Shift out a byte of data one bit at a time.
 * @param dataPin The pin to output the data.
 * @param clockPin The pin to output the clock.
 * @param bitOrder The order in which to shift out the bits.
 * @param value The byte of data to shift out.
 */
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t value);

/**
 * @brief Shift in a byte of data one bit at a time.
 * @param dataPin The pin to input the data.
 * @param clockPin The pin to input the clock.
 * @param bitOrder The order in which to shift in the bits.
 * @return The byte of data shifted in.
 */
uint32_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint32_t bitOrder);

/**
 * @brief Read a pulse width from the specified pin.
 * @param pin The pin number.
 * @param state The state of the pin to measure the pulse width.
 * @param timeout The maximum time to wait for the pulse to start.
 * @return The pulse width in microseconds.
 */
uint32_t pulseIn(uint32_t pin, uint32_t state, uint32_t timeout);

/**
 * @brief Map a value from one range to another.
 * @param x The value to map.
 * @param in_min The minimum value of the input range.
 * @param in_max The maximum value of the input range.
 * @param out_min The minimum value of the output range.
 * @param out_max The maximum value of the output range.
 * @return The mapped value.
 */
long map(long x, long in_min, long in_max, long out_min, long out_max);

/**
 * @brief Map a value from one range to another using a floating-point input.
 * @param x The value to map.
 * @param in_min The minimum value of the input range.
 * @param in_max The maximum value of the input range.
 * @param out_min The minimum value of the output range.
 * @param out_max The maximum value of the output range.
 * @return The mapped value.
 */
float fmap(float x, float in_min, float in_max, float out_min, float out_max);

/**
 * @brief Pause the program waiting for an interrupt.
 */
void yield(void);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* ARDUINO_H */
