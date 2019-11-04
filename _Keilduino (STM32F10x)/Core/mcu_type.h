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

//*************** BASIC ***************//
#include "stm32f10x.h"
#include "stm32f10x_conf.h"

typedef enum{
    F_CPU_16MHz,
    F_CPU_24MHz,
    F_CPU_32MHz,
    F_CPU_40MHz,
    F_CPU_48MHz,
    F_CPU_56MHz,
    F_CPU_64MHz,
    F_CPU_72MHz,
    F_CPU_80MHz,
    F_CPU_88MHz,
    F_CPU_96MHz,
    F_CPU_104MHz,
    F_CPU_112MHz,
    F_CPU_120MHz,
    F_CPU_128MHz,
}F_CPU_Type;

typedef struct{
    uint32_t F_CPU_x;
    uint32_t RCC_PLLMul_x;
}SysClock_TypeDef;

#define __KEILDUINO__ 710
#define __STM32F1__
#define F_CPU SystemCoreClock
#define CYCLES_PER_MICROSECOND (F_CPU / 1000000U)

//*************** GPIO ***************//
#define digitalWrite_HIGH(Pin) (PIN_MAP[Pin].GPIOx->BSRR = PIN_MAP[Pin].GPIO_Pin_x)
#define digitalWrite_LOW(Pin)  (PIN_MAP[Pin].GPIOx->BRR  = PIN_MAP[Pin].GPIO_Pin_x)
#define digitalRead_FAST(Pin) ((PIN_MAP[Pin].GPIOx->IDR  & PIN_MAP[Pin].GPIO_Pin_x)!=0)
#define togglePin(Pin)         (PIN_MAP[Pin].GPIOx->ODR ^= PIN_MAP[Pin].GPIO_Pin_x)

#define GPIO_HIGH(GPIOx,GPIO_Pin_x)     (GPIOx->BSRR = GPIO_Pin_x)
#define GPIO_LOW(GPIOx,GPIO_Pin_x)      (GPIOx->BRR  = GPIO_Pin_x)
#define GPIO_READ(GPIOx,GPIO_Pin_x)    ((GPIOx->IDR  & GPIO_Pin_x)!=0)
#define GPIO_TOGGLE(GPIOx,GPIO_Pin_x)   (GPIOx->ODR ^= GPIO_Pin_x)

typedef enum
{
    INPUT = GPIO_Mode_IN_FLOATING,
    OUTPUT = GPIO_Mode_Out_PP,
    OUTPUT_OPEN_DRAIN = GPIO_Mode_Out_OD,
    INPUT_PULLUP = GPIO_Mode_IPU,
    INPUT_PULLDOWN = GPIO_Mode_IPD,
    INPUT_ANALOG = GPIO_Mode_AIN,
    INPUT_ANALOG_DMA,
    PWM
} pinMode_Type;

//*************** USART ***************//
#define IS_USARTx_SendDone(USARTx) (USARTx->SR & USART_SR_TXE)

//*************** SPI ***************//
#define IS_SPIx_TxRxDone(SPIx,SPI_I2S_FLAG) (SPIx->SR & SPI_I2S_FLAG)

#define SPIx_FastSendData(SPIx,Data)    (SPIx->DR = Data)
#define SPIx_FastRecvData(SPIx)         (SPIx->DR)

#endif
