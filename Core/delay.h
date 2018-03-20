<<<<<<< HEAD
#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
extern "C"{
#endif

#include "stm32f10x.h"

void Delay_Init(void);
uint32_t millis(void);
uint32_t micros(void);
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);
	
#ifdef __cplusplus
}
#endif	

#endif /* __DELAY_H */
=======
#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void delay_init(void);
u32 millis(void );
u32 micros(void );
void delay_ms(u32 ms);
void delay_us(u32 us);

#endif /* __SYSTICK_H */

>>>>>>> 301cbab54594d1de84f6a7e456407b3a84e8d255
