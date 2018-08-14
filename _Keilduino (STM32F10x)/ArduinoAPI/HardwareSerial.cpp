#include "HardwareSerial.h"

HardwareSerial::HardwareSerial(USART_TypeDef *_USARTx)
{
	USARTx = _USARTx;
	if(_USARTx==USART1)USART_Num=0;
	else if(_USARTx==USART2)USART_Num=1;
	else if(_USARTx==USART3)USART_Num=2;
}

void HardwareSerial::begin(uint32_t BaudRate)
{
	USARTx_Init(USARTx,BaudRate,1,3);
}

void HardwareSerial::end(void)
{
	USART_Cmd(USARTx, DISABLE); 
}

void HardwareSerial::attachInterrupt(void_func_point f)
{
	USARTx_attachInterrupt(USART_Num,f);
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
	USARTx_putc(USARTx,n);
	return n;
}

//Creat Object For User
HardwareSerial Serial(USART1);//TX-PA9 RX-PA10
HardwareSerial Serial2(USART2);//TX-PA2 RX-PA3
HardwareSerial Serial3(USART3);//TX-PB10 RX-PB11
