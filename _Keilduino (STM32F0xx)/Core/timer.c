#include "timer.h"
#include "Arduino.h"

/*定时器编号枚举*/
typedef enum
{
    TIMER1, TIMER2, TIMER3, TIMER6,
    TIMER14, TIMER15, TIMER16, TIMER17,
    TIMER_MAX
} TIMERx_Type;

/*定时中断回调函数指针数组*/
static Timer_CallbackFunction_t TIM_Function[TIMER_MAX] = {0};

/**
  * @brief  启动或关闭指定定时器的时钟
  * @param  TIMx:定时器地址
  * @param  NewState: ENABLE启动，DISABLE关闭
  * @retval 无
  */
void TimerClockCmd(TIM_TypeDef* TIMx, FunctionalState NewState)
{
    if (TIMx == TIM1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, NewState);
    }
    else if (TIMx == TIM2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, NewState);
    }
    else if (TIMx == TIM3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, NewState);
    }
    else if (TIMx == TIM6)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, NewState);
    }
    else if (TIMx == TIM14)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, NewState);
    }
    else if (TIMx == TIM15)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, NewState);
    }
    else if (TIMx == TIM16)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, NewState);
    }
    else
    {
        if (TIMx == TIM17)
        {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, NewState);
        }
    }
}

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
        Timer_ChannelPriority_Default
    );
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
void Timer_Init(TIM_TypeDef* TIMx, uint32_t InterruptTime_us, Timer_CallbackFunction_t function, uint8_t ChannelPriority)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    uint32_t arr, psc;
    uint8_t TIMx_IRQn;
    TIMERx_Type TIMERx;

    if(!IS_TIM_ALL_PERIPH(TIMx))
        return;

    if(TIMx == TIM1)
    {
        TIMERx = TIMER1;
        TIMx_IRQn = TIM1_BRK_UP_TRG_COM_IRQn;
    }
    else if(TIMx == TIM2)
    {
        TIMERx = TIMER2;
        TIMx_IRQn = TIM2_IRQn;
    }
    else if(TIMx == TIM3)
    {
        TIMERx = TIMER3;
        TIMx_IRQn = TIM3_IRQn;
    }
    else if(TIMx == TIM6)
    {
        TIMERx = TIMER6;
        TIMx_IRQn = TIM6_DAC_IRQn;
    }
    else if(TIMx == TIM14)
    {
        TIMERx = TIMER14;
        TIMx_IRQn = TIM14_IRQn;
    }
    else if(TIMx == TIM15)
    {
        TIMERx = TIMER15;
        TIMx_IRQn = TIM15_IRQn;
    }
    else if(TIMx == TIM16)
    {
        TIMERx = TIMER16;
        TIMx_IRQn = TIM16_IRQn;
    }
    else if(TIMx == TIM17)
    {
        TIMERx = TIMER17;
        TIMx_IRQn = TIM17_IRQn;
    }

    /*register callback function*/
    TIM_Function[TIMERx] = function;//Callback Functions

    /*Calculate TIM_Period and TIM_Prescaler*/
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
    TimerClockCmd(TIMx, ENABLE);

    TIM_DeInit(TIMx);
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseStructure.TIM_Period = arr - 1;                 //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;              //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);             //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    /**********************************设置中断优先级************************************/
    NVIC_InitStructure.NVIC_IRQChannel = TIMx_IRQn;  //TIM中断
    NVIC_InitStructure.NVIC_IRQChannelPriority = ChannelPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    TIM_ClearFlag(TIMx, TIM_FLAG_Update);
    TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);  //使能TIM中断
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

    if(!IS_TIM_ALL_PERIPH(TIMx))
        return;

    /*Calculate TIM_Period and TIM_Prescaler*/
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
  * @brief  定时中断入口，定时器1
  * @param  无
  * @retval 无
  */
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        TIM_Function[TIMER1]();
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
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
        TIM_Function[TIMER2]();
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
        TIM_Function[TIMER3]();
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}

/**
  * @brief  定时中断入口，定时器6
  * @param  无
  * @retval 无
  */
void TIM6_DAC_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    {
        TIM_Function[TIMER6]();
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    }
}

/**
  * @brief  定时中断入口，定时器14
  * @param  无
  * @retval 无
  */
void TIM14_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)
    {
        TIM_Function[TIMER14]();
        TIM_ClearITPendingBit(TIM14, TIM_IT_Update);
    }
}

/**
  * @brief  定时中断入口，定时器15
  * @param  无
  * @retval 无
  */
void TIM15_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM15, TIM_IT_Update) != RESET)
    {
        TIM_Function[TIMER15]();
        TIM_ClearITPendingBit(TIM15, TIM_IT_Update);
    }
}

/**
  * @brief  定时中断入口，定时器16
  * @param  无
  * @retval 无
  */
void TIM16_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM16, TIM_IT_Update) != RESET)
    {
        TIM_Function[TIMER16]();
        TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
    }
}

/**
  * @brief  定时中断入口，定时器17
  * @param  无
  * @retval 无
  */
void TIM17_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM17, TIM_IT_Update) != RESET)
    {
        TIM_Function[TIMER17]();
        TIM_ClearITPendingBit(TIM17, TIM_IT_Update);
    }
}
