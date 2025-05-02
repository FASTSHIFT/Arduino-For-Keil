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
#include "gpio.h"

const PinInfo_TypeDef PIN_MAP[PIN_MAX] =
{
    /** GPIO_Type* GPIOx;        GPIOx地址
      * TIM_Type* TIMx;          TIMx地址
      * ADC_Type* ADCx;          ADCx地址
      * uint16_t GPIO_Pin_x;     GPIO_Pin位
      * uint8_t TimerChannel;    定时器通道
      * uint8_t ADC_CHANNEL;     ADC通道
      */
    {GPIOA, NULL, ADC1, GPIO_Pin_0, 1, ADC_CHANNEL_0},  /* PA0 */
    {GPIOA, NULL, ADC1, GPIO_Pin_1, 2, ADC_CHANNEL_1},  /* PA1 */
    {GPIOA, NULL, ADC1, GPIO_Pin_2, 3, ADC_CHANNEL_2},  /* PA2 */
    {GPIOA, NULL, ADC1, GPIO_Pin_3, 4, ADC_CHANNEL_3},  /* PA3 */
    {GPIOA, NULL, ADC1, GPIO_Pin_4, 0, ADC_CHANNEL_4},  /* PA4 */
    {GPIOA, NULL, ADC1, GPIO_Pin_5, 1, ADC_CHANNEL_5},  /* PA5 */
    {GPIOA, TIM3, ADC1, GPIO_Pin_6, 1, ADC_CHANNEL_6},  /* PA6 */
    {GPIOA, TIM3, ADC1, GPIO_Pin_7, 2, ADC_CHANNEL_7},  /* PA7 */
    {GPIOA, TIM1, NULL, GPIO_Pin_8, 1, ADC_CHANNEL_X},  /* PA8 */
    {GPIOA, TIM1, NULL, GPIO_Pin_9, 2, ADC_CHANNEL_X},  /* PA9 */
    {GPIOA, TIM1, NULL, GPIO_Pin_10, 3, ADC_CHANNEL_X}, /* PA10 */
    {GPIOA, TIM1, NULL, GPIO_Pin_11, 4, ADC_CHANNEL_X}, /* PA11 */
    {GPIOA, NULL, NULL, GPIO_Pin_12, 0, ADC_CHANNEL_X}, /* PA12 */
    {GPIOA, NULL, NULL, GPIO_Pin_13, 0, ADC_CHANNEL_X}, /* PA13 */
    {GPIOA, NULL, NULL, GPIO_Pin_14, 0, ADC_CHANNEL_X}, /* PA14 */
    {GPIOA, NULL, NULL, GPIO_Pin_15, 1, ADC_CHANNEL_X}, /* PA15 */

    {GPIOB, TIM3, ADC1,  GPIO_Pin_0, 3, ADC_CHANNEL_8}, /* PB0 */
    {GPIOB, TIM3, ADC1,  GPIO_Pin_1, 4, ADC_CHANNEL_9}, /* PB1 */
    {GPIOB, NULL, NULL,  GPIO_Pin_2, 4, ADC_CHANNEL_X}, /* PB2 */
    {GPIOB, NULL, NULL,  GPIO_Pin_3, 2, ADC_CHANNEL_X}, /* PB3 */
    {GPIOB, TIM3, NULL,  GPIO_Pin_4, 1, ADC_CHANNEL_X}, /* PB4 */
    {GPIOB, TIM3, NULL,  GPIO_Pin_5, 2, ADC_CHANNEL_X}, /* PB5 */
    {GPIOB, NULL, NULL,  GPIO_Pin_6, 1, ADC_CHANNEL_X}, /* PB6 */
    {GPIOB, NULL, NULL,  GPIO_Pin_7, 2, ADC_CHANNEL_X}, /* PB7 */
    {GPIOB, NULL, NULL,  GPIO_Pin_8, 3, ADC_CHANNEL_X}, /* PB8 */
    {GPIOB, NULL, NULL,  GPIO_Pin_9, 4, ADC_CHANNEL_X}, /* PB9 */
    {GPIOB, NULL, NULL, GPIO_Pin_10, 3, ADC_CHANNEL_X}, /* PB10 */
    {GPIOB, NULL, NULL, GPIO_Pin_11, 4, ADC_CHANNEL_X}, /* PB11 */
    {GPIOB, NULL, NULL, GPIO_Pin_12, 1, ADC_CHANNEL_X}, /* PB12 */
    {GPIOB, NULL, NULL, GPIO_Pin_13, 0, ADC_CHANNEL_X}, /* PB13 */
    {GPIOB, NULL, NULL, GPIO_Pin_14, 1, ADC_CHANNEL_X},/* PB14 */
    {GPIOB, NULL, NULL, GPIO_Pin_15, 2, ADC_CHANNEL_X}, /* PB15 */

    {GPIOC, NULL, ADC1,  GPIO_Pin_0, 0, ADC_CHANNEL_10},/* PC0 */
    {GPIOC, NULL, ADC1,  GPIO_Pin_1, 0, ADC_CHANNEL_11},/* PC1 */
    {GPIOC, NULL, ADC1,  GPIO_Pin_2, 2, ADC_CHANNEL_12},/* PC2 */
    {GPIOC, NULL, ADC1,  GPIO_Pin_3, 0, ADC_CHANNEL_13},/* PC3 */
    {GPIOC, NULL, ADC1,  GPIO_Pin_4, 1, ADC_CHANNEL_14},/* PC4 */
    {GPIOC, NULL, ADC1,  GPIO_Pin_5, 2, ADC_CHANNEL_15},/* PC5 */
    {GPIOC, TIM3, NULL,  GPIO_Pin_6, 1, ADC_CHANNEL_X}, /* PC6 */
    {GPIOC, TIM3, NULL,  GPIO_Pin_7, 2, ADC_CHANNEL_X}, /* PC7 */
    {GPIOC, TIM3, NULL,  GPIO_Pin_8, 3, ADC_CHANNEL_X}, /* PC8 */
    {GPIOC, TIM3, NULL,  GPIO_Pin_9, 4, ADC_CHANNEL_X}, /* PC9 */
    {GPIOC, NULL, NULL, GPIO_Pin_10, 2, ADC_CHANNEL_X}, /* PC10 */
    {GPIOC, NULL, NULL, GPIO_Pin_11, 3, ADC_CHANNEL_X}, /* PC11 */
    {GPIOC, NULL, NULL, GPIO_Pin_12, 0, ADC_CHANNEL_X}, /* PC12 */
    {GPIOC, NULL, NULL, GPIO_Pin_13, 0, ADC_CHANNEL_X}, /* PC13 */
    {GPIOC, NULL, NULL, GPIO_Pin_14, 0, ADC_CHANNEL_X}, /* PC14 */
    {GPIOC, NULL, NULL, GPIO_Pin_15, 0, ADC_CHANNEL_X}, /* PC15 */

    {GPIOF, NULL, NULL,  GPIO_Pin_0, 0, ADC_CHANNEL_X}, /* PF0 */
    {GPIOF, NULL, NULL,  GPIO_Pin_1, 0, ADC_CHANNEL_X}, /* PF1 */
    {GPIOF, NULL, NULL,  GPIO_Pin_2, 3, ADC_CHANNEL_X}, /* PF2 */
    {GPIOF, NULL, NULL,  GPIO_Pin_3, 4, ADC_CHANNEL_9}, /* PF3 */
    {GPIOF, NULL, NULL,  GPIO_Pin_4, 0, ADC_CHANNEL_14},/* PF4 */
    {GPIOF, NULL, NULL,  GPIO_Pin_5, 0, ADC_CHANNEL_15},/* PF5 */
    {GPIOF, NULL, NULL,  GPIO_Pin_6, 1, ADC_CHANNEL_4}, /* PF6 */
    {GPIOF, NULL, NULL,  GPIO_Pin_7, 1, ADC_CHANNEL_5}, /* PF7 */
    {GPIOF, NULL, NULL,  GPIO_Pin_8, 1, ADC_CHANNEL_6}, /* PF8 */
    {GPIOF, NULL, NULL,  GPIO_Pin_9, 1, ADC_CHANNEL_7}, /* PF9 */
    {GPIOF, NULL, NULL, GPIO_Pin_10, 0, ADC_CHANNEL_8}, /* PF10 */
    {GPIOF, NULL, NULL, GPIO_Pin_11, 0, ADC_CHANNEL_X}, /* PF11 */
    {GPIOF, NULL, NULL, GPIO_Pin_12, 1, ADC_CHANNEL_X}, /* PF12 */
    {GPIOF, NULL, NULL, GPIO_Pin_13, 2, ADC_CHANNEL_X}, /* PF13 */
    {GPIOF, NULL, NULL, GPIO_Pin_14, 3, ADC_CHANNEL_X}, /* PF14 */
    {GPIOF, NULL, NULL, GPIO_Pin_15, 4, ADC_CHANNEL_X}, /* PF15 */
};

