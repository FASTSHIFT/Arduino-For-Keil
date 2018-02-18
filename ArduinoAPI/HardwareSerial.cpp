#include "HardwareSerial.h"

USART_TypeDef *USARTx;
u8 USART_Num;

HardwareSerial::HardwareSerial(USART_TypeDef *_USARTx)
{
	USARTx = _USARTx;
	if(_USARTx==USART1)USART_Num=0;
	else if(_USARTx==USART2)USART_Num=1;
	else if(_USARTx==USART3)USART_Num=2;
}

void HardwareSerial::begin(u32 baud)
{
	usart_init(USARTx,baud);
}

void HardwareSerial::end(void)
{
	USART_Cmd(USARTx, DISABLE); 
}

void HardwareSerial::attachInterrupt(void (*f)())
{
	usart_attachInterrupt(USART_Num,f);
}

int HardwareSerial::available(void)
{
  return ((unsigned int)(SERIAL_RX_BUFFER_SIZE + _rx_buffer_head[USART_Num] - _rx_buffer_tail[USART_Num])) % SERIAL_RX_BUFFER_SIZE;
}

int HardwareSerial::read(void)
{
  // if the head isn't ahead of the tail, we don't have any characters
  if (_rx_buffer_head[USART_Num] == _rx_buffer_tail[USART_Num]) 
	{
    return -1;
  } 
	else 
	{
    unsigned char c = _rx_buffer[USART_Num][_rx_buffer_tail[USART_Num]];
    _rx_buffer_tail[USART_Num] = (rx_buffer_index_t)(_rx_buffer_tail[USART_Num] + 1) % SERIAL_RX_BUFFER_SIZE;
    return c;
  }
}

int HardwareSerial::peek(void)
{
  if (_rx_buffer_head[USART_Num] == _rx_buffer_tail[USART_Num])
	{
    return -1;
  } 
	else
	{
    return _rx_buffer[USART_Num][_rx_buffer_tail[USART_Num]];
  }
}

void HardwareSerial::flush(void)
{
	while(read()>=0);
}

size_t HardwareSerial::write(uint8_t n)
{
	usart_putc(USARTx,n);
	return n;
}
