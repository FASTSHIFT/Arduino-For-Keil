#ifndef __FILEGROUP_H
#define	__FILEGROUP_H

//*************C/C++ Library**************//
extern"C"{
#include "stm32f10x.h"
#include "Arduino.h"
#include "math.h"
#include "delay.h"
#include "gpio.h"
#include "adc.h"
#include "usart.h"
#include "spi.h"
#include "exti.h"
#include "time_exti.h"
#include "pwm.h"
#include "C_call_CPP.h"
}

#include "HardwareSerial.h"
#include "HardwareSPI.h"
#include "Wire.h"
#include "WString.h"
#include "Stream.h"

//*************Object**************//
extern HardwareSerial Serial;
extern HardwareSerial Serial2;
extern HardwareSerial Serial3;
extern HardwareSPI SPI;
extern HardwareSPI SPI_2;
extern TwoWire Wire;

#endif