/**
  * @brief  GPIO初始化
  * @param  GPIOx: GPIO地址
  * @param  GPIO_Pin_x: GPIO对应位
  * @param  GPIO_Mode_x: GPIO模式
  * @param  GPIO_Drive_x: GPIO速度
  * @retval 无
  */
void GPIOx_Init(
    gpio_type* GPIOx,
    uint16_t GPIO_Pin_x,
    PinMode_TypeDef Mode,
    gpio_drive_type GPIO_Drive_x
)
{
    gpio_init_type gpio_init_struct;
    crm_periph_clock_type CRM_GPIOx_PERIPH_CLOCK;

    if(GPIOx == GPIOA)     CRM_GPIOx_PERIPH_CLOCK = CRM_GPIOA_PERIPH_CLOCK;
    else if(GPIOx == GPIOB)CRM_GPIOx_PERIPH_CLOCK = CRM_GPIOB_PERIPH_CLOCK;
    else if(GPIOx == GPIOC)CRM_GPIOx_PERIPH_CLOCK = CRM_GPIOC_PERIPH_CLOCK;
    else if(GPIOx == GPIOF)CRM_GPIOx_PERIPH_CLOCK = CRM_GPIOF_PERIPH_CLOCK;
    else return;

    gpio_default_para_init(&gpio_init_struct);
    gpio_init_struct.gpio_pins = GPIO_Pin_x;
    gpio_init_struct.gpio_drive_strength = GPIO_Drive_x;

    switch(Mode)
    {
    case INPUT:
        gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
        gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
        break;

    case INPUT_PULLUP:
        gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
        gpio_init_struct.gpio_pull = GPIO_PULL_UP;
        break;

    case INPUT_PULLDOWN:
        gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
        gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
        break;

    case INPUT_ANALOG:
        gpio_init_struct.gpio_mode = GPIO_MODE_ANALOG;
        gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
        break;

    case OUTPUT:
        gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
        gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
        break;

    case OUTPUT_OPEN_DRAIN:
        gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
        gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
        gpio_init_struct.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
        break;

    case OUTPUT_AF_PP:
        gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
        gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
        break;

    case OUTPUT_AF_OD:
        gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
        gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
        gpio_init_struct.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
        break;

    default:
        return;
    }

    crm_periph_clock_enable(CRM_GPIOx_PERIPH_CLOCK, TRUE);
    gpio_init(GPIOx, &gpio_init_struct);
}


