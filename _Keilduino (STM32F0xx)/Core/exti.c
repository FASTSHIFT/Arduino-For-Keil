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
#include "exti.h"
#include "gpio.h"

/*外部中断回调函数指针数组*/
static EXTI_CallbackFunction_t EXTI_Function[16] = {0};

/**
  * @brief  获取外部中断的中断通道
  * @param  Pin: 引脚编号
  * @retval 通道编号
  */
static IRQn_Type EXTI_GetIRQn(uint8_t Pin)
{
    IRQn_Type EXTIx_IRQn;
    uint8_t Pinx = GPIO_GetPinNum(Pin);

    if(Pinx <= 1)
    {
        EXTIx_IRQn = EXTI0_1_IRQn;
    }
    else if(Pinx >= 2 && Pinx <= 3)
    {
        EXTIx_IRQn = EXTI2_3_IRQn;
    }
    else if(Pinx >= 4 && Pinx <= 15)
    {
        EXTIx_IRQn = EXTI4_15_IRQn;
    }
    
    return EXTIx_IRQn;
}

/**
  * @brief  外部中断初始化
  * @param  Pin: 引脚编号
  * @param  function: 回调函数
  * @param  Trigger_Mode: 触发方式
  * @param  ChannelPriority: 通道优先级
  * @retval 无
  */
void EXTIx_Init(uint8_t Pin, EXTI_CallbackFunction_t function, EXTITrigger_TypeDef Trigger_Mode, uint8_t ChannelPriority)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    IRQn_Type EXTIx_IRQn;
    uint8_t Pinx;

    if(!IS_PIN(Pin))
        return;

    Pinx = GPIO_GetPinNum(Pin);

    if(Pinx > 15)
        return;

    EXTI_Function[Pinx] = function;

    //GPIO中断线以及中断初始化配置
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_GetPortSourceGPIOx(Pin), EXTI_GetPinSourcex(Pin));

    EXTI_InitStructure.EXTI_Line = EXTI_GetLine(Pin);//设置中断线
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//设置触发模式，中断触发（事件触发）
    EXTI_InitStructure.EXTI_Trigger = Trigger_Mode;//设置触发方式
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTIx_IRQn;//使能所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPriority = ChannelPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  外部中断初始化
  * @param  Pin: 引脚编号
  * @param  function: 回调函数
  * @param  Trigger_Mode: 触发方式
  * @retval 无
  */
void attachInterrupt(uint8_t Pin, EXTI_CallbackFunction_t function, EXTITrigger_TypeDef Trigger_Mode)
{
    EXTIx_Init(
        Pin, 
        function, 
        Trigger_Mode, 
        EXTI_ChannelPriority_Default
    );
}
/**
  * @brief  关闭给定的中断
  * @param  Pin: 引脚编号
  * @retval 无
  */
void detachInterrupt(uint8_t Pin)
{
    if(!IS_PIN(Pin))
        return;

    NVIC_DisableIRQ(EXTI_GetIRQn(Pin));
}

#define EXTIx_IRQHANDLER(n) \
do{\
    if(EXTI_GetITStatus(EXTI_Line##n) != RESET)\
    {\
        if(EXTI_Function[n]) EXTI_Function[n]();\
        EXTI_ClearITPendingBit(EXTI_Line##n);\
    }\
}while(0)

/**
  * @brief  外部中断入口，通道0~1
  * @param  无
  * @retval 无
  */
void EXTI0_1_IRQHandler(void)
{
    EXTIx_IRQHANDLER(0);
    EXTIx_IRQHANDLER(1);
}

/**
  * @brief  外部中断入口，通道2~3
  * @param  无
  * @retval 无
  */
void EXTI2_3_IRQHandler(void)
{
    EXTIx_IRQHANDLER(2);
    EXTIx_IRQHANDLER(3);
}

/**
  * @brief  外部中断入口，通道4~15
  * @param  无
  * @retval 无
  */
void EXTI4_15_IRQHandler(void)
{
    EXTIx_IRQHANDLER(4);
    EXTIx_IRQHANDLER(5);
    EXTIx_IRQHANDLER(6);
    EXTIx_IRQHANDLER(7);
    EXTIx_IRQHANDLER(8);
    EXTIx_IRQHANDLER(9);
    EXTIx_IRQHANDLER(10);
    EXTIx_IRQHANDLER(11);
    EXTIx_IRQHANDLER(12);
    EXTIx_IRQHANDLER(13);
    EXTIx_IRQHANDLER(15);
    EXTIx_IRQHANDLER(15);
}
