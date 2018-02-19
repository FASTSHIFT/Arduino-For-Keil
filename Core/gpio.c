#include "gpio.h"

const STM32_PinInfo PIN_MAP[]={
/*GPIO_TypeDef* GPIOx;
	TIM_TypeDef* TIMx;
	ADC_TypeDef* ADCx;

	uint16_t GPIO_Pin_x;	
	uint8_t TimerChannel;	
	uint8_t ADC_Channel;*/
	{GPIOA, TIM2, ADC1,  GPIO_Pin_0, 1,    0}, /* PA0 */
	{GPIOA, TIM2, ADC1,  GPIO_Pin_1, 2,    1}, /* PA1 */
	{GPIOA, TIM2, ADC1,  GPIO_Pin_2, 3,    2}, /* PA2 */
	{GPIOA, TIM2, ADC1,  GPIO_Pin_3, 4,    3}, /* PA3 */
	{GPIOA, NULL, ADC1,  GPIO_Pin_4, 0,    4}, /* PA4 */	
	{GPIOA, NULL, ADC1,  GPIO_Pin_5, 0,    5}, /* PA5 */
	{GPIOA, TIM3, ADC1,  GPIO_Pin_6, 1,    6}, /* PA6 */
	{GPIOA, TIM3, ADC1,  GPIO_Pin_7, 2,    7}, /* PA7 */
	{GPIOA, TIM1, NULL,  GPIO_Pin_8, 1, xADC}, /* PA8 */
	{GPIOA, TIM1, NULL,  GPIO_Pin_9, 2, xADC}, /* PA9 */
	{GPIOA, TIM1, NULL, GPIO_Pin_10, 3, xADC}, /* PA10 */
	{GPIOA, TIM1, NULL, GPIO_Pin_11, 4, xADC}, /* PA11 */
	{GPIOA, NULL, NULL, GPIO_Pin_12, 0, xADC}, /* PA12 */	
	{GPIOA, NULL, NULL, GPIO_Pin_13, 0, xADC}, /* PA13 */
	{GPIOA, NULL, NULL, GPIO_Pin_14, 0, xADC}, /* PA14 */
	{GPIOA, NULL, NULL, GPIO_Pin_15, 0, xADC}, /* PA15 */

	{GPIOB, TIM3, ADC1,  GPIO_Pin_0, 3,    8}, /* PB0 */
	{GPIOB, TIM3, ADC1,  GPIO_Pin_1, 4,    9}, /* PB1 */
	{GPIOB, NULL, NULL,  GPIO_Pin_2, 0, xADC}, /* PB2 */	
	{GPIOB, NULL, NULL,  GPIO_Pin_3, 0, xADC}, /* PB3 */
	{GPIOB, NULL, NULL,  GPIO_Pin_4, 0, xADC}, /* PB4 */
	{GPIOB, NULL, NULL,  GPIO_Pin_5, 0, xADC}, /* PB5 */
	{GPIOB, TIM4, NULL,  GPIO_Pin_6, 1, xADC}, /* PB6 */
	{GPIOB, TIM4, NULL,  GPIO_Pin_7, 2, xADC}, /* PB7 */
	{GPIOB, TIM4, NULL,  GPIO_Pin_8, 3, xADC}, /* PB8 */
	{GPIOB, TIM4, NULL,  GPIO_Pin_9, 4, xADC}, /* PB9 */
	{GPIOB, NULL, NULL, GPIO_Pin_10, 0, xADC}, /* PB10 */
	{GPIOB, NULL, NULL, GPIO_Pin_11, 0, xADC}, /* PB11 */
	{GPIOB, NULL, NULL, GPIO_Pin_12, 0, xADC}, /* PB12 */
	{GPIOB, NULL, NULL, GPIO_Pin_13, 0, xADC}, /* PB13 */
	{GPIOB, NULL, NULL, GPIO_Pin_14, 0, xADC}, /* PB14 */
	{GPIOB, NULL, NULL, GPIO_Pin_15, 0, xADC}, /* PB15 */


	{GPIOC, NULL, ADC1,  GPIO_Pin_0, 0,   10}, /* PC0 */
	{GPIOC, NULL, ADC1,  GPIO_Pin_1, 0,   11}, /* PC1 */
	{GPIOC, NULL, ADC1,  GPIO_Pin_2, 0,   12}, /* PC2 */
	{GPIOC, NULL, ADC1,  GPIO_Pin_3, 0,   13}, /* PC3 */
	{GPIOC, NULL, ADC1,  GPIO_Pin_4, 0,   14}, /* PC4 */
	{GPIOC, NULL, ADC1,  GPIO_Pin_5, 0,   15}, /* PC5 */
	{GPIOC, TIM8, NULL,  GPIO_Pin_6, 1, xADC}, /* PC6 */	
	{GPIOC, TIM8, NULL,  GPIO_Pin_7, 2, xADC}, /* PC7 */
	{GPIOC, TIM8, NULL,  GPIO_Pin_8, 3, xADC}, /* PC8 */
	{GPIOC, TIM8, NULL,  GPIO_Pin_9, 4, xADC}, /* PC9 */
	{GPIOC, NULL, NULL, GPIO_Pin_10, 0, xADC}, /* PC10 UART4_TX/SDIO_D2 */
	{GPIOC, NULL, NULL, GPIO_Pin_11, 0, xADC}, /* PC11 UART4_RX/SDIO_D3 */
	{GPIOC, NULL, NULL, GPIO_Pin_12, 0, xADC}, /* PC12 UART5_TX/SDIO_CK */	
	{GPIOC, NULL, NULL, GPIO_Pin_13, 0, xADC}, /* PC13 TAMPER-RTC */
	{GPIOC, NULL, NULL, GPIO_Pin_14, 0, xADC}, /* PC14 OSC32_IN */
	{GPIOC, NULL, NULL, GPIO_Pin_15, 0, xADC}  /* PC15 OSC32_OUT */
};

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

uint8_t Get_GPIOx(uint8_t Pin)
{
			 if(PIN_MAP[Pin].GPIOx == GPIOA)return 0;
	else if(PIN_MAP[Pin].GPIOx == GPIOB)return 1;
	else if(PIN_MAP[Pin].GPIOx == GPIOC)return 2;
	else if(PIN_MAP[Pin].GPIOx == GPIOD)return 3;
	else if(PIN_MAP[Pin].GPIOx == GPIOE)return 4;
	else if(PIN_MAP[Pin].GPIOx == GPIOF)return 5;
	else if(PIN_MAP[Pin].GPIOx == GPIOG)return 6;
	else return 0xFF;
}
