/*
Base on OneWire v2.2
Edit by savage 2014-05-07

For single senser only

*/

#include "DS18B20_S.h"

DS18B20_S::DS18B20_S(uint8_t pin)
{
	PIN=pin;
	pinMode(pin, OUTPUT);
	digitalWrite(PIN,1);
	delayMicroseconds(1000);//??????,??????????,????
  set(0x3F);//???10???(???12???)
}

void DS18B20_S::set(byte n){
	reset();	
	write(0xCC);//skip	
	write(0x4E);//write
	write(0);//Th
	write(0);//Tl
	write(n);//seting 10bit mode B00111111 10???
}	

// Returns 1 if a device asserted a presence pulse, 0 otherwise.
//
uint8_t DS18B20_S::reset()	
{	
	uint8_t r;
	pinMode(PIN,OUTPUT);
	digitalWrite(PIN,0);
	delayMicroseconds(480);//??????480um,reset	
	pinMode(PIN,INPUT);
	
	noInterrupts();
	delayMicroseconds(70);		
	r=!digitalRead(PIN);//?????
	interrupts();
	
	delayMicroseconds(410);
	return r;
}

//
// Write a bit. Port and bit is used to cut lookup time and provide
// more certain timing.
//
void DS18B20_S::write_bit(uint8_t v)
{
  pinMode(PIN,OUTPUT);  
	if (v & 1) {		//?1
		noInterrupts();	
    digitalWrite(PIN,0);
		delayMicroseconds(5);
		digitalWrite(PIN,1);
		interrupts();
		delayMicroseconds(55);
	} else {     //?0
		noInterrupts();
		digitalWrite(PIN,0);
		delayMicroseconds(60);		
    digitalWrite(PIN,1);
    interrupts();
		delayMicroseconds(5);
	}
}
//
// Read a bit. Port and bit is used to cut lookup time and provide
// more certain timing.
//
uint8_t DS18B20_S::read_bit()
{
	uint8_t r;	
	pinMode(PIN,OUTPUT);
	noInterrupts();
	digitalWrite(PIN,0);//????1us??
	delayMicroseconds(2);
	pinMode(PIN,INPUT);     	
	delayMicroseconds(10);// let pin float, pull up will raise
	r = digitalRead(PIN);//???15us???
	interrupts();
	delayMicroseconds(50);
	return r;
}
//?
void DS18B20_S::write(uint8_t v) {
	  noInterrupts();	
    uint8_t bitMask;
    for (bitMask = 0x01; bitMask; bitMask <<= 1) {
	  DS18B20_S::write_bit( (bitMask & v)?1:0);
    }
    interrupts();
 }


// Read 
uint8_t DS18B20_S::read() {
	  noInterrupts();	
    uint8_t bitMask;
    uint8_t r = 0;
    for (bitMask = 0x01; bitMask; bitMask <<= 1) {
	  if ( DS18B20_S::read_bit()) r |= bitMask;
    }
    interrupts();
    return r;
}

void DS18B20_S::start(){
	 reset();
   write(0xCC);//skip
   write(0x44);
}

bool DS18B20_S::ready(){
	  return read()&0x01;
}

float DS18B20_S::get(){	
	reset();	
	write(0xCC);//
	write(0xBE);
	byte data[9];
	byte i;
	for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = read();
  }
  if(crc8(data, 8)!=data[8])return 250;//crc ??
  int raw = (data[1] << 8) | data[0];	
  return raw*0.0625;  		
}


#if DS18B20_S_CRC
// The 1-Wire CRC scheme is described in Maxim Application Note 27:
// "Understanding and Using Cyclic Redundancy Checks with Maxim iButton Products"
// Compute a Dallas Semiconductor 8 bit CRC directly.
// this is much slower, but much smaller, than the lookup table.
//
uint8_t DS18B20_S::crc8(const uint8_t *addr, uint8_t len)
{
	uint8_t crc = 0;	
	while (len--) {
		uint8_t inbyte = *addr++;
		for (uint8_t i = 8; i; i--) {
			uint8_t mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}
#endif
