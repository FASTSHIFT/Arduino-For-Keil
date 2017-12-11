#include "SoftIIC.h"

int I2C_SDA;
int I2C_SCL;

SoftIIC::SoftIIC(int _sda, int _scl)
{
  I2C_SDA=_sda;
  I2C_SCL=_scl;
	pinMode(I2C_SDA,OUTPUT);
	pinMode(I2C_SCL,OUTPUT);
}

void SoftIIC::begin()
{	
  digitalWrite(I2C_SDA, 1);
  digitalWrite(I2C_SCL, 1);
  delayMicroseconds(IIC_DELAY);
  digitalWrite(I2C_SDA, 0);
  delayMicroseconds(IIC_DELAY);
  digitalWrite(I2C_SCL, 0);
}

void SoftIIC::endTransmission()
{
  digitalWrite(I2C_SCL, 0);
  digitalWrite(I2C_SDA, 0);
  delayMicroseconds(IIC_DELAY);
  digitalWrite(I2C_SCL, 1);
  delayMicroseconds(IIC_DELAY);
  digitalWrite(I2C_SDA, 1);
  delayMicroseconds(IIC_DELAY);
}

unsigned char SoftIIC::beginTransmission(unsigned char addr)
{
  byte ack = 0;
  begin();
  ack = write(addr << 1);
  endTransmission();
  return ack;
}

unsigned char SoftIIC::write(unsigned char dat)
{
  unsigned char ack,mask;

  for (mask = 0x80; mask != 0; mask >>= 1)
  {
    if ((mask & dat) == 0)digitalWrite(I2C_SDA, 0);
    else digitalWrite(I2C_SDA, 1);
    delayMicroseconds(IIC_DELAY);
    digitalWrite(I2C_SCL, 1);
    delayMicroseconds(IIC_DELAY);
    digitalWrite(I2C_SCL, 0);
  }
  digitalWrite(I2C_SDA, 1);
  delayMicroseconds(IIC_DELAY);
  digitalWrite(I2C_SCL, 1);
  ack = digitalRead(I2C_SDA);
  delayMicroseconds(IIC_DELAY);
  digitalWrite(I2C_SCL, 0);

  return (~ack);
}

unsigned char SoftIIC::read(unsigned char ack)
{
  unsigned char dat,mask;

  for (mask = 0x80; mask != 0; mask >>= 1)
  {
    digitalWrite(I2C_SCL,0);
    delayMicroseconds(IIC_DELAY);
    digitalWrite(I2C_SCL,1);
    dat<<=1;
    if(digitalRead(I2C_SDA))dat++;
    delayMicroseconds(IIC_DELAY);
  }
  
  if (ack)ACK(); 
  else NACK(); 
  return dat;
}

void SoftIIC::ACK()
{
  digitalWrite(I2C_SCL, 0);
  digitalWrite(I2C_SDA, 0);
  delayMicroseconds(IIC_DELAY);
  digitalWrite(I2C_SCL, 1);
  delayMicroseconds(IIC_DELAY);
  digitalWrite(I2C_SCL, 0);
}

void SoftIIC::NACK()
{
  digitalWrite(I2C_SCL, 0);
  digitalWrite(I2C_SDA, 1);
  delayMicroseconds(IIC_DELAY);
  digitalWrite(I2C_SCL, 1);
  delayMicroseconds(IIC_DELAY);
  digitalWrite(I2C_SCL, 0);
}
