# Arduino-For-Keil STM32F10x
> https://github.com/FASTSHIFT/Arduino-For-Keil

# 更新日志
## [v1.0] - 2017.11.20
* 1.支持pinMode,digitalWrite,digitalRead,delay等基础Arduino API

## [v1.1] - 2017.11.21
* 1.改用C++编写程序，可以兼容部分Arduino函数库

## [v1.2] - 2017.11.23
* 1.移植Print、Stream、WString等class

## [v1.3] - 2017.12.12
* 1.添加millis()、micros()
* 2.添加Wire class、部分Serial class

## [v1.4] - 2018.1.10
* 1.全面支持Serial class
* 2.编写定时器库

## [v1.5] - 2018.1.12
* 1.尝试加入SD卡支持

## [v1.6] - 2018.1.21
* 1.支持analogRead(方式：DMA/No DMA)
* 2.analogWrite(频率和占空比修改)

## [v1.7] - 2018.1.23
* 1.支持attachInterrupt，所有GPIO都可以配置外部中断

## [v1.8] - 2018.1.31
* 1.TIM2~TIM4以可以正常输出PWM

## [v1.9] - 2018.2.1
* 1.HardwareSerial class加入end()、attachInterrupt()

## [v2.0] - 2018.2.1
* 1.修改函数指针定义方式

## [v2.1] - 2018.2.19
* 1.移除sys.h、sys.c
* 2.移植STM32duino的PIN_MAP[]，uint8_t类型的变量即可表示一个引脚
* 3.加入GPIO枚举，优化IO操作函数提升效率

## [v2.2] - 2018.2.27
* 1.调用Arduino.h时已不用声明extern"C"{}
* 2.加入WProgram.h以兼容某些库函数

## [v2.3] - 2018.2.28
* 1.修复EXTI的BUG

## [v2.4] - 2018.3.5
* 1.修复Wire.h的警告BUG

## [v2.5] - 2018.3.6
* 1.移植Servo舵机库

## [v2.6] - 2018.3.7
* 1.TIM1已支持中断和输出PWM
* 2.转移main.cpp创建的对象

## [v2.7] - 2018.3.15
* 1.添加pulseIn()
* 2.完善ADC_DMA

## [v2.8] - 2018.3.16
* 1.定时器库已支持配置中断优先级
* 2.去除Stream.h的警告
* 3.Core文件夹内的头文件已支持被C++直接调用

## [v2.9] - 2018.3.18
* 1.PWM库支持修改频率和占空比最大值
* 2.重新整理Arduino.h

## [v3.0] - 2018.3.27
* 1.支持自行选择ADCx进行配置
* 2.整理 "FileGroup.h"
* 3.PIN_MAP[] 添加 GPIOD~GPIOG 支持

## [v3.1] - 2018.3.28
 * 1.完善定时器库
 * 2.全面支持TIM1~TIM8快速配置定时器中断
 
## [v3.2] - 2018.4.25
* 1.添加analogRead_DMA()，用于读取PA0~PA7的ADC值(需使用ADCx_DMA_Init()函数进行DMA初始化)

## [v3.3] - 2018.4.28
* 1.移除旧版的 spi.c 和 HardwareSPI.c
* 2.完成SPI class

## [v3.4] - 2018.4.29
* 1.SD文件系统移植完成

## [v3.5] - 2018.5.3
* 1.加入更严格的引脚作用检查

## [v3.6] - 2018.5.7
* 1.添加示例代码，位于Example文件夹中

## [v3.7] - 2018.5.14
* 1.支持修改ADC求平均次数

## [v3.8] - 2018.6.9
* 1.更高速的digitalRead()与digitalWrite。(使用digitalWrite_HIGH(Pin)和digitalWrite_LOW(Pin))

## [v3.9] - 2018.6.13
* 1.修复digitalRead的BUG

## [v4.0] - 2018.6.15
* 1.修复attachInterrupt触发类型设置的BUG

## [v4.1] - 2018.6.19
* 1.SPI库中"uint16_t clock" 改为 "uint32_t clock"

## [v4.2] - 2018.6.23
* 1.System_ms 添加 volatile 修饰
* 2.改进微秒延时与tone相关函数

## [v4.3] - 2018.6.29
* 1.改进Wire库，I2C速度翻倍

## [v4.4] - 2018.7.4
* 1.Print库中，将 vsprintf 改成 vsnprintf 防止内存溢出

## [v4.5] - 2018.7.7
* 1.Print库中，重载 << 运算符，支持 Serial<<"Hello World"<<"\r\n" 流式操作

## [v4.6] - 2018.7.8
* 1.改进tone函数为非阻塞式(注意，tone会占用定时器，可以进入Tone.h进行更改，如无法避免冲突请使用Block_tone)
* 2.random()随机数已支持

## [v4.7] - 2018.7.25
* 1.优化 delay库、pwm库、timer_exti库
* 2.改进togglePin()
* 3.加入 F_CPU 自适应支持

