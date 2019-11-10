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
    TIMER1, TIMER2, TIMER3, TIMER4, TIMER5, TIMER6, TIMER7, TIMER8,
    TIMER9, TIMER10, TIMER11, TIMER12, TIMER13, TIMER14, TIMER15, 
    TIMER_MAX
} TIMER_Type;

/*定时中断回调函数指针数组*/
static Timer_CallbackFunction_t TIMx_Function[TIMER_MAX] = {0};

/**
  * @brief  启动或关闭指定定时器的时钟
  * @param  TIMx:定时器地址
  * @param  NewState: ENABLE启动，DISABLE关闭
  * @retval 无
  */
void TimerClockCmd(TIM_TypeDef* TIMx, FunctionalState NewState)
{
    if(TIMx == TIM1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_TMR1, NewState);
    }
    else if(TIMx == TIM2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR2, NewState);
    }
    else if(TIMx == TIM3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR3, NewState);
    }
    else if(TIMx == TIM4)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR4, NewState);
    }
    else if(TIMx == TIM5)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR5, NewState);
    }
    else if(TIMx == TIM6)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR6, NewState);
    }
    else if(TIMx == TIM7)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR7, NewState);
    }
    else if(TIMx == TIM8)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_TMR8, NewState);
    }
    else if(TIMx == TIM9)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_TMR9, NewState);
    }
    else if(TIMx == TIM10)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_TMR10, NewState);
    }
    else if(TIMx == TIM11)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_TMR11, NewState);
    }
    else if(TIMx == TIM12)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR12, NewState);
    }
    else if(TIMx == TIM13)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR13, NewState);
    }
    else if(TIMx == TIM14)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR14, NewState);
    }
    else if(TIMx == TIM15)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_TMR15, NewState);
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
  * @brief  更新定时中断时间
  * @param  TIMx:定时器地址
  * @param  InterruptTime_us: 中断时间(微秒)
  * @retval 无
  */
void TimerSet_InterruptTimeUpdate(TIM_TypeDef* TIMx, uint32_t InterruptTime_us)
{
    uint32_t arr, psc;
    
    if(!IS_TMR_ALL_PERIPH(TIMx))
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

//    if(!IS_APB2_TIM(TIMx))
//        psc /= 2;

    TIMx->AR  = arr - 1;
    TIMx->DIV = psc - 1;
    TIMx->EVEG = TMR_DIVReloadMode_Immediate;
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
    TMR_TimerBaseInitType  TMR_TimeBaseStructure;
    NVIC_InitType NVIC_InitStructure;

    uint32_t arr, psc;
    uint8_t TMRx_IRQn;
    TIMER_Type TIMERx;

    if(!IS_TMR_ALL_PERIPH(TIMx))
        return;

#define TMRx_IRQ_DEF(n,x_IRQn)\
do{\
    if(TIMx == TIM##n)\
    {\
        TIMERx = TIMER##n;\
        TMRx_IRQn = x_IRQn;\
    }\
}\
while(0)
    
    TMRx_IRQ_DEF(1, TMR1_OV_TMR10_IRQn);
    TMRx_IRQ_DEF(2, TMR2_GLOBAL_IRQn);
    TMRx_IRQ_DEF(3, TMR3_GLOBAL_IRQn);
    TMRx_IRQ_DEF(4, TMR4_GLOBAL_IRQn);
    TMRx_IRQ_DEF(5, TMR5_GLOBAL_IRQn);
    TMRx_IRQ_DEF(6, TMR6_GLOBAL_IRQn);
    TMRx_IRQ_DEF(7, TMR7_GLOBAL_IRQn);
    TMRx_IRQ_DEF(8, TMR8_OV_TMR13_IRQn);
    TMRx_IRQ_DEF(9, TMR1_BRK_TMR9_IRQn);
    TMRx_IRQ_DEF(10, TMR1_OV_TMR10_IRQn);
    TMRx_IRQ_DEF(11, TMR1_TRG_HALL_TMR11_IRQn);
    TMRx_IRQ_DEF(12, TMR8_BRK_TMR12_IRQn);
    TMRx_IRQ_DEF(13, TMR8_OV_TMR13_IRQn);
    TMRx_IRQ_DEF(14, TMR8_TRG_HALL_TMR14_IRQn);
    TMRx_IRQ_DEF(15, TMR15_OV_IRQn);

    /*register callback function*/
    TIMx_Function[TIMERx] = function;

    /*Enable PeriphClock*/
    TMR_Reset(TIMx);
    TimerClockCmd(TIMx, ENABLE);

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

    if(!IS_APB2_TIM(TIMx))
        psc /= 2;

    TMR_TimeBaseStructure.TMR_RepetitionCounter = 0;
    TMR_TimeBaseStructure.TMR_Period = arr - 1;         //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TMR_TimeBaseStructure.TMR_DIV = psc - 1;  //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
    TMR_TimeBaseStructure.TMR_ClockDivision = TMR_CKD_DIV1;     //设置时钟分割:TDTS = Tck_tim
    TMR_TimeBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;  //TIM向上计数模式
    TMR_TimeBaseInit(TIMx, &TMR_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    
    /**********************************设置中断优先级************************************/
    NVIC_InitStructure.NVIC_IRQChannel = TMRx_IRQn;  //TIM中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;  //先占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;  //从优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    TMR_ClearFlag(TIMx, TMR_FLAG_Update);
    TMR_INTConfig(TIMx, TMR_INT_Overflow, ENABLE);  //使能TIM中断
}

#define TMRx_IRQHANDLER(n) \
do{\
    if (TMR_GetINTStatus(TMR##n, TMR_INT_Overflow) != RESET)\
    {\
        if(TIMx_Function[TIMER##n]) TIMx_Function[TIMER##n]();\
        TMR_ClearITPendingBit(TIM##n, TMR_INT_Overflow);\
    }\
}while(0)

/**
  * @brief  定时中断入口，定时器1
  * @param  无
  * @retval 无
  */
void TMR1_OV_TMR10_IRQHandler(void)
{
    TMRx_IRQHANDLER(1);
    TMRx_IRQHANDLER(10);
}

/**
  * @brief  定时中断入口，定时器2
  * @param  无
  * @retval 无
  */
void TMR2_GLOBAL_IRQHandler(void)
{
    TMRx_IRQHANDLER(2);
}

/**
  * @brief  定时中断入口，定时器3
  * @param  无
  * @retval 无
  */
void TMR3_GLOBAL_IRQHandler(void)
{
    TMRx_IRQHANDLER(3);
}

/**
  * @brief  定时中断入口，定时器4
  * @param  无
  * @retval 无
  */
void TMR4_GLOBAL_IRQHandler(void)
{
    TMRx_IRQHANDLER(4);
}

/**
  * @brief  定时中断入口，定时器5
  * @param  无
  * @retval 无
  */
void TMR5_GLOBAL_IRQHandler(void)
{
    TMRx_IRQHANDLER(5);
}

/**
  * @brief  定时中断入口，定时器6
  * @param  无
  * @retval 无
  */
void TMR6_GLOBAL_IRQHandler(void)
{
    TMRx_IRQHANDLER(6);
}

/**
  * @brief  定时中断入口，定时器7
  * @param  无
  * @retval 无
  */
void TMR7_GLOBAL_IRQHandler(void)
{
    TMRx_IRQHANDLER(7);
}

/**
  * @brief  定时中断入口，定时器8
  * @param  无
  * @retval 无
  */
void TMR8_OV_TMR13_IRQHandler(void)
{
    TMRx_IRQHANDLER(8);
    TMRx_IRQHANDLER(13);
}
