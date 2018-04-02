#include "time_exti.h"

enum{TIMER1,TIMER2,TIMER3,TIMER4,TIMER5,TIMER6,TIMER7,TIMER8};

void_func_point TIM_func[8]={null_func, null_func, null_func, null_func, null_func, null_func, null_func, null_func};

void TimerSet(TIM_TypeDef* TIMx,uint32_t InterruptTime_us,void_func_point f)
{
	TimerSet_NVIC(TIMx, InterruptTime_us, f, 0, 3);
}

void TimerSet_NVIC(TIM_TypeDef* TIMx,uint32_t InterruptTime_us,void_func_point f,u8 NVIC_IRQ_CPP, u8 NVIC_IRQ_CSP)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	uint32_t arr,psc;
	uint32_t RCC_APBxPeriph_TIMx;
	uint8_t TIMx_IRQn;
	uint8_t TimerNum;
	
	if(!IS_TIM_ALL_PERIPH(TIMx))return;
	
	if(TIMx == TIM1)
	{
		TimerNum = TIMER1;
		RCC_APBxPeriph_TIMx = RCC_APB2Periph_TIM1;
		TIMx_IRQn = TIM1_UP_IRQn;
	}
	else if(TIMx == TIM2)
	{
		TimerNum = TIMER2;
		RCC_APBxPeriph_TIMx = RCC_APB1Periph_TIM2;
		TIMx_IRQn = TIM2_IRQn;
	}
	else if(TIMx == TIM3)
	{
		TimerNum = TIMER3;
		RCC_APBxPeriph_TIMx = RCC_APB1Periph_TIM3;
		TIMx_IRQn = TIM3_IRQn;
	}
	else if(TIMx == TIM4)
	{
		TimerNum = TIMER4;
		RCC_APBxPeriph_TIMx = RCC_APB1Periph_TIM4;
		TIMx_IRQn = TIM4_IRQn;
	}
#ifdef STM32F10X_HD
	else if(TIMx == TIM5)
	{
		TimerNum = TIMER5;
		RCC_APBxPeriph_TIMx = RCC_APB1Periph_TIM5;
		TIMx_IRQn = TIM5_IRQn;
	}
	else if(TIMx == TIM6)
	{
		TimerNum = TIMER6;
		RCC_APBxPeriph_TIMx = RCC_APB1Periph_TIM6;
		TIMx_IRQn = TIM6_IRQn;
	}
	else if(TIMx == TIM7)
	{
		TimerNum = TIMER5;
		RCC_APBxPeriph_TIMx = RCC_APB1Periph_TIM7;
		TIMx_IRQn = TIM7_IRQn;
	}
	else if(TIMx == TIM8)
	{
		TimerNum = TIMER8;
		RCC_APBxPeriph_TIMx = RCC_APB2Periph_TIM8;
		TIMx_IRQn = TIM8_UP_IRQn;
	}
#endif
	
	TIM_func[TimerNum] = f;//Callback Functions

	//Calculate TIM_Period and TIM_Prescaler
	InterruptTime_us *= 72;
	if(InterruptTime_us < 72*30)
	{
		arr=10;
		psc=InterruptTime_us / arr;
	}
	else if(InterruptTime_us < 65535*1000)
	{
		arr=InterruptTime_us / 1000;
		psc=InterruptTime_us / arr;
	}
	else
	{
		arr=InterruptTime_us / 20000;
		psc=InterruptTime_us / arr;
	}
	
	//Enable PeriphClock
	if(TIMx == TIM1 || TIMx == TIM8)
	{
		RCC_APB2PeriphClockCmd(RCC_APBxPeriph_TIMx, ENABLE);
	}
	else 
	{
		RCC_APB1PeriphClockCmd(RCC_APBxPeriph_TIMx, ENABLE); //时钟使能
	}

	TIM_DeInit(TIMx);
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_Period = arr-1; 		//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc-1; 	//设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	/**********************************设置中断优先级************************************/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //设置NVIC中断分组1
	NVIC_InitStructure.NVIC_IRQChannel = TIMx_IRQn;  //TIM中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IRQ_CPP;  //先占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_IRQ_CSP;  //从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	TIM_ClearFlag(TIMx, TIM_FLAG_Update); 
	TIM_ITConfig(TIMx, TIM_IT_Update ,ENABLE);	//使能TIM中断
}

/*****************************定时中断函数*********************************************/
void TIM1_UP_IRQHandler(void)   //TIM1中断
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) 		//检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_func[TIMER1]();
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  	//清除TIMx的中断待处理位:TIM 中断源		
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_func[TIMER2]();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_func[TIMER3]();
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_func[TIMER4]();
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}

#if STM32F10X_HD
void TIM5_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{
		TIM_func[TIMER5]();
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	}
}

void TIM6_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		TIM_func[TIMER6]();
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
}

void TIM7_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
	{
		TIM_func[TIMER7]();
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	}
}

void TIM8_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)
	{
		TIM_func[TIMER8]();
		TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
	}
}
#endif