## [v4.8] - 2018.7.30
* 1.优化GPIO相关函数

## [v4.9] - 2018.8.14
* 1.整理USART相关代码
* 2.修改Tone库只占用一个定时器(可使用toneSetTimer()函数切换)
* 3.time_exti.c改名为timer.c

## [v5.0] - 2018.8.17
* 1.在Arduino.h添加了更完整的与GPIO寄存器交互函数
* 2.提升与第三方库的兼容性

## [v5.1] - 2018.8.17
* 1.添加WCharacter.h，支持Arduino有关Characters的函数
* 2.添加mcu_type.h，使Arduino.h可以跨MCU使用

## [v5.2] - 2018.9.3
* 1.更新Arduino.h、SPI库、Tone库、GPIO库、定时器库
* 2.提升运行效率和兼容性

## [v5.3] - 2018.9.6
* 1.修改了Core核心库的一些定义(更新之后可能会导致报错，请修改相关函数名)

## [v5.4] - 2018.9.17
* 1.移除usart.h、usart.c
* 2.合并 HardwareSerial 与 usart 库

## [v5.5] - 2018.9.27
* 1.完善代码注释
* 2.提升SPI速度

## [v5.6] - 2018.9.29
* 1.重新从stm32duino移植更高精度的pulseIn函数

## [v5.7] - 2018.10.27
* 1.更新HardwareSerial，将USART发送完成标志位 USART_SR_TC 更换 USART_SR_TXE，更高的发送效率

## [v5.8] - 2018.11.4
* 1.提升digitalWrite等函数运行效率

## [v5.9] - 2018.11.20
* 1.修复TIM1与TIM8部分通道无法输出PWM的bug
* 2.更新SPI库

## [v6.0] - 2018.11.29
* 1.更新detachInterrupt()函数

## [v6.1] - 2018.12.10
* 1.补充Arduino.h和timer.c

## [v6.2] - 2019.1.26
* 1.更新Core核心库
* 2.支持"一键"超频(使用SysClock_Init配置主频)
* 3.在Tone库中添加参数合法性判断

## [v6.3] - 2019.2.5
* 1.Arduino.h加入WString.h和HardwareSerial.h包含

## [v6.4] - 2019.3.22
* 1.更新Tone库参数合法性判断顺序
* 2.为SysTick设置最高中断优先级，防止被其他中断打断(非常重要!)

## [v6.5] - 2019.5.22
* 1.pwm.c添加遗漏的定时器时钟初始化

## [v6.6] - 2019.6.3
* 1.pwm.c库将频率设定值从16位改为32位，并加入参数合法范围判断
* 2.ARDUINO宏定义从Arduino.h转至全局宏定义

## [v6.7] - 2019.7.12
* 1.Arduino.h里添加yield()与digitalPinToInterrupt(Pin)定义
* 2.更改analogRead_DMA()合法参数判断方式

## [v6.8] - 2019.8.1
* 1.更新pwm库和timer库
* 2.添加_BV(bit)宏
* 3.WString类添加sprintf成员

## [v6.9] - 2019.8.5
* 1.ADC DMA搬运已支持注册模式，可参考Example文件夹ADC_DMA.cpp
* 2.更新Example
* 3.更严格的引脚合法性检查

## [v7.0] - 2019.8.22
* 1.整理代码制表符全部替换为空格
* 2.去除analogRead求均值
* 3.延长ADC采样保持时间

## [v7.1] - 2019.11.4
* 1.更新Print库(移植Georges Menie的printf-stdarg.c)，printf不再有长度限制
* 2.Print添加sprintf成员

## [v7.2] - 2019.11.29
* 1.更新SPI库
* 2.更新Timer库

## [v7.3] - 2019.12.1
* 1.更新timer库，使用独立的开根号运算函数，不依赖math.h库

## [v7.4] - 2019.12.4
* 1.工程框架修改，工程文件与代码文件分离
* 2.修复了digitalWrite_LOW复制粘贴导致的BUG

## [v7.5] - 2019.12.6
* 1.调整Core里的一些函数命名更加规范更贴近标准库
* 2.EXTI中断入口更换了EXTIx_IRQHANDLER写法

## [v7.6] - 2020.1.15
* 1.由于Print库的printf不支持浮点输出，将Print库还原，使用vsnprintf转换printf

## [v7.7] - 2020.4.7
* 1.更新日志改为使用Markdown语法编写
* 2.去除重复的dtostrf.c、dtostrf.h
* 3.添加Libraries路径
* 4.Arduino.h添加时钟周期与时间转换的宏
* 5.Arduino.h添加WMath.h与Tone.h
* 6.添加README文件组

## [v7.8] - 2020.4.23
* 1.去除FileGroup.h
* 2.GPIO快速操作宏定义函数从mcu_type.h转移到gpio.h和Arduino.h
* 3.__STM32__宏定义从Arduino.h转移到mcu_type.h
