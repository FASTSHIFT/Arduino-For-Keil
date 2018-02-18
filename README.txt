# Arduino_For_Keil

    这个项目可以使你在Keil MDK5环境下也可以使用Arduino的函数对STM32F103系列单片机进行编程。它可以兼容Arduino函数库，只需将函数库的文件夹拖入 "Library" 文件夹下即可，使用时可以直接复制Arduino的源代码，注意include的时候要加入完整的路径，例如在Arduino IDE下：#include"SD.h"，在Keil中就要写成：#include"SD/SD.h"。有的函数库可能要进行修改才能正常运行，例如最常见的将引脚作为参数传递时Arduino一般是 "uint8_t" 类型，而本项目使用的是 "int" 类型，所以需要将函数库其中代表引脚的变量全部改成 "int" 类型才能正常运行。
    
    它的底层是基于STM32标准外设库(3.5版本)二次封装。因为本人目前还在学习阶段，所以这个移植工程还有不少BUG，目前只是成功测试了部分的函数。
目前在STM32F103C8T6上测试函数有：
  基本：
    时间：delay(),delayMicroseconds(),millis(),micros();
    输入输出：pinMode(),digitalWrite(),digitalRead(),analogRead(), analogWrite(),shiftOut(),shiftIn();
    Print类
    String类
    Stream类
    
  外设相关：
    Serial类
    Wire类
    SPI类（不支持）
    

  This project allows you to program the STM32F103 series of microcontrollers using the functions of Arduino in Keil MDK5 environment as well. It can be compatible with Arduino function library, simply drag the library folder into the Library folder, you can directly copy the Arduino source code, be careful when include the full path, for example, in the Arduino IDE: #include "SD.h", in Keil it should be written as: #include "SD/SD.h". Some library may need to be modified to work properly, such as the most common Arduino when the pin is passed as a parameter is generally "uint8_t" type, and the project uses the "int" type, so you need to function library which represents Feet all the variables into "int" type to work properly.
  
  It is based on the STM32 Standard Peripheral Library (V 3.5) encapsulation. because I am still in the learning, so this transplant project still has many BUGs. At present, I have successfully tested some of the functions.
  
  Currently on the STM32F103C8T6 test functions are:            
    Basic:
      Time: delay(), delayMicroseconds(), millis(), micros();
      Input output: pinMode(), digitalWrite(), digitalRead(), analogRead(), analogWrite(), shiftOut(), shiftIn();
      Print class 
      String class
      Stream class
      
    Peripherals:
      Serial class
      Wire class
      SPI class(Unsupport)


//*********************************UPGRADE LOGS************************************//

//Finish in 2017.11.20 V_1.0 支持pinMode,digitalWrite,digitalRead,delay等基础Arduino API
//Upgrade 			    11.21 V_1.1 改用C++编写程序，可以兼容部分Arduino函数库，只需将库拖入Library文件夹下即可
//Upgrade 			    11.23 V_1.2 移植Print、Stream、WString等class
//Upgrade 			    12.12 V_1.3 支持millis、micros、Wire class、部分Serial class
//Upgrade   2018.1.10  V_1.4 全面支持Serial class，编写了定时器库
//Upgrade 			    1.12  V_1.5 尝试加入SD卡支持
//Upgrade 			    1.21  V_1.6 支持analogRead(方式：DMA/No DMA)、analogWrite(频率和占空比可调)
//Upgrade 			    1.23  V_1.7 支持attachInterrupt，所有GPIO都可以配置外部中断
//Upgrade 			    1.31  V_1.8 TIM2~TIM4以可以正常输出PWM
//Upgrade 			    2.1 	 V_1.9 HardwareSerial class加入end()、attachInterrupt()
//Upgrade 			    2.10  V_2.0 修改函数指针定义方式
