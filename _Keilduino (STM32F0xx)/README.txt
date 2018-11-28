# Arduino_For_Keil @FASTSHIFT
# For STM32F0xx

//*********************************UPGRADE LOGS************************************//

//Finish in 2018.7.29  V_1.0 基于STM32F0xx标志外设库(1.0.1版)，移植了全部的ArduinoAPI
//Upgrade        8.14  V_1.1 整理USART相关代码，修改Tone库只占用一个定时器(可使用toneSetTimer()函数切换),time_exti改名为timer
//Upgrade        8.17  V_1.2 在Arduino.h添加了更完整的与GPIO寄存器交互函数，提升与第三方库的兼容性
//Upgrade        8.20  V_1.3 添加WCharacter.h，支持Arduino有关Characters的函数，添加mcu_type.h，使Arduino.h可以跨MCU使用
//Upgrade        10.9  V_1.4 添加注释，更新Tone、HardwareSerial，去除usart.c usart.h
//Upgrade        11.21 V_1.5 更新Arduino.c