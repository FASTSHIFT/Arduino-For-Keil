#include "delay.h"

#define SysTick_LoadValue (F_CPU / 1000U) //根据当前时钟频率自动计算Systick初始值

volatile static uint32_t System_ms = 0; //系统时钟计时变量
static SysClock_TypeDef SysClock[] = {
    {16000000, RCC_PLLMul_2},//16MHz, 2倍频
    {24000000, RCC_PLLMul_3},//24MHz, 3倍频
    {32000000, RCC_PLLMul_4},//32MHz, 4倍频
    {40000000, RCC_PLLMul_5},//40MHz, 5倍频
    {48000000, RCC_PLLMul_6},//48MHz, 6倍频
    {56000000, RCC_PLLMul_7},//56MHz, 7倍频
    {64000000, RCC_PLLMul_8},//64MHz, 8倍频
    {72000000, RCC_PLLMul_9},//72MHz, 9倍频
    {80000000, RCC_PLLMul_10},//80MHz, 10倍频
    {88000000, RCC_PLLMul_11},//88MHz, 11倍频
    {96000000, RCC_PLLMul_12},//96MHz, 12倍频
    {104000000, RCC_PLLMul_13},//104MHz, 13倍频
    {112000000, RCC_PLLMul_14},//112MHz, 14倍频
    {120000000, RCC_PLLMul_15},//120MHz, 15倍频
    {128000000, RCC_PLLMul_16},//128MHz, 16倍频
};

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
  * @brief  系统时钟配置
  * @param  fcpu:主频值 (详见F_CPU_Type枚举)
  * @retval 无
  */
void SysClock_Init(F_CPU_Type fcpu)
{
    SystemCoreClock = SysClock[fcpu].F_CPU_x;
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
    while(RCC_GetSYSCLKSource());
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    RCC_PCLK1Config(RCC_HCLK_Div2);
    RCC_PCLK2Config(RCC_HCLK_Div1);
    RCC_PLLCmd(DISABLE);
    RCC_HSEConfig(RCC_HSE_ON);
    RCC_WaitForHSEStartUp();
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, SysClock[fcpu].RCC_PLLMul_x);
    RCC_PLLCmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) ;
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while(RCC_GetSYSCLKSource() != 0x08);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
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
