/*
 * MIT License
 * Copyright (c) 2017 - 2022 _VIFEXTech
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
#include "gpio.h"

typedef enum
{
    TIMER1, TIMER3, TIMER6,
    TIMER14, TIMER15, TIMER16, TIMER17,
    TIMER_MAX
} TIMER_Type;

static Timer_CallbackFunction_t Timer_CallbackFunction[TIMER_MAX] = { 0 };

/**
  * @brief  启动或关闭指定定时器的时钟
  * @param  TIMx:定时器地址
  * @param  Enable: true启动，false关闭
  * @retval 无
  */
void Timer_ClockCmd(tmr_type* TIMx, bool Enable)
{
    int index;
    typedef struct
    {
        tmr_type* tmr;
        crm_periph_clock_type crm_periph_clock;
    } crm_tmr_clock_map_t;

#   define CLOCK_MAP_DEF(n) {TMR##n, CRM_TMR##n##_PERIPH_CLOCK}

    static const crm_tmr_clock_map_t clock_map[] =
    {
        CLOCK_MAP_DEF(1),
        CLOCK_MAP_DEF(3),
        CLOCK_MAP_DEF(6),
        CLOCK_MAP_DEF(14),
        CLOCK_MAP_DEF(15),
        CLOCK_MAP_DEF(16),
        CLOCK_MAP_DEF(17),
    };

    for(index = 0; index < sizeof(clock_map) / sizeof(crm_tmr_clock_map_t); index++)
    {
        if(TIMx == clock_map[index].tmr)
        {
            crm_periph_clock_enable(clock_map[index].crm_periph_clock, Enable ? TRUE : FALSE);
            break;
        }
    }
}

static float Qsqrt(float number)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5f;
    x2 = number * 0.5f;
    y  = number;
    i  = *(long*)&y;
    i  = 0x5f3759df - (i >> 1);
    y  = *(float*)&i;
    y  = y * (threehalfs - (x2 * y * y));
    y  = y * (threehalfs - (x2 * y * y));
    return 1.0f / y;
}

/**
  * @brief  频率因数分解，获取接近的值
  * @param  freq: 中断频率(Hz)
  * @param  clock: 定时器时钟
  * @param  *period: 重装值地址
  * @param  *prescaler: 时钟分频值地址
  * @retval 误差值(Hz)
  */
static bool Timer_FreqFactorization(
    uint32_t freq,
    uint32_t clock,
    uint16_t* factor1,
    uint16_t* factor2,
    int32_t* error
)
{
    uint32_t targetProdect;
    uint16_t fct1;
    uint16_t fct2;
    uint16_t fct1_save = 1;
    uint16_t fct2_save = 1;
    uint16_t fct_max;
    uint16_t max_error = 0xFFFF;

    if(freq == 0 || freq > clock)
    {
        return false;
    }

    /* 获取目标乘积 */
    targetProdect = clock / freq;

    /* 从targetProdect的平方根开始计算 */
    fct1 = Qsqrt(targetProdect);

    /* 计算因数最大值，减少遍历次数 */
    fct_max = (targetProdect < 0xFFFF) ? targetProdect : 0xFFFF;

    /* 遍历，使两因数的乘积足够接近prodect */
    for(; fct1 > 1; fct1--)
    {
        for(fct2 = fct1; fct2 < fct_max; fct2++)
        {
            /* 求误差 */
            int32_t newerr = fct1 * fct2 - targetProdect;

            if(newerr < 0)
            {
                newerr = -newerr;
            }

            if(newerr < max_error)
            {
                /* 保存最小误差 */
                max_error = (uint16_t)newerr;

                fct1_save = fct1;
                fct2_save = fct2;

                /* 最佳 */
                if(max_error == 0)
                {
                    break;
                }
            }
        }
    }

    *factor1 = fct1_save;
    *factor2 = fct2_save;
    *error = (freq - clock / (fct1_save * fct2_save));

    return true;
}

/**
  * @brief  将定时中断时间转换为重装值和时钟分频值
  * @param  time: 中断时间(微秒)
  * @param  clock: 定时器时钟
  * @param  *period: 重装值地址
  * @param  *prescaler: 时钟分频值地址
  * @retval 无
  */
