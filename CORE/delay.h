#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void delay_init(void);
u32 millis(void );
u32 micros(void );
void delay_ms(u32 ms);
void delay_us(u32 us);

#endif /* __SYSTICK_H */