/**
  * @brief  获取当前引脚对应的GPIOx编号
  * @param  Pin: 引脚编号
  * @retval 无
  */
scfg_port_source_type GPIO_GetPortNum(uint8_t Pin)
{
    uint8_t retval = 0xFF;
    int i;
    gpio_type* GPIOx = PIN_MAP[Pin].GPIOx;

    static const gpio_type* GPIO_Map[] =
    {
        GPIOA,
        GPIOB,
        GPIOC,
        GPIOF
    };

    for(i = 0; i < sizeof(GPIO_Map) / sizeof(GPIO_Map[0]); i++)
    {
        if(GPIOx == GPIO_Map[i])
        {
            retval = i;
            break;
        }
    }

    return (scfg_port_source_type)retval;
}

/**
  * @brief  获取当前引脚对应的 PinSource
  * @param  GPIO_Pin_x: GPIO对应位
  * @retval 无
  */
gpio_pins_source_type GPIO_GetPinSource(uint16_t GPIO_Pin_x)
{
    uint8_t PinSource = 0;
    while(GPIO_Pin_x > 1)
    {
        GPIO_Pin_x >>= 1;
        PinSource++;
    }
    return (gpio_pins_source_type)PinSource;
}

/**
  * @brief  获取当前引脚对应的编号
  * @param  Pin: 引脚编号
  * @retval 无
  */
uint8_t GPIO_GetPinNum(uint8_t Pin)
{
    return GPIO_GetPinSource(PIN_MAP[Pin].GPIO_Pin_x);
}
