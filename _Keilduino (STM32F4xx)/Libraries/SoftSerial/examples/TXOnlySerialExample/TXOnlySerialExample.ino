/*
  TXOnly serial test

 This example code is in the public domain.

 */
#define SERIALOUT 4
//#define TXONLY
#define SWSERIAL




#ifdef TXONLY
#include <TXOnlySerial.h>

TXOnlySerial mySerial(SERIALOUT); // TX
#endif

#ifdef SWSERIAL
#include <SoftwareSerial.h>

SoftwareSerial mySerial(SERIALOUT+1, SERIALOUT); // TX
#endif


void setup() {
#if defined(TXONLY) || defined(SWSERIAL)
  mySerial.begin(19200);
  mySerial.println("Hello, world?");
  mySerial.println(42,HEX);
#endif
}

void loop() { 
}

