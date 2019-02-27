/*
SoftSerial.cpp (derived from SoftSerial.cpp) - 
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

// 
// Includes
// 
#include <avr/pgmspace.h>
#include <Arduino.h>
#include <SoftSerial.h>

//
// Private methods
//

/* static */ 
inline void SoftSerial::tunedDelay(uint16_t delay) { 
  
}

//
// Constructor
//
SoftSerial::SoftSerial(uint8_t transmitPin, bool inverse_logic /* = false */) : 
  _tx_delay(0),
  _buffer_overflow(false),
  _inverse_logic(inverse_logic)
{
  setTX(transmitPin);
}

//
// Destructor
//
SoftSerial::~SoftSerial()
{
  end();
}

void SoftSerial::setTX(uint8_t tx)
{
  // First write, then set output. If we do this the other way around,
  // the pin would be output low for a short while before switching to
  // output hihg. Now, it is input with pullup for a short while, which
  // is fine. With inverse logic, either order is fine.
  digitalWrite(tx, _inverse_logic ? LOW : HIGH);
  pinMode(tx, OUTPUT);
  TX_GPIOx = PIN_MAP[tx].GPIOx;
	TX_GPIO_Pin_x = PIN_MAP[tx].GPIO_Pin_x;
}

uint16_t SoftSerial::subtract_cap(uint16_t num, uint16_t sub) {
  if (num > sub)
    return num - sub;
  else
    return 1;
}

//
// Public methods
//

void SoftSerial::begin(long speed)
{
  // Precalculate the various delays, in number of 4-cycle delays
  uint16_t bit_delay = (F_CPU / speed) / 4;

  // 12 (gcc 4.8.2) or 13 (gcc 4.3.2) cycles from start bit to first bit,
  // 15 (gcc 4.8.2) or 16 (gcc 4.3.2) cycles between bits,
  // 12 (gcc 4.8.2) or 14 (gcc 4.3.2) cycles from last bit to stop bit
  // These are all close enough to just use 15 cycles, since the inter-bit
  // timings are the most critical (deviations stack 8 times)
  _tx_delay = subtract_cap(bit_delay, 15 / 4);

}

void SoftSerial::end()
{

}


size_t SoftSerial::write(uint8_t b)
{
  if (_tx_delay == 0) {
    setWriteError();
    return 0;
  }

  // By declaring these as local variables, the compiler will put them
  // in registers _before_ disabling interrupts and entering the
  // critical timing sections below, which makes it a lot easier to
  // verify the cycle timings
  bool inv = _inverse_logic;
  uint16_t delay = _tx_delay;

  if (inv)
    b = ~b;

  cli();  // turn off interrupts for a clean txmit

  // Write the start bit
  if (inv)
    GPIO_HIGH(TX_GPIOx,TX_GPIO_Pin_x);
  else
    GPIO_LOW(TX_GPIOx,TX_GPIO_Pin_x);

  tunedDelay(delay);

  // Write each of the 8 bits
  for (uint8_t i = 8; i > 0; --i)
  {
    if (b & 1) // choose bit
      GPIO_HIGH(TX_GPIOx,TX_GPIO_Pin_x); // send 1
    else
      GPIO_LOW(TX_GPIOx,TX_GPIO_Pin_x); // send 0

    tunedDelay(delay);
    b >>= 1;
  }

  // restore pin to natural state
  if (inv)
    GPIO_LOW(TX_GPIOx,TX_GPIO_Pin_x);
  else
    GPIO_HIGH(TX_GPIOx,TX_GPIO_Pin_x);

  tunedDelay(_tx_delay);
  
  return 1;
}

void SoftSerial::flush()
{
  // There is no tx buffering, simply return
}

int SoftSerial::available()
{
  return 0; // we do not read anything
}

int SoftSerial::peek()
{
  return 0; // nothing to peek
}

int SoftSerial::read()
{
  return 0; // nothing to read
}
