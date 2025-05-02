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
#include "delay.h"

/*根据当前时钟频率自动计算Systick初始值*/
#define SysTick_LoadValue (F_CPU / 1000U)

/*系统时钟计时变量*/
volatile static uint32_t System_ms = 0;

/**
  * @brief  初始化内部RC作为时钟源
  * @param  无
  * @retval 无
  */
void InternalClocks_Init(void)
{
    /*@48MHz*/
    RCC_HSICmd(ENABLE);
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
    while(RCC_GetSYSCLKSource());
    RCC_PLLCmd(DISABLE);
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);
    RCC_PLLCmd(ENABLE);
    while(!RCC_GetFlagStatus(RCC_FLAG_PLLRDY));
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    RCC_PCLKConfig(RCC_SYSCLK_Div1);
}

/**
  * @brief  系统滴答定时器初始化，定时1ms
  * @param  无
  * @retval 无
  */
void Delay_Init(void)
{
    SystemCoreClockUpdate();
    SysTick_Config(SysTick_LoadValue);
    NVIC_SetPriority(SysTick_IRQn, 0);
}

/**
  * @brief  系统滴答定时器中断入口
  * @param  无
  * @retval 无
  */
void SysTick_Handler(void)
{
    System_ms++;
}

/**
  * @brief  获取单片机自上电以来经过的毫秒数
  * @param  无
  * @retval 当前系统时钟毫秒数
  */
uint32_t millis(void)
{
    return System_ms;
}

/**
  * @brief  获取单片机自上电以来经过的微秒数
  * @param  无
  * @retval 当前系统时钟微秒数
  */
uint32_t micros(void)
{
    return (System_ms * 1000 + (SysTick_LoadValue - SysTick->VAL) / CYCLES_PER_MICROSECOND);
}

/**
  * @brief  毫秒级延时
  * @param  ms: 要延时的毫秒数
  * @retval 无
  */
void delay_ms(uint32_t ms)
{
    uint32_t Stop_TimePoint = System_ms + ms;
    while(System_ms < Stop_TimePoint);
}

/**
  * @brief  微秒级延时
  * @param  us: 要延时的微秒数
  * @retval 无
  */
void delay_us(uint32_t us)
{
    uint32_t total = 0;
    uint32_t target = CYCLES_PER_MICROSECOND * us;
    int last = SysTick->VAL;
    int now = last;
    int diff = 0;
start:
    now = SysTick->VAL;
    diff = last - now;
    if(diff > 0)
    {
        total += diff;
    }
    else
    {
        total += diff + SysTick_LoadValue;
    }
    if(total > target)
    {
        return;
    }
    last = now;
    goto start;
}
