#include "timer.h"

typedef enum {
    TIMER1, TIMER2, TIMER3, TIMER4, 
    TIMER5, TIMER6, TIMER7, TIMER8, 
    TIMER_MAX
}TIMERx_Type;//定时器编号枚举

static Timer_CallbackFunction_t TIMx_Function[TIMER_MAX] = {0};//定时中断回调函数指针数组

/**
  * @brief  定时中断配置
  * @param  TIMx:定时器地址
  * @param  InterruptTime_us: 中断时间(微秒)
  * @param  function: 定时中断回调函数
  * @retval 无
  */
void TimerSet(TIM_TypeDef* TIMx, uint32_t InterruptTime_us, Timer_CallbackFunction_t function)
{
    Timer_Init(
        TIMx, 
        InterruptTime_us, 
        function, 
        Timer_PreemptionPriority_Default, 
        Timer_SubPriority_Default
    );
}

/**
  * @brief  更新定时中断时间
  * @param  TIMx:定时器地址
  * @param  InterruptTime_us: 中断时间(微秒)
  * @retval 无
  */
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

/**
  * @brief  定时中断配置
  * @param  TIMx:定时器地址
  * @param  InterruptTime_us: 中断时间(微秒)
  * @param  function: 定时中断回调函数
  * @param  PreemptionPriority: 抢占优先级
  * @param  SubPriority: 子优先级
  * @retval 无
  */
void Timer_Init(TIM_TypeDef* TIMx, uint32_t InterruptTime_us, Timer_CallbackFunction_t function, uint8_t PreemptionPriority, uint8_t SubPriority)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    uint32_t arr, psc;
    uint32_t RCC_APBxPeriph_TIMx;
    IRQn_Type TIMx_IRQn;
    TIMERx_Type TIMERx;

    if(!IS_TIM_ALL_PERIPH(TIMx))return;

    if(TIMx == TIM1)
    {
        TIMERx = TIMER1;
        RCC_APBxPeriph_TIMx = RCC_APB2Periph_TIM1;
        TIMx_IRQn = TIM1_UP_IRQn;
    }
    else if(TIMx == TIM2)
    {
        TIMERx = TIMER2;
        RCC_APBxPeriph_TIMx = RCC_APB1Periph_TIM2;
        TIMx_IRQn = TIM2_IRQn;
    }
    else if(TIMx == TIM3)
    {
        TIMERx = TIMER3;
        RCC_APBxPeriph_TIMx = RCC_APB1Periph_TIM3;
        TIMx_IRQn = TIM3_IRQn;
    }
    else if(TIMx == TIM4)
    {
        TIMERx = TIMER4;
        RCC_APBxPeriph_TIMx = RCC_APB1Periph_TIM4;
        TIMx_IRQn = TIM4_IRQn;
    }
#ifdef STM32F10X_HD
    else if(TIMx == TIM5)
    {
        TIMERx = TIMER5;
        RCC_APBxPeriph_TIMx = RCC_APB1Periph_TIM5;
        TIMx_IRQn = TIM5_IRQn;
    }
    else if(TIMx == TIM6)
    {
        TIMERx = TIMER6;
        RCC_APBxPeriph_TIMx = RCC_APB1Periph_TIM6;
        TIMx_IRQn = TIM6_IRQn;
    }
    else if(TIMx == TIM7)
    {
        TIMERx = TIMER5;
        RCC_APBxPeriph_TIMx = RCC_APB1Periph_TIM7;
        TIMx_IRQn = TIM7_IRQn;
    }
    else if(TIMx == TIM8)
    {
        TIMERx = TIMER8;
        RCC_APBxPeriph_TIMx = RCC_APB2Periph_TIM8;
        TIMx_IRQn = TIM8_UP_IRQn;
    }
#endif

    TIMx_Function[TIMERx] = function;//Callback Functions

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
    TIM_DeInit(TIMx);
    if(TIMx == TIM1 || TIMx == TIM8)
    {
        RCC_APB2PeriphClockCmd(RCC_APBxPeriph_TIMx, ENABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(RCC_APBxPeriph_TIMx, ENABLE); //时钟使能
    }
    
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseStructure.TIM_Period = arr - 1; 		//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1; 	//设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    /**********************************设置中断优先级************************************/
    NVIC_InitStructure.NVIC_IRQChannel = TIMx_IRQn;  //TIM中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;  //先占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;  //从优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
  
    TIM_ClearFlag(TIMx, TIM_FLAG_Update);
    TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
    TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);	//使能TIM中断
}

/**
  * @brief  定时中断入口，定时器1
  * @param  无
  * @retval 无
  */
void TIM1_UP_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) 		//检查指定的TIM中断发生与否:TIM 中断源
    {
        if(TIMx_Function[TIMER1]) TIMx_Function[TIMER1]();
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  	//清除TIMx的中断待处理位:TIM 中断源
    }
}

/**
  * @brief  定时中断入口，定时器2
  * @param  无
  * @retval 无
  */
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        if(TIMx_Function[TIMER2]) TIMx_Function[TIMER2]();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

/**
  * @brief  定时中断入口，定时器3
  * @param  无
  * @retval 无
  */
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        if(TIMx_Function[TIMER3]) TIMx_Function[TIMER3]();
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}

/**
  * @brief  定时中断入口，定时器4
  * @param  无
  * @retval 无
  */
void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        if(TIMx_Function[TIMER4]) TIMx_Function[TIMER4]();
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
}

/**
  * @brief  定时中断入口，定时器5
  * @param  无
  * @retval 无
  */
void TIM5_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
        if(TIMx_Function[TIMER5]) TIMx_Function[TIMER5]();
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    }
}

/**
  * @brief  定时中断入口，定时器6
  * @param  无
  * @retval 无
  */
void TIM6_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    {
        if(TIMx_Function[TIMER6]) TIMx_Function[TIMER6]();
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    }
}

/**
  * @brief  定时中断入口，定时器7
  * @param  无
  * @retval 无
  */
void TIM7_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
    {
        if(TIMx_Function[TIMER7]) TIMx_Function[TIMER7]();
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    }
}

/**
  * @brief  定时中断入口，定时器8
  * @param  无
  * @retval 无
  */
void TIM8_UP_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)
    {
        if(TIMx_Function[TIMER8]) TIMx_Function[TIMER8]();
        TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
    }
}
