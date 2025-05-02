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

/*********************
 *      INCLUDES
 *********************/

#include "Arduino.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void pinMode(uint8_t pin, PinMode_TypeDef mode)
{
    if (!IS_PIN(pin)) {
        return;
    }

    switch (mode) {
    case INPUT_ANALOG_DMA:
        if (!IS_ADC_PIN(pin)) {
            return;
        }
        pinMode(pin, INPUT_ANALOG);
        ADC_DMA_Register(PIN_MAP[pin].ADC_Channel);
        break;

    case PWM:
        PWM_Init(pin, PWM_RESOLUTION_DEFAULT, PWM_FREQUENCY_DEFAULT);
        break;

    default:
        GPIOx_Init(
            PIN_MAP[pin].GPIOx,
            PIN_MAP[pin].GPIO_Pin_x,
            mode,
            GPIO_DRIVE_DEFAULT);
        break;
    }
}

void digitalWrite(uint8_t pin, uint8_t value)
{
    if (!IS_PIN(pin)) {
        return;
    }

    value ? digitalWrite_HIGH(pin) : digitalWrite_LOW(pin);
}

uint8_t digitalRead(uint8_t pin)
{
    if (!IS_PIN(pin)) {
        return 0;
    }

    return digitalRead_FAST(pin);
}

void analogWrite(uint8_t pin, uint16_t value)
{
    if (!IS_PWM_PIN(pin)) {
        return;
    }

    PWM_Write(pin, value);
}

uint16_t analogRead(uint8_t pin)
{
    if (!IS_ADC_PIN(pin)) {
        return 0;
    }

    return ADCx_GetValue(PIN_MAP[pin].ADCx, PIN_MAP[pin].ADC_Channel);
}

uint16_t analogRead_DMA(uint8_t pin)
{
    if (!IS_ADC_PIN(pin)) {
        return 0;
    }

    return ADC_DMA_GetValue(PIN_MAP[pin].ADC_Channel);
}

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t value)
{
    int i;
    if (!(IS_PIN(dataPin) && IS_PIN(clockPin))) {
        return;
    }

    digitalWrite_LOW(clockPin);
    for (i = 0; i < 8; i++) {
        int bit = bitOrder == LSBFIRST ? i : (7 - i);
        digitalWrite(dataPin, (value >> bit) & 0x1);
        togglePin(clockPin);
        togglePin(clockPin);
    }
}

uint32_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint32_t bitOrder)
{
    uint8_t value = 0;
    int i;

    if (!(IS_PIN(dataPin) && IS_PIN(clockPin))) {
        return 0;
    }

    for (i = 0; i < 8; ++i) {
        digitalWrite_HIGH(clockPin);
        if (bitOrder == LSBFIRST) {
            value |= digitalRead(dataPin) << i;
        } else {
            value |= digitalRead(dataPin) << (7 - i);
        }
        digitalWrite_LOW(clockPin);
    }

    return value;
}

uint32_t pulseIn(uint32_t pin, uint32_t state, uint32_t timeout)
{
    /** cache the IDR address and bit of the pin in order to speed up the
     * pulse width measuring loop and achieve finer resolution.  calling
     * digitalRead() instead yields much coarser resolution.
     */
#ifdef GPIO_HAVE_PORT_REGISTER_TYPE
    PORT_INPUT_REGISTER_TYPE* idr;
#else
    volatile uint32_t* idr;
#endif
    idr = portInputRegister(digitalPinToPort(pin));
    const uint32_t bit = digitalPinToBitMask(pin);
    const uint32_t stateMask = (state ? bit : 0);

    uint32_t width = 0; // keep initialization out of time critical area

    /** convert the timeout from microseconds to a number of times through
     * the initial loop; it takes 16 clock cycles per iteration.
     */
    uint32_t numloops = 0;
    uint32_t maxloops = timeout * (F_CPU / 16000000);
    volatile uint32_t dummyWidth = 0;

    if (!IS_PIN(pin))
        return 0;

    /* wait for any previous pulse to end */
    while ((*idr & bit) == stateMask) {
        if (numloops++ == maxloops) {
            return 0;
        }
        dummyWidth++;
    }

    /* wait for the pulse to start */
    while ((*idr & bit) != stateMask) {
        if (numloops++ == maxloops) {
            return 0;
        }
        dummyWidth++;
    }

    /* wait for the pulse to stop */
    while ((*idr & bit) == stateMask) {
        if (numloops++ == maxloops) {
            return 0;
        }
        width++;
    }

    /** Excluding time taking up by the interrupts, it needs 16 clock cycles to look through the last while loop
     * 5 is added as a fiddle factor to correct for interrupts etc. But ultimately this would only be accurate if it was done ona hardware timer
     */
    return (uint32_t)(((unsigned long long)(width + 5) * (unsigned long long)16000000.0) / (unsigned long long)F_CPU);
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void yield(void)
{
    __WFI();
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
