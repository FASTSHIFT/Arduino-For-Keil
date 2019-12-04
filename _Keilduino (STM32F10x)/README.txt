
# A Cross-platform migration Project @FASTSHIFT
# Arduino for STM32F10x

# https://github.com/FASTSHIFT/Arduino-For-Keil

//*********************************UPGRADE LOGS************************************//

//Finish in 2017.11.20 V_1.0 支持pinMode,digitalWrite,digitalRead,delay等基础Arduino API
//Upgrade   2017.11.21 V_1.1 改用C++编写程序，可以兼容部分Arduino函数库，只需将库拖入Library文件夹下即可
//Upgrade   2017.11.23 V_1.2 移植Print、Stream、WString等class
//Upgrade   2017.12.12 V_1.3 支持millis、micros、Wire class、部分Serial class
//Upgrade   2018.1.10  V_1.4 全面支持Serial class，编写了定时器库
//Upgrade   2018.1.12  V_1.5 尝试加入SD卡支持
//Upgrade   2018.1.21  V_1.6 支持analogRead(方式：DMA/No DMA)、analogWrite(频率和占空比修改)
//Upgrade   2018.1.23  V_1.7 支持attachInterrupt，所有GPIO都可以配置外部中断
//Upgrade   2018.1.31  V_1.8 TIM2~TIM4以可以正常输出PWM
//Upgrade   2018.2.1   V_1.9 HardwareSerial class加入end()、attachInterrupt()
//Upgrade   2018.2.10  V_2.0 修改函数指针定义方式
//Upgrade   2018.2.19  V_2.1 移除sys.h、sys.c，移植STM32duino的PIN_MAP[]，uint8_t类型的变量即可表示一个引脚，加入enum，优化IO操作函数提升效率
//Upgrade   2018.2.27  V_2.2 调用Arduino.h时已不用声明extern"C"{},加入WProgram.h以兼容某些库函数
//Upgrade   2018.2.28  V_2.3 修复EXTI的BUG
//Upgrade   2018.3.5   V_2.4 修复Wire.h的警告BUG
//Upgrade   2018.3.6   V_2.5 移植Servo舵机库
//Upgrade   2018.3.7   V_2.6 TIM1已支持中断和输出PWM，转移main.cpp创建的对象
//Upgrade   2018.3.15  V_2.7 支持pulseIn(),完善ADC_DMA
//Upgrade   2018.3.16  V_2.8 定时器库已支持配置中断优先级，去除Stream.h的警告,Core文件夹内的头文件已支持被C++直接调用
//Upgrade   2018.3.18  V_2.9 PWM库支持修改频率和占空比最大值，重新整理Arduino.h
//Upgrade   2018.3.27  V_3.0 支持自行选择ADCx进行配置，整理 "FileGroup.h"，PIN_MAP[] 添加 GPIOD~GPIOG 支持
//Upgrade   2018.3.28  V_3.1 完善定时器库，全面支持TIM1~TIM8快速配置定时器中断
//Upgrade   2018.4.25  V_3.2 支持使用analogRead_DMA()函数读取PA0~PA7的ADC值(需使用ADCx_DMA_Init()函数进行DMA初始化)
//Upgrade   2018.4.28  V_3.3 移除旧版的 spi.c 和 HardwareSPI.c , SPI class已支持
//Upgrade   2018.4.29  V_3.4 SD文件系统移植完成
//Upgrade   2018.5.3   V_3.5 加入更严格的引脚作用检查
//Upgrade   2018.5.7   V_3.6 添加示例代码，位于Example文件夹中
//Upgrade   2018.5.14  V_3.7 支持修改ADC求平均次数
//Upgrade   2018.6.9   V_3.8 更高速的digitalRead(Pin), digitalWrite(使用digitalWrite_HIGH(Pin)和digitalWrite_LOW(Pin))
//Upgrade   2018.6.13  V_3.9 修复digitalRead的BUG
//Upgrade   2018.6.15  V_4.0 修复attachInterrupt触发类型设置的BUG
//Upgrade   2018.6.19  V_4.1 SPI库中"uint16_t clock" 改为 "uint32_t clock"
//Upgrade   2018.6.23  V_4.2 System_ms 改为 volatile 修饰，改进微秒延时与tone相关函数
//Upgrade   2018.6.29  V_4.3 改进Wire库，I2C速度翻倍
//Upgrade   2018.7.4   V_4.4 Print库中，将 vsprintf 改成 vsnprintf 防止内存溢出
//Upgrade   2018.7.7   V_4.5 Print库中，重载 << 运算符，支持 Serial<<"Hello World"<<"\r\n" 流式操作
//Upgrade   2018.7.8   V_4.6 改进tone函数为非阻塞式(注意，tone会占用定时器，可以进入Tone.h进行更改，如无法避免冲突请使用Block_tone)，random()随机数已支持
//Upgrade   2018.7.25  V_4.7 优化 delay库、pwm库、timer_exti库，改进togglePin()，加入 F_CPU 自适应支持
//Upgrade   2018.7.30  V_4.8 优化GPIO相关函数
//Upgrade   2018.8.14  V_4.9 整理USART相关代码，修改Tone库只占用一个定时器(可使用toneSetTimer()函数切换),time_exti改名为timer
//Upgrade   2018.8.17  V_5.0 在Arduino.h添加了更完整的与GPIO寄存器交互函数，提升与第三方库的兼容性
//Upgrade   2018.8.17  V_5.1 添加WCharacter.h，支持Arduino有关Characters的函数，添加mcu_type.h，使Arduino.h可以跨MCU使用
//Upgrade   2018.9.3   V_5.2 更新Arduino.h、SPI库、Tone库、GPIO库、定时器库，更高的效率以及更好的兼容性
//Upgrade   2018.9.6   V_5.3 修改了Core核心库的一些定义(更新之后可能会导致报错，请修改相关函数名)
//Upgrade   2018.9.17  V_5.4 合并 HardwareSerial 与 usart 库，usart.h、usart.c已移除
//Upgrade   2018.9.27  V_5.5 完善代码注释，提升SPI速度
//Upgrade   2018.9.29  V_5.6 重新从stm32duino移植更高精度的pulseIn函数
//Upgrade   2018.10.27 V_5.7 更新HardwareSerial，将USART发送完成标志位 USART_SR_TC 更换 USART_SR_TXE，更高的发送效率
//Upgrade   2018.11.4  V_5.8 提升digitalWrite等函数运行效率
//Upgrade   2018.11.20 V_5.9 修复TIM1与TIM8部分通道无法输出PWM的bug，更新SPI库
//Upgrade   2018.11.29 V_6.0 更新detachInterrupt()函数
//Upgrade   2018.12.10 V_6.1 补充Arduino.h和timer.c
//Upgrade   2019.1.26  V_6.2 更新Core核心库，支持"一键"超频(使用SysClock_Init配置主频)，在Tone库中添加参数合法性判断
//Upgrade   2019.2.5   V_6.3 Arduino.h已包含WString.h和HardwareSerial.h 
//Upgrade   2019.3.22  V_6.4 更新Tone库参数合法性判断顺序，为SysTick设置最高中断优先级，防止被其他中断打断(非常重要!)
//Upgrade   2019.5.22  V_6.5 pwm.c添加遗漏的定时器时钟初始化
//Upgrade   2019.6.3   V_6.6 pwm.c库将频率设定值从16位改为32位，并加入参数合法范围判断，ARDUINO宏定义从Arduino.h转至全局宏定义
//Upgrade   2019.7.12  V_6.7 Arduino.h里添加yield()与digitalPinToInterrupt(Pin)定义，更改analogRead_DMA()合法参数判断方式
//Upgrade   2019.8.1   V_6.8 更新pwm库和timer库,添加_BV(bit)宏，WString类添加sprintf成员
//Upgrade   2019.8.5   V_6.9 ADC DMA搬运已支持注册模式，可参考Example文件夹ADC_DMA.cpp，更新Example，更严格的引脚合法性检查
//Upgrade   2019.8.22  V_7.0 整理代码制表符全部替换为空格，去除analogRead求均值，延长ADC采样保持时间
//Upgrade   2019.11.4  V_7.1 更新Print库(移植Georges Menie的printf-stdarg.c)，printf不再有长度限制，添加sprintf成员
//Upgrade   2019.11.29 V_7.2 更新SPI库,更新Timer库
//Upgrade   2019.12.1  V_7.3 更新timer库，使用独立的开根号运算函数，不依赖math.h库
//Upgrade   2019.12.4  V_7.4 工程框架修改，工程文件与代码文件分离，修复了digitalWrite_LOW复制粘贴导致的BUG 