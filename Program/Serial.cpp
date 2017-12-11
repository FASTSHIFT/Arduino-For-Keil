
#include "Serial.h"
	
Serial::Serial()
{
	
}
void Serial::begin(USART_TypeDef *USARTx,u32 baud)
{
	uart_init(USARTx,baud);
}

size_t Serial::write(uint8 ch)
{
	fputc(ch, (FILE*)0);
}

