#include "time_exti.h"
#include "Arduino.h"

void_func_point TIM_func[5]={null_func, null_func, null_func, null_func, null_func};

/**********************************************************
** 函数名:	TIM3_Int_Init(u16 arr,u16 psc)
** 功能描述: 定时器初始化
** 输入参数: arr：设置自动重装载寄存器周期	psc：设置钟频率除数的预分频值
** 输出参数: 无
** 说    明：输入时钟源为72MHZ	定时时间设置为：t=(arr*psc)/72  (us)
***********************************************************/
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr-1; 		//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc-1; 	//设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	TIM_ITConfig(TIM3, TIM_IT_Update ,ENABLE );	//使能TIM3中断

	/**********************************设置中断优先级************************************/
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

void TimerSet(u8 TimerNum,u32 us,void_func_point f)
{
	u8 NVIC_IRQ_CPP=0,NVIC_IRQ_CSP=3;
	u32 arr,psc;
	
	TIM_TypeDef* TIMx;
	u32 RCC_APB1Periph_TIMx;
	u8 TIMx_IRQn;
	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	if(TimerNum<2 || TimerNum>4)return;
	
	TIM_func[TimerNum] = f;
	
	us*=36;
	if(us<65535*1000)
	{
		arr=us/1000;
		psc=us/arr;
	}
	else
	{
		arr=us/20000;
		psc=us/arr;
	}
	
	switch(TimerNum)
	{
		case TIMER2:
			TIMx = TIM2;
			RCC_APB1Periph_TIMx = RCC_APB1Periph_TIM2;
			TIMx_IRQn = TIM2_IRQn;
			break;
		
		case TIMER3:
			TIMx = TIM3;
			RCC_APB1Periph_TIMx = RCC_APB1Periph_TIM3;
			TIMx_IRQn = TIM3_IRQn;
			break;
		
		case TIMER4:
			TIMx = TIM4;
			RCC_APB1Periph_TIMx = RCC_APB1Periph_TIM4;
			TIMx_IRQn = TIM4_IRQn;
			break;
	}
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMx, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr-1; 		//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc-1; 	//设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	TIM_ITConfig(TIMx, TIM_IT_Update ,ENABLE );	//使能TIM中断

	/**********************************设置中断优先级************************************/
	NVIC_InitStructure.NVIC_IRQChannel = TIMx_IRQn;  //TIM中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IRQ_CPP;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_IRQ_CSP;  //从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

/*****************************定时中断函数*********************************************/
void TIM2_IRQHandler(void)   //TIM2中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 		//检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_func[TIMER2]();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  	//清除TIMx的中断待处理位:TIM 中断源		
	}
}

void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_func[TIMER3]();
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

void TIM4_IRQHandler(void)   //TIM4中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_func[TIMER4]();
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}








