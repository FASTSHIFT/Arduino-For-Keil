#ifndef __MCU_TYPE
#define __MCU_TYPE

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include <mcu_config.h>

#define __STM32__
#define __STM32F4__
#ifndef F_CPU
#   define F_CPU                    SystemCoreClock
#endif
#define CYCLES_PER_MICROSECOND      (F_CPU / 1000000U)

#endif
