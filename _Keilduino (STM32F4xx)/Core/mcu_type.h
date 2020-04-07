#ifndef __MCU_TYPE
#define __MCU_TYPE

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#define __STM32F4__
#define F_CPU SystemCoreClock
#define CYCLES_PER_MICROSECOND (F_CPU / 1000000U)

#define __KEILDUINO__ 300

#define GPIO_HIGH(GPIOx,GPIO_Pin_x)     ((GPIOx)->BSRRL = (GPIO_Pin_x))
#define GPIO_LOW(GPIOx,GPIO_Pin_x)      ((GPIOx)->BSRRH = (GPIO_Pin_x))
#define GPIO_READ(GPIOx,GPIO_Pin_x)    (((GPIOx)->IDR   & (GPIO_Pin_x))!=0)
#define GPIO_TOGGLE(GPIOx,GPIO_Pin_x)   ((GPIOx)->ODR  ^= (GPIO_Pin_x))

#define digitalWrite_HIGH(Pin) (GPIO_HIGH  (PIN_MAP[Pin].GPIOx,PIN_MAP[Pin].GPIO_Pin_x))
#define digitalWrite_LOW(Pin)  (GPIO_LOW   (PIN_MAP[Pin].GPIOx,PIN_MAP[Pin].GPIO_Pin_x))
#define digitalRead_FAST(Pin)  (GPIO_READ  (PIN_MAP[Pin].GPIOx,PIN_MAP[Pin].GPIO_Pin_x))
#define togglePin(Pin)         (GPIO_TOGGLE(PIN_MAP[Pin].GPIOx,PIN_MAP[Pin].GPIO_Pin_x))

#endif
