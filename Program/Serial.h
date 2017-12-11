#ifndef Serial_h
#define Serial_h

extern "C"
{
	#include "usart.h"
}
#include "Print.h"

class Serial : public Print
{
	public:
		Serial(void);
		void begin(USART_TypeDef *USARTx,u32 baud);
		size_t write(uint8 ch);
};

#endif
