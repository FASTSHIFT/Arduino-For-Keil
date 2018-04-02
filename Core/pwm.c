#include "pwm.h"

//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIMx_Init(TIM_TypeDef* TIMx,u16 arr,u16 psc,uint8_t CHx)//f=72MHz/arr/psc,最大占空比=arr
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

			 if(TIMx == TIM1)RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	else if(TIMx == TIM2)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	else if(TIMx == TIM3)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	else if(TIMx == TIM4)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	switch(CHx)
	{
		case 1:
			TIM_OC1Init(TIMx, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
			break;
		case 2:
			TIM_OC2Init(TIMx, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
			break;
		case 3:
			TIM_OC3Init(TIMx, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
			break;
		case 4:
			TIM_OC4Init(TIMx, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);
			break;
	}
	
	TIM_Cmd(TIMx, ENABLE);
	if(TIMx==TIM1 || TIMx==TIM8)TIM_CtrlPWMOutputs(TIMx, ENABLE);
}

uint8_t PWM_Init(uint8_t Pin, uint16_t PWM_DutyCycle, uint16_t PWM_Frequency)
{
	uint32_t arr,psc;
	
	if(!IS_PWM_PIN(Pin))return 0;
	
	pinMode(Pin,GPIO_Mode_AF_PP);
	
	arr = PWM_DutyCycle;
	psc = 72000000/PWM_DutyCycle/PWM_Frequency;
	
	TIM_Cmd(PIN_MAP[Pin].TIMx, DISABLE);
	TIMx_Init(PIN_MAP[Pin].TIMx,arr-1,psc-1,PIN_MAP[Pin].TimerChannel);
	return PIN_MAP[Pin].TimerChannel;
}

uint16_t pwmWrite(uint8_t Pin,uint16_t val)
{
	switch(PIN_MAP[Pin].TimerChannel)
	{
		case 1: PIN_MAP[Pin].TIMx->CCR1 = val;break;
		case 2: PIN_MAP[Pin].TIMx->CCR2 = val;break;
		case 3: PIN_MAP[Pin].TIMx->CCR3 = val;break;
		case 4: PIN_MAP[Pin].TIMx->CCR4 = val;break;
	}
	return val;
}

//*************************//

uint16_t timer_get_compare(TIM_TypeDef* TIMx,u8 TimerChannel)
{
	u16 compare=0;
	switch(TimerChannel)
	{
		case 1: compare = TIMx->CCR1;break;
		case 2: compare = TIMx->CCR2;break;
		case 3: compare = TIMx->CCR3;break;
		case 4: compare = TIMx->CCR4;break;
	}
	return compare;
}

void timer_set_prescaler(TIM_TypeDef* TIMx,uint16_t psc)
{
   TIMx->PSC = psc;
}

void timer_set_reload(TIM_TypeDef* TIMx,uint16_t arr)
{
  TIMx->ARR = arr;
}

void timer_generate_update(TIM_TypeDef* TIMx)
{
	TIMx->EGR = TIM_PSCReloadMode_Immediate; 
}
