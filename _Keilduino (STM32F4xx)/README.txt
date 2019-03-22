# A Cross-platform migration Project @FASTSHIFT
# Arduino for STM32F4xx

# https://github.com/FASTSHIFT/Arduino-For-Keil

注意{ 
	此版本的标准外设库默认晶振频率是25MHz;
	若板载晶振为8MHz,需要将stm32f4xx.h里的HSE_VALUE的定义改为 ((uint32_t)8000000),
	以及system_stm32f4xx.c里的PLL_M定义 25 改为 8 !
}

//*********************************UPGRADE LOGS************************************//

//Finish in 2018.8.1    V_1.0 基于STM32F4xx标志外设库(1.3.0版)，移植了全部的ArduinoAPI
//Upgrade   2018.8.14   V_1.1 整理USART相关代码，修改Tone库只占用一个定时器(可使用toneSetTimer()函数切换),time_exti改名为timer
//Upgrade   2018.8.17   V_1.2 在Arduino.h添加了更完整的与GPIO寄存器交互函数，提升与第三方库的兼容性
//Upgrade   2018.8.17   V_1.3 添加WCharacter.h，支持Arduino有关Characters的函数，添加mcu_type.h，使Arduino.h可以跨MCU使用
//Upgrade   2018.11.28  V_1.4 增加了注释
//Upgrade   2019.2.17   V_1.5 提升效率以及兼容性,SPI库已可以正常工作
//Upgrade   2019.3.22   V_1.6 为SysTick设置最高中断优先级，防止被其他中断打断(非常重要!)