# Arduino_For_Keil @FASTSHIFT
# For STM32F10x   

//*********************************UPGRADE LOGS************************************//

//Finish in 2017.11.20 V_1.0 支持pinMode,digitalWrite,digitalRead,delay等基础Arduino API
//Upgrade        11.21 V_1.1 改用C++编写程序，可以兼容部分Arduino函数库，只需将库拖入Library文件夹下即可
//Upgrade        11.23 V_1.2 移植Print、Stream、WString等class
//Upgrade        12.12 V_1.3 支持millis、micros、Wire class、部分Serial class
//Upgrade   2018.1.10  V_1.4 全面支持Serial class，编写了定时器库
//Upgrade        1.12  V_1.5 尝试加入SD卡支持
//Upgrade        1.21  V_1.6 支持analogRead(方式：DMA/No DMA)、analogWrite(频率和占空比修改)
//Upgrade        1.23  V_1.7 支持attachInterrupt，所有GPIO都可以配置外部中断
//Upgrade        1.31  V_1.8 TIM2~TIM4以可以正常输出PWM
//Upgrade        2.1   V_1.9 HardwareSerial class加入end()、attachInterrupt()
//Upgrade        2.10  V_2.0 修改函数指针定义方式
//Upgrade        2.19  V_2.1 移除sys.h、sys.c，移植STM32duino的PIN_MAP[]，uint8_t类型的变量即可表示一个引脚，加入enum，优化IO操作函数提升效率
//Upgrade        2.27  V_2.2 调用Arduino.h时已不用声明extern"C"{},加入WProgram.h以兼容某些库函数
//Upgrade        2.28  V_2.3 修复EXTI的BUG
//Upgrade        3.5   V_2.4 修复Wire.h的警告BUG
//Upgrade        3.6   V_2.5 移植Servo舵机库
//Upgrade        3.7   V_2.6 TIM1已支持中断和输出PWM，转移main.cpp创建的对象
//Upgrade        3.15  V_2.7 支持pulseIn(),完善ADC_DMA
//Upgrade        3.16  V_2.8 定时器库已支持配置中断优先级，去除Stream.h的警告,Core文件夹内的头文件已支持被C++直接调用
//Upgrade        3.18  V_2.9 PWM库支持修改频率和占空比最大值，重新整理Arduino.h
//Upgrade        3.27  V_3.0 支持自行选择ADCx进行配置，整理 "FileGroup.h"，PIN_MAP[] 添加 GPIOD~GPIOG 支持
//Upgrade        3.28  V_3.1 完善定时器库，全面支持TIM1~TIM8快速配置定时器中断
//Upgrade        4.25  V_3.2 支持使用analogRead_DMA()函数读取PA0~PA7的ADC值(需使用ADCx_DMA_Init()函数进行DMA初始化)
//Upgrade        4.28  V_3.3 移除旧版的 spi.c 和 HardwareSPI.c , SPI class已支持
//Upgrade        4.29  V_3.4 SD文件系统移植完成
//Upgrade        5.3   V_3.5 加入更严格的引脚作用检查
//Upgrade        5.7   V_3.6 添加示例代码，位于Example文件夹中
//Upgrade        5.14  V_3.7 支持修改ADC求平均次数
//Upgrade        6.9   V_3.8 更高速的digitalRead(Pin), digitalWrite(使用digitalWrite_HIGH(Pin)和digitalWrite_LOW(Pin))
//Upgrade        6.13  V_3.9 修复digitalRead的BUG
//Upgrade        6.15  V_4.0 修复attachInterrupt触发类型设置的BUG
//Upgrade        6.19  V_4.1 SPI库中"uint16_t clock" 改为 "uint32_t clock"
//Upgrade        6.23  V_4.2 System_ms 改为 volatile 修饰，改进微秒延时与tone相关函数
//Upgrade        6.29  V_4.3 改进Wire库，I2C速度翻倍
//Upgrade        7.4   V_4.4 Print库中，将 vsprintf 改成 vsnprintf 防止内存溢出
//Upgrade        7.7   V_4.5 Print库中，重载 << 运算符，支持 Serial<<"Hello World"<<"\r\n" 流式操作
//Upgrade        7.8   V_4.6 改进tone函数为非阻塞式(注意，tone会占用定时器，可以进入Tone.h进行更改，如无法避免冲突请使用Block_tone)，random()随机数已支持
//Upgrade        7.25  V_4.7 优化 delay库、pwm库、timer_exti库，改进togglePin()，加入 F_CPU 自适应支持
//Upgrade        7.30  V_4.8 优化GPIO相关函数
//Upgrade        8.14  V_4.9 整理USART的代码
