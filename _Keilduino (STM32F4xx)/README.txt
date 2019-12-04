# A Cross-platform migration Project @FASTSHIFT
# Arduino for STM32F4xx

# https://github.com/FASTSHIFT/Arduino-For-Keil

注意: 
    此版本的标准外设库默认晶振频率是25MHz;
    若板载晶振为8MHz,需要将system_stm32f4xx.c -> 第254行 -> PLL_M定义 25 改为 8

//*********************************UPGRADE LOGS************************************//

//Finish in 2018.8.1    V_1.0 基于STM32F4xx标准外设库(1.3.0版)，移植了全部的ArduinoAPI
//Upgrade   2018.8.14   V_1.1 整理USART相关代码，修改Tone库只占用一个定时器(可使用toneSetTimer()函数切换),time_exti改名为timer
//Upgrade   2018.8.17   V_1.2 在Arduino.h添加了更完整的与GPIO寄存器交互函数，提升与第三方库的兼容性
//Upgrade   2018.8.17   V_1.3 添加WCharacter.h，支持Arduino有关Characters的函数，添加mcu_type.h，使Arduino.h可以跨MCU使用
//Upgrade   2018.11.28  V_1.4 增加了注释
//Upgrade   2019.2.17   V_1.5 提升效率以及兼容性,SPI库已可以正常工作
//Upgrade   2019.3.22   V_1.6 为SysTick设置最高中断优先级，防止被其他中断打断(非常重要!)
//Upgrade   2019.5.9    V_1.7 更新:ADC库、PWM库、定时器库、Servo库、PIN_MAP引脚外设映射表，Tone库已可以正常工作
//Upgrade   2019.6.3    V_1.8 pwm.c库将频率设定值从16位改为32位，并加入参数合法范围判断，ARDUINO宏定义从Arduino.h转至全局宏定义
//Upgrade   2019.7.12   V_1.9 Arduino.h里添加yield()与digitalPinToInterrupt(Pin)定义，更改analogRead_DMA()合法参数判断方式
//Upgrade   2019.8.22   V_2.0 同步_Keilduino(STM32F10x) V_7.0代码
//Upgrade   2019.8.24   V_2.1 digitalRead_Fast修改为digitalRead_FAST，修复 gpio.c 内 PIN_MAP 的 PC3、PC4、PC5 ADC通道的错误定义
//Upgrade   2019.9.28   V_2.2 更新SPI库
//Upgrade   2019.10.9   V_2.3 添加了STM32F4xx V1.8.0标准库，已支持新的F4型号，但是需要手动添加FLASH烧写算法文件，或者安装新的PACK
//Upgrade   2019.11.4   V_2.4 更新Print库(移植Georges Menie的printf-stdarg.c)，printf不再有长度限制，添加sprintf成员
//Upgrade   2019.12.1   V_2.5 更新timer库，使用独立的开根号运算函数，不依赖math.h库
//Upgrade   2019.12.4   V_2.6 工程框架修改，工程文件与代码文件分离，修复了digitalWrite_LOW复制粘贴导致的BUG  