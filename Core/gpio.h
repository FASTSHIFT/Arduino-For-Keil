#ifndef __GPIO_H 
#define __GPIO_H   

#ifdef __cplusplus
extern "C"{
#endif

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

#ifndef NULL
#define NULL 0
#endif

#define xADC 0xFF

typedef enum{
	PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
	PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
	PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15,
	PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15,
	PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7, PE8, PE9, PE10, PE11, PE12, PE13, PE14, PE15,
	PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7, PF8, PF9, PF10, PF11, PF12, PF13, PF14, PF15,
	PG0, PG1, PG2, PG3, PG4, PG5, PG6, PG7, PG8, PG9, PG10, PG11, PG12, PG13, PG14, PG15
}Pxx_TypeDef;

typedef struct STM32_PinInfo{
	GPIO_TypeDef* GPIOx;
	TIM_TypeDef* TIMx;
	ADC_TypeDef* ADCx;

	uint16_t GPIO_Pin_x;	
	uint8_t TimerChannel;	
	uint8_t ADC_Channel;
}PinInfo_TypeDef;

extern const PinInfo_TypeDef PIN_MAP[];

#define IS_ADC_PIN(Pin) (PIN_MAP[Pin].ADC_Channel != xADC)

#define IS_PWM_PIN(Pin) (PIN_MAP[Pin].TIMx != NULL)

void GPIO_Config(void);
void GPIO_begin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x,GPIOMode_TypeDef GPIO_Mode_x,GPIOSpeed_TypeDef GPIO_Speed_x);
void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x);
uint8_t Get_GPIOx(uint8_t Pin);
uint8_t Get_Pinx(uint8_t Pin);

#ifdef __cplusplus
}
#endif

#endif 
