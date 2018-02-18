#ifndef EEPROM_SAVEFLOAT_H_
#define EEPROM_SAVEFLOAT_H_

extern"C"{
#include "Arduino.h"
}
#include "extEEPROM.h"

class EEPROM_SaveFloat
{
public:
	EEPROM_SaveFloat(unsigned int _capacity = 2048);
	void clear();
  void clear(unsigned int stop_addr);
	void clear(unsigned int start_addr,unsigned int stop_addr);
  byte write(unsigned int addr,float dat);
  float read(unsigned int addr);

private:
	unsigned int Capacity;
  union u_data;
};

#endif
