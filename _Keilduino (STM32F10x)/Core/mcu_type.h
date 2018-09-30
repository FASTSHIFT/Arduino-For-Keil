#ifndef __MCU_TYPE
#define __MCU_TYPE

//*************** BASIC ***************//
#include "stm32f10x.h"

#define __KEILDUINO__ 560
#define __STM32F1__
#define F_CPU 72000000U

//*************** GPIO ***************//
#include "stm32f10x_gpio.h"

#define digitalWrite_HIGH(Pin) (PIN_MAP[Pin].GPIOx->BSRR = PIN_MAP[Pin].GPIO_Pin_x)
#define digitalWrite_LOW(Pin)  (PIN_MAP[Pin].GPIOx->BRR  = PIN_MAP[Pin].GPIO_Pin_x)
#define digitalRead_FAST(Pin) ((PIN_MAP[Pin].GPIOx->IDR  & PIN_MAP[Pin].GPIO_Pin_x)!=0)
#define togglePin(Pin)         (PIN_MAP[Pin].GPIOx->ODR ^= PIN_MAP[Pin].GPIO_Pin_x)

#define GPIO_HIGH(GPIOx,GPIO_Pin_x)		(GPIOx->BSRR = GPIO_Pin_x)
#define GPIO_LOW(GPIOx,GPIO_Pin_x)   	(GPIOx->BRR  = GPIO_Pin_x)
#define GPIO_READ(GPIOx,GPIO_Pin_x)    ((GPIOx->IDR  & GPIO_Pin_x)!=0)
#define GPIO_TOGGLE(GPIOx,GPIO_Pin_x) 	(GPIOx->ODR ^= GPIO_Pin_x)

typedef enum
{
    INPUT = GPIO_Mode_IN_FLOATING,
    OUTPUT = GPIO_Mode_Out_PP,
    OUTPUT_OPEN_DRAIN = GPIO_Mode_Out_OD,
    INPUT_PULLUP = GPIO_Mode_IPU,
    INPUT_PULLDOWN = GPIO_Mode_IPD,
    INPUT_ANALOG = GPIO_Mode_AIN,
    PWM
} pinMode_Type;

//*************** USART ***************//
#include "stm32f10x_usart.h"
#define IS_USARTx_SendDone(USARTx)	(USARTx->SR & USART_SR_TC)

//*************** SPI ***************//
#include "stm32f10x_spi.h"
#define IS_SPIx_TxRxDone(SPIx,SPI_I2S_FLAG)	(SPIx->SR & SPI_I2S_FLAG)

#define SPIx_FastSendData(SPIx,Data)	(SPIx->DR = Data)
#define SPIx_FastRecvData(SPIx)			(SPIx->DR)

#endif
