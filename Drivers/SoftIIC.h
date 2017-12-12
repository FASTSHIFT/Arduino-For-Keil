//By _VIFEXTech 2017.10.17

#ifndef SOFTIIC_H
#define SOFTIIC_H
extern"C"
{
#include "FileGroup.h"
}
#define IIC_DELAY 4

class SoftIIC
{
	public:
		SoftIIC(int _sda, int _scl);
		void begin();
		void endTransmission();
		unsigned char beginTransmission(unsigned char addr);
		unsigned char write(unsigned char dat);
		unsigned char read(unsigned char ack);


	private:
		int I2C_SDA;
		int I2C_SCL;
		void NACK();
		void ACK();
};

#endif
