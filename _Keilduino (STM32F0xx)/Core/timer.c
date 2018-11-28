#include "timer.h"

typedef enum {
    TIMER1, TIMER2, TIMER3,
    TIMER14, TIMER15, TIMER16, TIMER17
} TIMERx_TypeDef;

Timer_CallbackFunction_t TIM_Function[] = {0, 0, 0, 0, 0, 0, 0};

void TimerSet(TIM_TypeDef* TIMx, uint32_t InterruptTime_us, Timer_CallbackFunction_t function)
{
    Timer_Init(TIMx, InterruptTime_us, function, Timer_ChannelPriority_Default);
}

void Timer_Init(TIM_TypeDef* TIMx, uint32_t InterruptTime_us, Timer_CallbackFunction_t function, uint8_t ChannelPriority)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    uint32_t arr, psc;
    uint32_t RCC_APBxPeriph_TIMx;
    uint8_t TIMx_IRQn;
    uint8_t TimerNum;

    if(!IS_TIM_ALL_PERIPH(TIMx))return;

    if(TIMx == TIM1)
    {
        TimerNum = TIMER1;
        RCC_APBxPeriph_TIMx = RCC_APB2Periph_TIM1;
        TIMx_IRQn = TIM1_BRK_UP_TRG_COM_IRQn;
    }

#if !defined(STM32F030)
    else if(TIMx == TIM2)
    {
        TimerNum = TIMER2;
        RCC_APBxPeriph_TIMx = RCC_APB1Periph_TIM2;
        TIMx_IRQn = TIM2_IRQn;
    }
#endif

    else if(TIMx == TIM3)
    {
        TimerNum = TIMER3;
        RCC_APBxPeriph_TIMx = RCC_APB1Periph_TIM3;
        TIMx_IRQn = TIM3_IRQn;
    }
    else if(TIMx == TIM14)
    {
        TimerNum = TIMER14;
        RCC_APBxPeriph_TIMx = RCC_APB1Periph_TIM14;
        TIMx_IRQn = TIM14_IRQn;
    }
    else if(TIMx == TIM15)
    {
        TimerNum = TIMER15;
        RCC_APBxPeriph_TIMx = RCC_APB2Periph_TIM15;
        TIMx_IRQn = TIM15_IRQn;
    }
    else if(TIMx == TIM16)
    {
        TimerNum = TIMER16;
        RCC_APBxPeriph_TIMx = RCC_APB2Periph_TIM16;
        TIMx_IRQn = TIM16_IRQn;
    }
    else if(TIMx == TIM17)
    {
        TimerNum = TIMER17;
        RCC_APBxPeriph_TIMx = RCC_APB2Periph_TIM17;
        TIMx_IRQn = TIM17_IRQn;
    }

    TIM_Function[TimerNum] = function;//Callback Functions

    //Calculate TIM_Period and TIM_Prescaler
    InterruptTime_us *= CYCLES_PER_MICROSECOND;
    if(InterruptTime_us < CYCLES_PER_MICROSECOND * 30)
    {
        arr = 10;
        psc = InterruptTime_us / arr;
    }
    else if(InterruptTime_us < 65535 * 1000)
    {
        arr = InterruptTime_us / 1000;
        psc = InterruptTime_us / arr;
    }
    else
    {
        arr = InterruptTime_us / 20000;
        psc = InterruptTime_us / arr;
    }

    //Enable PeriphClock
    if(TIMx == TIM1 || TIMx == TIM15 || TIMx == TIM16 || TIMx == TIM17)
    {
        RCC_APB2PeriphClockCmd(RCC_APBxPeriph_TIMx, ENABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(RCC_APBxPeriph_TIMx, ENABLE); //时钟使能
    }

    TIM_DeInit(TIMx);
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseStructure.TIM_Period = arr - 1; 		//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1; 	//设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    /**********************************设置中断优先级************************************/
    NVIC_InitStructure.NVIC_IRQChannel = TIMx_IRQn;  //TIM中断
    NVIC_InitStructure.NVIC_IRQChannelPriority = ChannelPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    TIM_ClearFlag(TIMx, TIM_FLAG_Update);
    TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);	//使能TIM中断
}

void TimerSet_InterruptTimeUpdate(TIM_TypeDef* TIMx, uint32_t InterruptTime_us)
{
	uint32_t arr, psc;
	//Calculate TIM_Period and TIM_Prescaler
    InterruptTime_us *= CYCLES_PER_MICROSECOND;
    if(InterruptTime_us < CYCLES_PER_MICROSECOND * 30)
    {
        arr = 10;
        psc = InterruptTime_us / arr;
    }
    else if(InterruptTime_us < 65535 * 1000)
    {
        arr = InterruptTime_us / 1000;
        psc = InterruptTime_us / arr;
    }
    else
    {
        arr = InterruptTime_us / 20000;
        psc = InterruptTime_us / arr;
    }
	
	TIMx->ARR = arr - 1;
    TIMx->PSC = psc - 1;
	TIMx->EGR = TIM_PSCReloadMode_Immediate;
}

/*****************************定时中断函数*********************************************/
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        TIM_Function[TIMER1]();
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}

#if !defined(STM32F030)
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_Function[TIMER2]();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
#endif

void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_Function[TIMER3]();
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}

void TIM14_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)
    {
        TIM_Function[TIMER14]();
        TIM_ClearITPendingBit(TIM14, TIM_IT_Update);
    }
}

void TIM15_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM15, TIM_IT_Update) != RESET)
    {
        TIM_Function[TIMER15]();
        TIM_ClearITPendingBit(TIM15, TIM_IT_Update);
    }
}

void TIM16_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM16, TIM_IT_Update) != RESET)
    {
        TIM_Function[TIMER16]();
        TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
    }
}

void TIM17_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM17, TIM_IT_Update) != RESET)
    {
        TIM_Function[TIMER17]();
        TIM_ClearITPendingBit(TIM17, TIM_IT_Update);
    }
}
