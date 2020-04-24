# Arduino-For-Keil STM32F0xx
> https://github.com/FASTSHIFT/Arduino-For-Keil

# 更新日志
## [v1.0] - 2018.7.29
* 1.基于STM32F0xx标准外设库(1.0.1版)，移植了全部的ArduinoAPI

## [v1.1] - 2018.8.14
* 1.整理USART相关代码
* 2.修改Tone库只占用一个定时器(可使用toneSetTimer()函数切换)
* 3.time_exti.c改名为timer.c

## [v1.2] - 2018.8.17
* 1.在Arduino.h添加了更完整的与GPIO寄存器交互函数
* 2.提升与第三方库的兼容性

## [v1.3] - 2018.8.20
* 1.添加WCharacter.h，支持Arduino有关Characters的函数
* 2.添加mcu_type.h，使Arduino.h可以跨MCU使用

## [v1.4] - 2018.10.9
* 1.添加注释
* 2.更新Tone、HardwareSerial
* 3.去除usart.c usart.h

## [v1.5] - 2018.11.21
* 1.更新Arduino.c

## [v1.6] - 2019.2.18
* 1.整理代码
* 2.更好的兼容性

## [v1.7] - 2019.3.22
* 1.为SysTick设置最高中断优先级，防止被其他中断打断(非常重要!)

## [v1.8] - 2019.6.3
* 1.pwm.c库将频率设定值从16位改为32位，并加入参数合法范围判断
* 2.ARDUINO宏定义从Arduino.h转至全局宏定义

## [v1.9] - 2019.6.18
* 1.PA4已可以输出PWM

## [v2.0] - 2019.7.12
* 1.Arduino.h里添加yield()与digitalPinToInterrupt(Pin)定义
* 2.更改analogRead_DMA()合法参数判断方式

## [v2.1] - 2019.10.3
* 1.同步_Keilduino(STM32F10x) V_7.0部分代码
* 2.暂时去除ADC DMA
* 3.将STM32F030F4P6串口1映射到PA9和PA10
* 4.添加TIM6定时中断

## [v2.2] - 2019.11.4
* 1.更新Print库(移植Georges Menie的printf-stdarg.c)，printf不再有长度限制
* 2.添加sprintf成员

## [v2.3] - 2019.12.25
* 1.SPI库可以正常使用，添加使用内部时钟选项
* 2.同步_Keilduino(STM32F10x) V_7.5部分代码

## [v2.4] - 2020.1.15
* 1.由于Print库的printf不支持浮点输出，将Print库还原，使用vsnprintf转换printf 

## [v2.5] - 2020.4.14
* 1.更新日志改为使用Markdown语法编写
* 2.去除重复的dtostrf.c、dtostrf.h
* 3.添加Libraries路径
* 4.Arduino.h添加时钟周期与时间转换的宏
* 5.Arduino.h添加WMath.h与Tone.h
* 6.添加README文件组
* 7.添加yield()函数实体
* 8.GPIO快速操作宏定义函数从mcu_type.h转移到gpio.h和Arduino.h
* 9.__STM32__宏定义从Arduino.h转移到mcu_type.h
* 10.delay.c、adc.c去除#include "Arduino.h"
* 11.为PA5添加定时器通道定义

## [v2.6] - 2020.4.16
* 1.去除FileGroup.h
* 2.Libraries只保留Servo库

## [v2.7] - 2020.4.24
* 1.adc库只能读一个通道的BUG已解决
* 2.ADC_DMA模式已支持
