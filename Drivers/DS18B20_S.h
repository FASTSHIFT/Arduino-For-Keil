#ifndef DS18B20_S_h
#define DS18B20_S_h

extern "C"
{
#include <inttypes.h>
#include "FileGroup.h"
}

//#if ARDUINO >= 100
//#include "Arduino.h"       // for delayMicroseconds, digitalPinToBitMask, etc
//#else
//#include "WProgram.h"      // for delayMicroseconds
//#include "pins_arduino.h"  // for digitalPinToBitMask, etc
//#endif


// You can exclude CRC checks altogether by defining this to 0
#ifndef DS18B20_S_CRC
#define DS18B20_S_CRC 1
#endif


#define FALSE 0
#define TRUE  1

class DS18B20_S
{
  private:
  	uint8_t PIN;
    void write(uint8_t v);    
    void write_bit(uint8_t v); 
    uint8_t read(void);   
    uint8_t read_bit(void);
    uint8_t reset(void);  
  public:
  	void set(byte n);
    DS18B20_S( uint8_t pin);
    float get();
    void start();
    bool ready();    

#if DS18B20_S_CRC
    // Compute a Dallas Semiconductor 8 bit CRC, these are used in the
    // ROM and scratchpad registers.
    static uint8_t crc8(const uint8_t *addr, uint8_t len);

#endif
};

#endif
