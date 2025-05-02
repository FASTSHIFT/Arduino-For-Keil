/*
 * MIT License
 * Copyright (c) 2017 - 2025 _VIFEXTech
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
#ifndef TONE_H
#define TONE_H

/*********************
 *      INCLUDES
 *********************/

#include "timer.h"

/*********************
 *      DEFINES
 *********************/

#define TONE_DURATION_INFINITE  0xFFFFFFFFU

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * @brief  Generate a square wave on the specified pin at the specified frequency.
 * @param  pin       The pin on which to generate the square wave.
 * @param  freq      The frequency of the square wave in Hz.
 * @param  duration  The duration of the tone in milliseconds.
 * @note   If duration is set to TONE_DURATION_INFINITE, the tone will continue until noTone() is called.
 */
void tone(uint8_t pin, uint32_t freq, uint32_t duration = TONE_DURATION_INFINITE);

/**
 * @brief  Stop generating the tone on the specified pin.
 * @param  pin  The pin on which the tone is being generated.
 */
void noTone(uint8_t pin);

/**
 * @brief  Set the timer used for generating tones.
 * @param  timer  The timer to be used for tone generation.
 * @note   This function should be called before using the tone() function.
 */
void toneSetTimer(TIM_TypeDef* timer);

#endif
