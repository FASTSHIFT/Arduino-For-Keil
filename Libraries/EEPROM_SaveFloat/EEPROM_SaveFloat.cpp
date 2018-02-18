#include "EEPROM_SaveFloat.h"

union u_data
{
  float dt_float;
  byte dt_byte[4];
}col;

unsigned int Capacity;
extEEPROM EEPROM(kbits_2, 1, 8, 0x50);

EEPROM_SaveFloat::EEPROM_SaveFloat(unsigned int _capacity)
{
	EEPROM.begin();
	Capacity = _capacity;
}

void EEPROM_SaveFloat::clear(unsigned int start_addr,unsigned int stop_addr)
{
	for (unsigned int i = start_addr; i < stop_addr; i++)
	{
		EEPROM.write(i, 0);
	}
}

void EEPROM_SaveFloat::clear(unsigned int stop_addr)
{
  clear(0,stop_addr);
}

void EEPROM_SaveFloat::clear()
{
  clear(0,Capacity);
}


byte EEPROM_SaveFloat::write(unsigned int addr,float dat)
{
	if(addr > Capacity/4)return 0;
  col.dt_float = dat;
  for(byte i=0;i<4;i++)
	{
		EEPROM.write(i+4*addr,col.dt_byte[i]);
	}
}

float EEPROM_SaveFloat::read(unsigned int addr)
{
	if(addr > Capacity/4)return 0;
  for(byte i=0;i<4;i++)col.dt_byte[i]=EEPROM.read(i+4*addr);
  return col.dt_float;
}
