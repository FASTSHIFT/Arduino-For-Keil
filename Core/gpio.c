#include "GPIO.h"

void GPIO_begin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIOMode, GPIOSpeed_TypeDef GPIOSpeed)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint32_t RCC_APB2Periph_GPIOx;
	
			 if(GPIOx == GPIOA)RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOA;	
	else if(GPIOx == GPIOB)RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOB;	
	else if(GPIOx == GPIOC)RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOC;	
	else if(GPIOx == GPIOD)RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOD;	
	else if(GPIOx == GPIOE)RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOE;	
	else if(GPIOx == GPIOF)RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOF;	
	else if(GPIOx == GPIOG)RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOG;	
	else return;
	
	if((GPIOMode == GPIO_Mode_AF_OD) || (GPIOMode == GPIO_Mode_AF_PP))
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx|RCC_APB2Periph_AFIO, ENABLE);
	}
	else RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIOMode;
	GPIO_InitStructure.GPIO_Speed = GPIOSpeed;		
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}

void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	GPIOx->ODR^=GPIO_Pin;
}

uint8_t Get_GPIOx(int Pin)
{
	GPIO_TypeDef* GPIOx = (GPIO_TypeDef*)(Pin&~0x0F);

			 if(GPIOx == GPIOA)return 0;
	else if(GPIOx == GPIOB)return 1;
	else if(GPIOx == GPIOC)return 2;
	else if(GPIOx == GPIOD)return 3;
	else if(GPIOx == GPIOE)return 4;
	else if(GPIOx == GPIOF)return 5;
	else if(GPIOx == GPIOG)return 6;
	else return 10;
}

uint8_t Get_Pinx(int Pin)
{
	return (Pin - (Pin&~0x0F));
}
