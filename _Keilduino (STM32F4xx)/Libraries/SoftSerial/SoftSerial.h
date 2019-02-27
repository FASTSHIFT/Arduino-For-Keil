/*
TXOnlySerial.h (derived from SoftSerial.h) - 
Multi-instance software serial TX only library for Arduino/Wiring
-- Tuning, circular buffer, derivation from class Print/Stream,
   multi-instance support, porting to 8MHz processors,
   various optimizations, PROGMEM delay tables, inverse logic and 
   direct port writing by Mikal Hart (http://www.arduiniana.org)
-- 20MHz processor support by Garrett Mace (http://www.macetech.com)
-- ATmega1280/2560 support by Brett Hagman (http://www.roguerobotics.com/)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Latest version could be found here: https://github.com/felias-fogg
*/

#ifndef __SOFTSERIAL_h
#define __SOFTSERIAL_h

#include <inttypes.h>
#include <Stream.h>
#include "Arduino.h"

/******************************************************************************
* Definitions
******************************************************************************/

#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

class SoftSerial : public Stream
{
private:
  // per object data
  GPIO_TypeDef *TX_GPIOx, *RX_GPIOx;
	uint16_t TX_GPIO_Pin_x, RX_GPIO_Pin_x;

  uint16_t _tx_delay;

  uint16_t _buffer_overflow:1;
  uint16_t _inverse_logic:1;


  // private methods
  void setTX(uint8_t transmitPin);

  // Return num - sub, or 1 if the result would be < 1
  static uint16_t subtract_cap(uint16_t num, uint16_t sub);

  // private static method for timing
  static inline void tunedDelay(uint16_t delay);

public:
  // public methods
  SoftSerial(uint8_t transmitPin, bool inverse_logic = false);
  ~SoftSerial();
  void begin(long speed);
  void end();
  bool overflow() { bool ret = _buffer_overflow; if (ret) _buffer_overflow = false; return ret; }

  virtual int peek();
  virtual int read();
  virtual int available();
  virtual size_t write(uint8_t byte);
  virtual void flush();
  operator bool() { return true; }
  
  using Print::write;
	
};

#endif
