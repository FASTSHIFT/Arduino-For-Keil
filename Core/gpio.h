#ifndef __GPIO_H 
#define __GPIO_H   

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

#ifndef NULL
#define NULL 0
#endif

#define xADC 0xFF

enum{
  PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
	PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
	PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15
};

typedef struct STM32_PinInfo{
    GPIO_TypeDef* GPIOx;
		TIM_TypeDef* TIMx;
		ADC_TypeDef* ADCx;
	
		uint16_t GPIO_Pin_x;	
		uint8_t TimerChannel;	
    uint8_t ADC_Channel;
}STM32_PinInfo;

extern const STM32_PinInfo PIN_MAP[];

#define IS_ADC_PIN(Pin) (((Pin)==PA0) || ((Pin)==PA1) || ((Pin)==PA2) || ((Pin)==PA3) || \
												 ((Pin)==PA4) || ((Pin)==PA5) || ((Pin)==PA6) || ((Pin)==PA7) || \
												 ((Pin)==PB0) || ((Pin)==PB1))

#define IS_PWM_PIN(Pin) (((Pin)==PA8) || ((Pin)==PA9) || ((Pin)==PA10) || ((Pin)==PA11) || \
												 ((Pin)==PA0) || ((Pin)==PA1) || ((Pin)==PA2) || ((Pin)==PA3) || \
												 ((Pin)==PA6) || ((Pin)==PA7) || ((Pin)==PB0) || ((Pin)==PB1) || \
												 ((Pin)==PB6) || ((Pin)==PB7) || ((Pin)==PB8) || ((Pin)==PB9))

void GPIO_Config(void);
void GPIO_begin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,GPIOMode_TypeDef GPIOMode,GPIOSpeed_TypeDef GPIOSpeed);
void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint8_t Get_GPIOx(uint8_t Pin);
uint8_t Get_Pinx(uint8_t Pin);

#endif 