static void Timer_TimeFactorization(
    uint32_t time,
    uint32_t clock,
    uint16_t* factor1,
    uint16_t* factor2
)
{
    const uint32_t cyclesPerMicros = clock / 1000000U;
    const uint32_t prodect = time * cyclesPerMicros;
    uint16_t fct1, fct2;

    if(prodect < cyclesPerMicros * 30)
    {
        fct1 = 10;
        fct2 = prodect / 10;
    }
    else if(prodect < 65535 * 1000)
    {
        fct1 = prodect / 1000;
        fct2 = prodect / fct1;
    }
    else
    {
        fct1 = prodect / 20000;
        fct2 = prodect / fct1;
    }
    *factor1 = fct1;
    *factor2 = fct2;
}

/**
  * @brief  定时器使能
  * @param  TIMx: 定时器地址
  * @param  Enable: 使能
  * @retval 无
  */
void Timer_SetEnable(tmr_type* TIMx, bool Enable)
{
    tmr_counter_enable(TIMx, Enable ? TRUE : FALSE);
}

/**
  * @brief  定时中断配置
  * @param  TIMx:定时器地址
  * @param  Time: 中断时间(微秒)
  * @param  Function: 定时中断回调函数
  * @retval 无
  */
void Timer_SetInterrupt(tmr_type* TIMx, uint32_t Time, Timer_CallbackFunction_t Function)
{
    uint16_t period = 0;
    uint16_t prescaler = 0;
    uint32_t clock = Timer_GetClockMax(TIMx);

    if(Time == 0)
    {
        return;
    }

    /*将定时中断时间转换为重装值和时钟分频值*/
    Timer_TimeFactorization(
        Time,
        clock,
        &period,
        &prescaler
    );

    /*定时中断配置*/
    Timer_SetInterruptBase(
        TIMx,
        period,
        prescaler,
        Function,
        TIMER_PREEMPTIONPRIORITY_DEFAULT,
        TIMER_SUBPRIORITY_DEFAULT
    );
}

/**
  * @brief  更新定时中断频率
  * @param  TIMx:定时器地址
  * @param  Freq:中断频率
  * @retval true: 设置成功
  */
bool Timer_SetInterruptFreqUpdate(tmr_type* TIMx, uint32_t Freq)
{
    uint16_t period, prescaler;
    uint32_t clock = Timer_GetClockMax(TIMx);
    int32_t error;
    bool success;

    if(Freq == 0)
        return false;

    success = Timer_FreqFactorization(
                       Freq,
                       clock,
                       &period,
                       &prescaler,
                       &error
                   );

    if(!success)
    {
        return false;
    }

    tmr_base_init(TIMx, period - 1, prescaler - 1);
    return true;
}

/**
  * @brief  获取定时器最大频率
  * @param  TIMx:定时器地址
  * @retval 最大频率
  */
uint32_t Timer_GetClockMax(tmr_type* TIMx)
{
    static crm_clocks_freq_type crm_clocks_freq_struct = {0};
    if(!crm_clocks_freq_struct.sclk_freq)
    {
        crm_clocks_freq_get(&crm_clocks_freq_struct);
    }

    if(TIMx == TIM3 || TIMx == TIM6 || TIMx == TIM14)
    {
        return crm_clocks_freq_struct.apb1_freq;
    }

    if(TIMx == TIM1 || TIMx == TIM15 || TIMx == TIM16 || TIMx == TIM17)
    {
        return crm_clocks_freq_struct.apb2_freq;
    }

    return 0;
}

/**
  * @brief  获取定时器中断频率
  * @param  TIMx:定时器地址
  * @retval 中断频率
  */
uint32_t Timer_GetClockOut(tmr_type* TIMx)
{
    uint32_t clock = Timer_GetClockMax(TIMx);

    return (clock / ((TIMx->pr + 1) * (TIMx->div + 1)));
}

/**
  * @brief  更新定时中断时间
  * @param  TIMx:定时器地址
  * @param  Time: 中断时间(微秒)
  * @retval 无
  */
void Timer_SetInterruptTimeUpdate(tmr_type* TIMx, uint32_t Time)
{
    uint16_t period, prescaler;
    uint32_t clock = Timer_GetClockMax(TIMx);

    Timer_TimeFactorization(
        Time,
        clock,
        &period,
        &prescaler
    );

    tmr_base_init(TIMx, period - 1, prescaler - 1);
}

