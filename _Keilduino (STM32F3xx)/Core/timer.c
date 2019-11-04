/*
 * MIT License
 * Copyright (c) 2019 _VIFEXTech
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "timer.h"

/*定时器编号枚举*/
typedef enum
{
    TIMER1, TIMER2, TIMER3, TIMER4, TIMER6, TIMER7, TIMER8,
    TIMER15, TIMER16, TIMER17, TIMER_MAX
} TIMER_Type;

/*定时中断回调函数指针数组*/
Timer_CallbackFunction_t TIM_Function[TIMER_MAX] = {0};

/**
  * @brief  启动或关闭指定定时器的时钟
  * @param  TIMx:定时器地址
  * @param  NewState: ENABLE启动，DISABLE关闭
  * @retval 无
  */
void TimerClockCmd(TIM_TypeDef* TIMx, FunctionalState NewState)
{
    if(!IS_TIM_ALL_PERIPH(TIMx))
        return;

    if(TIMx == TIM1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, NewState);
    }
    else if(TIMx == TIM2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, NewState);
    }
    else if(TIMx == TIM3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, NewState);
    }
    else if(TIMx == TIM4)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, NewState);
    }
    else if(TIMx == TIM6)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, NewState);
    }
    else if(TIMx == TIM7)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, NewState);
    }
    else if(TIMx == TIM8)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, NewState);
    }
    else if (TIMx == TIM15)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);
    }
    else if (TIMx == TIM16)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);
    }
    else if (TIMx == TIM17)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);
    }
    else
    {
        if (TIMx == TIM20)
        {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM20, ENABLE);
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
    Timer_Init(TIMx, InterruptTime_us, function, Timer_PreemptionPriority_Default, Timer_SubPriority_Default);
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
    uint8_t TIMx_IRQn;
    TIMER_Type TIMERx;

    if(!IS_TIM_ALL_PERIPH(TIMx))
        return;

    if(TIMx == TIM1)
    {
        TIMERx = TIMER1;
        TIMx_IRQn = TIM1_BRK_TIM15_IRQn;
    }

    else if(TIMx == TIM2)
    {
        TIMERx = TIMER2;
        TIMx_IRQn = TIM2_IRQn;
    }
//    else if(TIMx == TIM3)
//    {
//        TIMERx = TIMER3;
//        TIMx_IRQn = TIM3_IRQn;
//    }
    else if(TIMx == TIM15)
    {
        TIMERx = TIMER15;
        TIMx_IRQn = TIM1_BRK_TIM15_IRQn;
    }
    else if(TIMx == TIM16)
    {
        TIMERx = TIMER16;
        TIMx_IRQn = TIM1_UP_TIM16_IRQn;
    }
    else if(TIMx == TIM17)
    {
        TIMERx = TIMER17;
        TIMx_IRQn = TIM1_TRG_COM_TIM17_IRQn;
    }

    TIM_Function[TIMERx] = function;//Callback Functions

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
    TimerClockCmd(TIMx, ENABLE);

//    TIM_DeInit(TIMx);
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseStructure.TIM_Period = arr - 1;         //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;  //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    /**********************************设置中断优先级************************************/
    NVIC_InitStructure.NVIC_IRQChannel = TIMx_IRQn;  //TIM中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;  //先占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;  //从优先级
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
void TIM1_BRK_TIM15_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        TIM_Function[TIMER1]();
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_Function[TIMER2]();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_Function[TIMER3]();
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
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
