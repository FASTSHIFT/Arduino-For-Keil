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

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#define __STM32F4__
#define F_CPU SystemCoreClock
#define CYCLES_PER_MICROSECOND (F_CPU / 1000000U)

#define __KEILDUINO__ 240

#define digitalWrite_HIGH(Pin) (PIN_MAP[Pin].GPIOx->BSRRL  = PIN_MAP[Pin].GPIO_Pin_x)
#define digitalWrite_LOW(Pin)  (PIN_MAP[Pin].GPIOx->BSRRH  = PIN_MAP[Pin].GPIO_Pin_x)
#define digitalRead_FAST(Pin) ((PIN_MAP[Pin].GPIOx->IDR    & PIN_MAP[Pin].GPIO_Pin_x)!=0)
#define togglePin(Pin)         (PIN_MAP[Pin].GPIOx->ODR   ^= PIN_MAP[Pin].GPIO_Pin_x)

#define GPIO_HIGH(GPIOX,GPIO_PIN_X)    (GPIOX->BSRRL = GPIO_PIN_X)
#define GPIO_LOW(GPIOX,GPIO_PIN_X)     (GPIOX->BSRRH = GPIO_PIN_X)
#define GPIO_READ(GPIOX,GPIO_PIN_X)   ((GPIOX->IDR   & GPIO_PIN_X)!=0)
#define GPIO_TOGGLE(GPIOX,GPIO_PIN_X)  (GPIOX->ODR  ^= GPIO_PIN_X)

//*************** USART ***************//
#define IS_USARTx_SendDone(USARTx) (USARTx->SR & USART_FLAG_TXE)

//*************** SPI ***************//
#define IS_SPIx_TxRxDone(SPIx,SPI_I2S_FLAG) (SPIx->SR & SPI_I2S_FLAG)
#define SPIx_FastSendData(SPIx,Data)        (SPIx->DR = Data)
#define SPIx_FastRecvData(SPIx)             (SPIx->DR)

#ifdef __cplusplus
}
#endif

#endif
