# Arduino_For_Keil @FASTSHIFT
# For STM32F4xx

//*********************************UPGRADE LOGS************************************//

//Finish in 2018.8.1    V_1.0 基于STM32F4xx标志外设库(1.3.0版)，移植了全部的ArduinoAPI
//Upgrade  2018.8.14    V_1.1 整理USART相关代码，修改Tone库只占用一个定时器(可使用toneSetTimer()函数切换),time_exti改名为timer
//Upgrade       8.17    V_1.2 在Arduino.h添加了更完整的与GPIO寄存器交互函数，提升与第三方库的兼容性
//Upgrade       8.17    V_1.3 添加WCharacter.h，支持Arduino有关Characters的函数，添加mcu_type.h，使Arduino.h可以跨MCU使用
//Upgrade      11.28    V_1.4 增加了注释 