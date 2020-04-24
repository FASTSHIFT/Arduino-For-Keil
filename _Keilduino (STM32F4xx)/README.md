# Arduino-For-Keil STM32F4xx
> https://github.com/FASTSHIFT/Arduino-For-Keil

# 注意: 
1. 标准外设库默认晶振频率是25MHz。若板载晶振为8MHz,需要将system_stm32f4xx.c 的 PLL_M 定义 25 改为 8。
2. 该项目模板包括三个工程：
 - (1)**GCC**: 使用Keil调用GCC(arm-none-eabi)编译器进行编译，前提是您的电脑必须安装有这个编译器。"xxx.ld"文件可以使用CubeMX生成，需要在Linker菜单里重新选择。
 - (2)**STD v1.3.0**: 使用pack安装到Keil内的v1.3.0的标准库，用这个配置工程比较方便，但是STM32F4的很多新型号不支持。
 - (3)**STD v1.8.0**: 使用工程自带的v1.8.0的标准库，对新型号支持较好，工程打开后会提示安装pack，安装后就可以正常使用芯片选型。缺点是要手动配置工程增删文件。

# 更新日志
## [v1.0] - 2018.8.1
* 1.基于STM32F4xx标准外设库(1.3.0版)，移植了全部的ArduinoAPI

## [v1.1] - 2018.8.14
* 1.整理USART相关代码
* 2.修改Tone库只占用一个定时器(可使用toneSetTimer()函数切换)
* 3.time_exti.c改名为timer.c

## [v1.2] - 2018.8.17
* 1.在Arduino.h添加了更完整的与GPIO寄存器交互函数
* 2.提升与第三方库的兼容性

## [v1.3] - 2018.8.17
* 1.添加WCharacter.h，支持Arduino有关Characters的函数
* 2.添加mcu_type.h，使Arduino.h可以跨MCU使用

## [v1.4] - 2018.11.28
* 1.增加了注释

## [v1.5] - 2019.2.17
* 1.提升效率以及兼容性
* 2.SPI库已可以正常工作

## [v1.6] - 2019.3.22
* 1.为SysTick设置最高中断优先级，防止被其他中断打断(非常重要!)

## [v1.7] - 2019.5.9
* 1.更新:ADC库、PWM库、定时器库、Servo库、PIN_MAP引脚外设映射表
* 2.Tone库已可以正常工作

## [v1.8] - 2019.6.3
* 1.pwm.c库将频率设定值从16位改为32位，并加入参数合法范围判断
* 2.ARDUINO宏定义从Arduino.h转至全局宏定义

## [v1.9] - 2019.7.12
* 1.Arduino.h里添加yield()与digitalPinToInterrupt(Pin)定义
* 2.更改analogRead_DMA()合法参数判断方式

## [v2.0] - 2019.8.22
* 1.同步_Keilduino(STM32F10x) V_7.0代码

## [v2.1] - 2019.8.24
* 1.digitalRead_Fast修改为digitalRead_FAST
* 2.修复 gpio.c 内 PIN_MAP 的 PC3、PC4、PC5 ADC通道的错误定义

## [v2.2] - 2019.9.28
* 1.更新SPI库

## [v2.3] - 2019.10.9
* 1.添加了STM32F4xx V1.8.0标准库，支持新的F4型号
* 2.需要手动添加FLASH烧写算法文件,或者安装新的PACK

## [v2.4] - 2019.11.4
* 1.更新Print库(移植Georges Menie的printf-stdarg.c)，printf不再有长度限制，添加sprintf成员

## [v2.5] - 2019.12.1
* 1.更新timer库，使用独立的开根号运算函数，不依赖math.h库

## [v2.6] - 2019.12.4
* 1.工程框架修改，工程文件与代码文件分离
* 2.修复了digitalWrite_LOW复制粘贴导致的BUG

## [v2.7] - 2019.12.6
* 1.使Core里的一些函数命名更加规范更贴近标准库
* 2.EXTI中断入口更换了EXTIx_IRQHANDLER写法
* 3.添加了GNU支持

## [v2.8] - 2019.12.31
* 1.更新SPI库的setClockDivider

## [v2.9] - 2019.12.31
* 1.由于Print库的printf不支持浮点输出，将Print库还原，使用vsnprintf转换printf

## [v3.0] - 2020.4.7
* 1.更新日志改为使用Markdown语法编写
* 2.去除重复的dtostrf.c、dtostrf.h
* 3.Arduino.h添加时钟周期与时间转换的宏
* 4.Arduino.h添加WMath.h与Tone.h
* 5.添加README文件组

## [v3.1] - 2020.4.16
* 1.去除FileGroup.h
* 2.GPIO快速操作宏定义函数从mcu_type.h转移到gpio.h和Arduino.h
* 3.__STM32__宏定义从Arduino.h转移到mcu_type.h
* 4.delay.c、adc.c去除#include "Arduino.h"
* 5.Libraries只保留Servo库
