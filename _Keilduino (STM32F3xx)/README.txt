# Arduino_For_Keil @FASTSHIFT
# For STM32F30x

//*********************************UPGRADE LOGS************************************//

//Finish in 2018.10.24 V_1.0 基于STM32F30x标准外设库(1.2.3版)，移植了大部分的ArduinoAPI
//Upgrade   2019.3.22  V_1.1 为SysTick设置最高中断优先级，防止被其他中断打断(非常重要!)
//Upgrade   2019.9.10  V_1.2 同步了_Keilduino(STM32F4xx)大部分功能
//Upgrade   2019.11.4  V_1.3 更新Print库(移植Georges Menie的printf-stdarg.c)，printf不再有长度限制，添加sprintf成员
//Upgrade   2020.1.15  V_1.4 由于Print库的printf不支持浮点输出，将Print库还原，使用vsnprintf转换printf 