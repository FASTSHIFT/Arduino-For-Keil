#ifndef __MCU_TYPE
#define __MCU_TYPE

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#define __STM32__
#define __STM32F4__
#define F_CPU                   SystemCoreClock
#define CYCLES_PER_MICROSECOND (F_CPU / 1000000U)

#define __KEILDUINO__ 310

#endif
