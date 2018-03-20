<<<<<<< HEAD
#ifndef __USART_H
#define __USART_H

#ifdef __cplusplus
extern "C"{
#endif

#include "stm32f10x.h"
#include "Arduino.h"

#define SERIAL_RX_BUFFER_SIZE 128
#define SERIAL_NUM 3

typedef uint16_t rx_buffer_index_t;
extern volatile uint16_t _rx_buffer_head[SERIAL_NUM];
extern volatile uint16_t _rx_buffer_tail[SERIAL_NUM];		
extern uint8_t _rx_buffer[SERIAL_NUM][SERIAL_RX_BUFFER_SIZE];

void usart_init(USART_TypeDef* USARTx,uint32_t bound,uint8_t PreemptionPriority,uint8_t SubPriority);
void usart_putc(USART_TypeDef* USARTx,uint8_t ch);
void usart_attachInterrupt(uint8_t USART_Num,void_func_point f);
	
#ifdef __cplusplus
}
#endif

#endif

=======
#ifndef __USART_H
#define __USART_H	
#include "stm32f10x.h"

#define SERIAL_RX_BUFFER_SIZE 128

typedef uint16_t rx_buffer_index_t;
extern volatile uint16_t _rx_buffer_head[3];
extern volatile uint16_t _rx_buffer_tail[3];		
extern u8 _rx_buffer[3][SERIAL_RX_BUFFER_SIZE];

void usart_init(USART_TypeDef * USARTx,u32 bound);
void usart_putc(USART_TypeDef* USARTx, u8 ch);
void usart_attachInterrupt(u8 USART_Num,void (*f)());
#endif

>>>>>>> 301cbab54594d1de84f6a7e456407b3a84e8d255
