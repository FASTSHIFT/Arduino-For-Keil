/*
 * MIT License
 * Copyright (c) 2019 _VIFEXTech
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
#ifndef __MCU_TYPE
#define __MCU_TYPE

#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"

#define __KEILDUINO__ 230
#define __STM32F0__
#define F_CPU SystemCoreClock
#define CYCLES_PER_MICROSECOND  (F_CPU / 1000000U)

#define GPIO_HIGH(GPIOx,GPIO_Pin_x)     (GPIOx->BSRR = GPIO_Pin_x)
#define GPIO_LOW(GPIOx,GPIO_Pin_x)      (GPIOx->BRR  = GPIO_Pin_x)
#define GPIO_READ(GPIOx,GPIO_Pin_x)    ((GPIOx->IDR  & GPIO_Pin_x)!=0)
#define GPIO_TOGGLE(GPIOx,GPIO_Pin_x)   (GPIOx->ODR ^= GPIO_Pin_x)

#define digitalWrite_HIGH(Pin) (GPIO_HIGH(PIN_MAP[Pin].GPIOx,PIN_MAP[Pin].GPIO_Pin_x))
#define digitalWrite_LOW(Pin)  (GPIO_LOW(PIN_MAP[Pin].GPIOx,PIN_MAP[Pin].GPIO_Pin_x))
#define digitalRead_FAST(Pin)  (GPIO_READ(PIN_MAP[Pin].GPIOx,PIN_MAP[Pin].GPIO_Pin_x))
#define togglePin(Pin)         (GPIO_TOGGLE(PIN_MAP[Pin].GPIOx,PIN_MAP[Pin].GPIO_Pin_x))

#endif
