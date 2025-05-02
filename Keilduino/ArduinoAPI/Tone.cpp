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

/*********************
 *      INCLUDES
 *********************/

#include "Tone.h"
#include "Arduino.h"

/*********************
 *      DEFINES
 *********************/

#define TONE_FREQ_MAX 500000U

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    TIM_TypeDef* timer;
    uint32_t toggleCount;
    bool isContinuous;
    uint8_t pin;
} ToneContext_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void toneTimerHandler();

/**********************
 *  STATIC VARIABLES
 **********************/

static ToneContext_t ToneContext = { NULL, 0, false, NOT_A_PIN };

/**********************
 *      MACROS
 **********************/

void tone(uint8_t pin, uint32_t freq, uint32_t duration)
{
    if (!IS_PIN(pin)) {
        return;
    }

    noTone(pin);

    if (duration == 0 || freq == 0 || freq > TONE_FREQ_MAX) {
        return;
    }

    ToneContext.pin = pin;
    ToneContext.isContinuous = (duration == TONE_DURATION_INFINITE) ? true : false;

    if (!ToneContext.isContinuous) {
        ToneContext.toggleCount = freq * duration / 1000 * 2;

        if (ToneContext.toggleCount == 0) {
            return;
        }

        ToneContext.toggleCount--;
    }

    if (ToneContext.timer == NULL) {
        toneSetTimer(TONE_TIMER_DEFAULT);
    }

    Timer_SetInterruptTimeUpdate(ToneContext.timer, TONE_FREQ_MAX / freq);
    Timer_SetEnable(ToneContext.timer, true);
}

void noTone(uint8_t pin)
{
    if (ToneContext.timer) {
        Timer_SetEnable(ToneContext.timer, false);
    }

    digitalWrite(pin, LOW);
    ToneContext.isContinuous = false;
    ToneContext.pin = NOT_A_PIN;
    ToneContext.toggleCount = 0;
}

void toneSetTimer(TIM_TypeDef* timer)
{
    Timer_SetInterruptBase(
        timer,
        0xFF,
        0xFF,
        toneTimerHandler,
        TONE_PREEMPTIONPRIORITY_DEFAULT,
        TONE_SUBPRIORITY_DEFAULT);
    ToneContext.timer = timer;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void toneTimerHandler()
{
    if (!ToneContext.isContinuous) {
        if (ToneContext.toggleCount == 0) {
            noTone(ToneContext.pin);
            return;
        }

        ToneContext.toggleCount--;
    }

    togglePin(ToneContext.pin);
}
