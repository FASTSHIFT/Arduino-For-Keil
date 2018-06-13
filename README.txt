# Arduino_For_Keil @FASTSHIFT

     这是一个跨平台移植的项目，可以在Keil MDK5环境下使用Arduino的函数对STM32F10x系列单片机进行编程。它本质上是一个建立好的Keil工程，然后在它的基础上封装一套类似于Arduino的底层函数库，可以像在Arduino IDE一样直接调用写好的API，只需将Arduino支持的第三方库的文件夹加入 "Libraries" 文件夹下即可，然后打开"USER"文件夹启动keil工程，在"main.cpp"这个文件下进行编写程序，可以直接使用Arduino的示例源代码。

     注意： 
     1.请不要删除"main.cpp"中的FileGroup.h和main函数。 
     2.include第三方库函数的时候要加入完整的路径，例如在Arduino IDE下：#include"SD.h"，在Keil中就要写成：#include"SD/SD.h"。 
     3.不能保证兼容所有的Arduino第三方库函数，有的函数库可能要进行修改才能正常运行，具体修改方式请按照编译器的提示，或者可以提交Issues。 
      
     它的底层是基于STM32标准外设库(3.5版本)二次封装，部分源代码来自 stm32duino，用法与Arduino基本一致，支持与寄存器和标准外设库的函数混用。
     
     目前在STM32F103C8T6/ZET6上成功移植的API有： 
   基本： 
     时间:
          delay(Time_ms)
          delayMicroseconds(Time_us)
          millis()
          micros()
          TimerSet(TIMx,InterruptTime_us,Callback_function)
     输入输出：
          pinMode(Pin,Mode)
          attachInterrrupt(Pin,Trigger_Mode,Callback_function)
          digitalWrite(Pin,State)
          digitalRead(Pin)
          analogRead(Pin)
          analogWrite(Pin,DutyCycle)
          shiftOut(dataPin,clockPin,bitOrder,value)
          shiftIn(dataPin,clockPin,bitOrder)
          tone(Pin,Frequency,Time_ms)
          
     Print类 //支持printf
     String类 
     Stream类 
      
   外设相关： 
     Serial类 
     Wire类
     SPI类
      
 
   This is a cross-platform porting project. You can use Arduino's functions to program STM32F10x microcontrollers in the Keil MDK5 environment. It is essentially a well-established Keil project, and then it encapsulates a set of low-level libraries similar to Arduino. It can directly call the written APIs just like the Arduino IDE. It only needs to support third-party libraries supported by Arduino. The folder can be added under the "Libraries" folder, and then open the "USER" folder to start the keil project, under the "main.cpp" file to write the program, you can directly use the Arduino example source code.

     Note:
     1. Please do not delete the FileGroup.h and main functions in "main.cpp".
     2. Include a complete path when including third-party library functions. For example, under the Arduino IDE: #include "SD.h". In Keil, write: #include "SD/SD.h".
     3. It is not guaranteed to be compatible with all Arduino third-party library functions. Some libraries may need to be modified to operate normally. For specific modification methods, follow the compiler's prompts or submit the issues.
      
     Its is based on the STM32 Standard Peripheral Library (3.5 version) secondary package, part of the source code from the stm32duino, usage is basically the same as the Arduino, supports mixed use of Registers and Standard Peripheral Library functions.
     
     The APIs that are successfully ported on STM32F103C8T6/ZET6:
   Basic:
     Time:
          delay(Time_ms)
          delayMicroseconds(Time_us)
          millis()
          micros()
          TimerSet(TIMx,InterruptTime_us,Callback_function)
     Input Output:
          pinMode(Pin, Mode)
          attachInterrrupt(Pin,Trigger_Mode,Callback_function)
          digitalWrite(Pin,State)
          digitalRead(Pin)
          analogRead(Pin)
          analogWrite(Pin,DutyCycle)
          shiftOut(dataPin, clockPin, bitOrder, value)
          shiftIn(dataPin, clockPin, bitOrder)
          tone(Pin,Frequency,Time_ms)
          
     Print class // printf is supported
     String class
     Stream class
      
   Peripheral related:
     Serial class
     Wire class
     SPI class



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
