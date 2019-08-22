#include "pwm.h"
#include "timer.h"
#include "Arduino.h"

/**
  * @brief  定时器输出捕获初始化
  * @param  TIMx: 定时器地址
  * @param  arr: 自动重装值
  * @param  psc: 时钟预分频数
  * @param  TimerChannel: 定时器通道
  * @retval 无
  */
void TIMx_OCxInit(TIM_TypeDef* TIMx, uint16_t arr, uint16_t psc, uint8_t TimerChannel)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    if(!IS_TIM_ALL_PERIPH(TIMx))
        return;
    
    TimerClockCmd(TIMx, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    switch(TimerChannel)
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
    
    if(IS_APB2_TIM(TIMx))
    {
        TIM_CtrlPWMOutputs(TIMx, ENABLE);
    }
}

/**
  * @brief  PWM输出初始化
  * @param  Pin:引脚编号
  * @param  PWM_DutyCycle: PWM分级数
  * @param  PWM_Frequency: PWM频率
  * @retval 引脚对应的定时器通道
  */
uint8_t PWM_Init(uint8_t Pin, uint16_t PWM_DutyCycle, uint32_t PWM_Frequency)
{
    uint32_t arr, psc;
    
    if(!IS_PWM_PIN(Pin))
        return 0;
    
    if(PWM_DutyCycle == 0 || PWM_Frequency == 0 || (PWM_DutyCycle * PWM_Frequency) > F_CPU)
        return 0;

    pinMode(Pin, OUTPUT_AF);
    GPIO_PinAFConfig(PIN_MAP[Pin].GPIOx, Get_TIM_PinSourcex(Pin), Get_TIMx_GPIO_AF_x(Pin));

    arr = PWM_DutyCycle;
    psc = F_CPU / PWM_DutyCycle / PWM_Frequency;
    
    if(!IS_APB2_TIM(PIN_MAP[Pin].TIMx))
        psc /= 2;

    TIM_Cmd(PIN_MAP[Pin].TIMx, DISABLE);
    TIMx_OCxInit(PIN_MAP[Pin].TIMx, arr - 1, psc - 1, PIN_MAP[Pin].TimerChannel);
    return PIN_MAP[Pin].TimerChannel;
}

/**
  * @brief  获取引脚对应的定时器复用编号
  * @param  Pin: 引脚编号
  * @retval 定时器复用编号
  */
uint8_t Get_TIMx_GPIO_AF_x(uint8_t Pin)
{
    uint8_t GPIO_AF_x = 0;
    TIM_TypeDef* TIMx = PIN_MAP[Pin].TIMx;

    switch((int)TIMx)
    {
    case (int)TIM1:
        GPIO_AF_x = GPIO_AF_TIM1;
        break;
    case (int)TIM2:
        GPIO_AF_x = GPIO_AF_TIM2;
        break;
    case (int)TIM3:
        GPIO_AF_x = GPIO_AF_TIM3;
        break;
    case (int)TIM4:
        GPIO_AF_x = GPIO_AF_TIM4;
        break;
    case (int)TIM5:
        GPIO_AF_x = GPIO_AF_TIM5;
        break;
#if defined (GPIO_AF_TIM6)
    case (int)TIM6:
        GPIO_AF_x = GPIO_AF_TIM6;
        break;
#endif
#if defined (GPIO_AF_TIM7)
    case (int)TIM7:
        GPIO_AF_x = GPIO_AF_TIM7;
        break;
#endif
#if defined (GPIO_AF_TIM8)
    case (int)TIM8:
        GPIO_AF_x = GPIO_AF_TIM8;
        break;
#endif
#if defined (GPIO_AF_TIM9)
    case (int)TIM9:
        GPIO_AF_x = GPIO_AF_TIM9;
        break;
#endif
#if defined (GPIO_AF_TIM10)
    case (int)TIM10:
        GPIO_AF_x = GPIO_AF_TIM10;
        break;
#endif
#if defined (GPIO_AF_TIM11)
    case (int)TIM11:
        GPIO_AF_x = GPIO_AF_TIM11;
        break;
#endif
#if defined (GPIO_AF_TIM12)
    case (int)TIM12:
        GPIO_AF_x = GPIO_AF_TIM12;
        break;
#endif
#if defined (GPIO_AF_TIM13)
    case (int)TIM13:
        GPIO_AF_x = GPIO_AF_TIM13;
        break;
#endif
#if defined (GPIO_AF_TIM14)
    case (int)TIM14:
        GPIO_AF_x = GPIO_AF_TIM14;
        break;
#endif
    default:
        GPIO_AF_x = 0;
        break;
    }

    return GPIO_AF_x;
}

/**
  * @brief  输出PWM信号
  * @param  Pin: 引脚编号
  * @param  val:PWM占空比值
  * @retval PWM占空比值
  */
uint16_t pwmWrite(uint8_t Pin, uint16_t val)
{
    switch(PIN_MAP[Pin].TimerChannel)
    {
    case 1:
        PIN_MAP[Pin].TIMx->CCR1 = val;
        break;
    case 2:
        PIN_MAP[Pin].TIMx->CCR2 = val;
        break;
    case 3:
        PIN_MAP[Pin].TIMx->CCR3 = val;
        break;
    case 4:
        PIN_MAP[Pin].TIMx->CCR4 = val;
        break;
    }
    return val;
}

/**
  * @brief  获取捕获值
  * @param  TIMx: 定时器地址
  * @param  TimerChannel: 定时器通道
  * @retval 捕获值
  */
uint16_t timer_get_compare(TIM_TypeDef* TIMx, uint8_t TimerChannel)
{
    uint16_t compare = 0;
    switch(TimerChannel)
    {
    case 1:
        compare = TIMx->CCR1;
        break;
    case 2:
        compare = TIMx->CCR2;
        break;
    case 3:
        compare = TIMx->CCR3;
        break;
    case 4:
        compare = TIMx->CCR4;
        break;
    }
    return compare;
}

/**
  * @brief  更新定时器时钟预分频数
  * @param  TIMx: 定时器地址
  * @param  psc: 时钟预分频数
  * @retval 无
  */
void timer_set_prescaler(TIM_TypeDef* TIMx, uint16_t psc)
{
    TIMx->PSC = psc;
}

/**
  * @brief  更新定时器自动重装值
  * @param  TIMx: 定时器地址
  * @param  arr: 自动重装值
  * @retval 无
  */
void timer_set_reload(TIM_TypeDef* TIMx, uint16_t arr)
{
    TIMx->ARR = arr;
}

/**
  * @brief  应用定时器更改
  * @param  TIMx: 定时器地址
  * @retval 无
  */
void timer_generate_update(TIM_TypeDef* TIMx)
{
    TIMx->EGR = TIM_PSCReloadMode_Immediate;
}