/**
  * @brief  定时中断基本配置
  * @param  TIMx:定时器地址
  * @param  Period:重装值
  * @param  Prescaler:时钟分频值
  * @param  Function: 定时中断回调函数
  * @param  PreemptionPriority: 抢占优先级
  * @param  SubPriority: 子优先级
  * @retval 无
  */
void Timer_SetInterruptBase(
    tmr_type* TIMx,
    uint16_t Period,
    uint16_t Prescaler,
    Timer_CallbackFunction_t Function,
    uint8_t PreemptionPriority,
    uint8_t SubPriority
)
{
    IRQn_Type TMRx_IRQn = (IRQn_Type)0;
    TIMER_Type TIMERx = TIMER1;

#define TMRx_IRQ_DEF(n,x_IRQn)\
do{\
    if(TIMx == TIM##n)\
    {\
        TIMERx = TIMER##n;\
        TMRx_IRQn = x_IRQn;\
        goto match;\
    }\
}\
while(0)

    /*如果编译器提示：identifier "xxx_IRQn" is undefined
     *把未定义的注释掉即可
     */
    TMRx_IRQ_DEF(1, TMR1_BRK_OVF_TRG_HALL_IRQn);
    TMRx_IRQ_DEF(3, TMR3_GLOBAL_IRQn);
    TMRx_IRQ_DEF(6, TMR6_GLOBAL_IRQn);
    TMRx_IRQ_DEF(14, TMR14_GLOBAL_IRQn);
    TMRx_IRQ_DEF(15, TMR14_GLOBAL_IRQn);
    TMRx_IRQ_DEF(16, TMR14_GLOBAL_IRQn);
    TMRx_IRQ_DEF(17, TMR14_GLOBAL_IRQn);

match:

    if(TMRx_IRQn == 0)
    {
        return;
    }

    Timer_CallbackFunction[TIMERx] = Function;

    tmr_reset(TIMx);
    Timer_ClockCmd(TIMx, true);

    tmr_repetition_counter_set(TIMx, 0);
    tmr_cnt_dir_set(TIMx, TMR_COUNT_UP);
    tmr_base_init(TIMx, Period - 1, Prescaler - 1);

    nvic_irq_enable(TMRx_IRQn, PreemptionPriority, SubPriority);

    tmr_flag_clear(TIMx, TMR_OVF_FLAG);
    tmr_interrupt_enable(TIMx, TMR_OVF_INT, TRUE);
}

/**
  * @brief  设置输出比较值
  * @param  TIMx: 定时器地址
  * @param  TimerChannel: 定时器通道
  * @param  Compare:输出比较值
  * @retval 无
  */
void Timer_SetCompare(tmr_type* TIMx, uint8_t TimerChannel, uint32_t Compare)
{
    switch(TimerChannel)
    {
    case 1:
        tmr_channel_value_set(TIMx, TMR_SELECT_CHANNEL_1, Compare);
        break;
    case 2:
        tmr_channel_value_set(TIMx, TMR_SELECT_CHANNEL_2, Compare);
        break;
    case 3:
        tmr_channel_value_set(TIMx, TMR_SELECT_CHANNEL_3, Compare);
        break;
    case 4:
        tmr_channel_value_set(TIMx, TMR_SELECT_CHANNEL_4, Compare);
        break;
    default:
        break;
    }
}

/**
  * @brief  获取捕获值
  * @param  TIMx: 定时器地址
  * @param  TimerChannel: 定时器通道
  * @retval 捕获值
  */
uint16_t Timer_GetCompare(tmr_type* TIMx, uint8_t TimerChannel)
{
    uint16_t retval = 0;
    switch(TimerChannel)
    {
    case 1:
        retval = tmr_channel_value_get(TIMx, TMR_SELECT_CHANNEL_1);
        break;
    case 2:
        retval = tmr_channel_value_get(TIMx, TMR_SELECT_CHANNEL_2);
        break;
    case 3:
        retval = tmr_channel_value_get(TIMx, TMR_SELECT_CHANNEL_3);
        break;
    case 4:
        retval = tmr_channel_value_get(TIMx, TMR_SELECT_CHANNEL_4);
        break;
    default:
        break;
    }

    return retval;
}

/**
  * @brief  更新定时器时钟预分频数
  * @param  TIMx: 定时器地址
  * @param  rescaler: 时钟预分频数
  * @retval 无
  */
void Timer_SetPrescaler(tmr_type* TIMx, uint16_t rescaler)
{
    TIMx->div = rescaler;
}

/**
  * @brief  更新定时器自动重装值
  * @param  TIMx: 定时器地址
  * @param  Reload: 自动重装值
  * @retval 无
  */
void Timer_SetReload(tmr_type* TIMx, uint16_t Reload)
{
    TIMx->pr = Reload;
}

/**
  * @brief  应用定时器更改
  * @param  TIMx: 定时器地址
  * @retval 无
  */
void Timer_GenerateUpdate(tmr_type* TIMx)
{
    TIMx->swevt_bit.ovfswtr = TRUE;
}

/**
  * @brief  获取引脚对应的定时器复用编号
  * @param  Pin: 引脚编号
  * @retval 定时器复用编号
  */
gpio_mux_sel_type Timer_GetGPIO_MUX(uint8_t Pin)
{
    gpio_mux_sel_type GPIO_MUX_x = GPIO_MUX_0;
    tmr_type* TIMx = PIN_MAP[Pin].TIMx;
    uint8_t TimerChannel = PIN_MAP[Pin].TimerChannel;

#define TIMER_GPIO_MUX_DEF(mux, timx, channel) \
    else if(TIMx == timx && TimerChannel == channel) \
        GPIO_MUX_x = GPIO_MUX_##mux

    if(0) {}

    /* GPIO_MUX_0 */
    TIMER_GPIO_MUX_DEF(0, TIM15, 1);
    TIMER_GPIO_MUX_DEF(0, TIM15, 2);
    TIMER_GPIO_MUX_DEF(0, TIM14, 1);

    /* GPIO_MUX_1 */
    TIMER_GPIO_MUX_DEF(1, TIM3, 1);
    TIMER_GPIO_MUX_DEF(1, TIM3, 2);
    TIMER_GPIO_MUX_DEF(1, TIM3, 3);
    TIMER_GPIO_MUX_DEF(1, TIM3, 4);

    /* GPIO_MUX_2 */
    TIMER_GPIO_MUX_DEF(2, TIM1, 1);
    TIMER_GPIO_MUX_DEF(2, TIM1, 2);
    TIMER_GPIO_MUX_DEF(2, TIM1, 3);
    TIMER_GPIO_MUX_DEF(2, TIM1, 4);

    /* GPIO_MUX_4 */
    TIMER_GPIO_MUX_DEF(4, TIM14, 1);

    return GPIO_MUX_x;
}

#define TMRx_IRQHANDLER(n)                              \
do{                                                     \
    if (tmr_flag_get(TMR##n, TMR_OVF_FLAG) != RESET)    \
    {                                                   \
        if(Timer_CallbackFunction[TIMER##n])            \
        {                                               \
            Timer_CallbackFunction[TIMER##n]();         \
        }                                               \
        tmr_flag_clear(TMR##n, TMR_OVF_FLAG);           \
    }                                                   \
}while(0)

/**
  * @brief  定时中断入口
  * @param  无
  * @retval 无
  */
void TMR1_BRK_OVF_TRG_HALL_IRQHandler(void)
{
    TMRx_IRQHANDLER(1);
}


void TMR3_GLOBAL_IRQHandler(void)
{
    TMRx_IRQHANDLER(3);
}

void TMR6_GLOBAL_IRQHandler(void)
{
    TMRx_IRQHANDLER(6);
}

void TMR14_GLOBAL_IRQHandler(void)
{
    TMRx_IRQHANDLER(14);
}

void TMR15_GLOBAL_IRQHandler(void)
{
    TMRx_IRQHANDLER(15);
}

void TMR16_GLOBAL_IRQHandler(void)
{
    TMRx_IRQHANDLER(16);
}

void TMR17_GLOBAL_IRQHandler(void)
{
    TMRx_IRQHANDLER(17);
}
