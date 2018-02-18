#ifndef __USART_H
#define __USART_H	
#include "sys.h" 

#define SERIAL_RX_BUFFER_SIZE 128

typedef uint16_t rx_buffer_index_t;
extern volatile uint16_t _rx_buffer_head[3];
extern volatile uint16_t _rx_buffer_tail[3];		
extern u8 _rx_buffer[3][SERIAL_RX_BUFFER_SIZE];

void usart_init(USART_TypeDef * USARTx,u32 bound);
void usart_putc(USART_TypeDef* USARTx, u8 ch);
void usart_attachInterrupt(u8 USART_Num,void (*f)());
#endif

