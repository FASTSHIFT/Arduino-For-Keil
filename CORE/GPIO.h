#ifndef __GPIO_H 
#define __GPIO_H   

#include "sys.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

#define PA0 (GPIOA_BASE)
#define PA1 (GPIOA_BASE+1)
#define PA2 (GPIOA_BASE+2)
#define PA3 (GPIOB_BASE+3)
#define PA4 (GPIOA_BASE+4)
#define PA5 (GPIOA_BASE+5)
#define PA6 (GPIOB_BASE+6)
#define PA7 (GPIOA_BASE+7)
#define PA8 (GPIOA_BASE+8)
#define PA9 (GPIOB_BASE+9)
#define PA10 (GPIOA_BASE+10)
#define PA11 (GPIOA_BASE+11)
#define PA12 (GPIOA_BASE+12)
#define PA13 (GPIOA_BASE+13)
#define PA14 (GPIOA_BASE+14)
#define PA15 (GPIOA_BASE+15)

#define PB0 (GPIOB_BASE)
#define PB1 (GPIOB_BASE+1)
#define PB2 (GPIOB_BASE+2)
#define PB3 (GPIOB_BASE+3)
#define PB4 (GPIOB_BASE+4)
#define PB5 (GPIOB_BASE+5)
#define PB6 (GPIOB_BASE+6)
#define PB7 (GPIOB_BASE+7)
#define PB8 (GPIOB_BASE+8)
#define PB9 (GPIOB_BASE+9)
#define PB10 (GPIOB_BASE+10)
#define PB11 (GPIOB_BASE+11)
#define PB12 (GPIOB_BASE+12)
#define PB13 (GPIOB_BASE+13)
#define PB14 (GPIOB_BASE+14)
#define PB15 (GPIOB_BASE+15)

#define PC0 (GPIOC_BASE)
#define PC1 (GPIOC_BASE+1)
#define PC2 (GPIOC_BASE+2)
#define PC3 (GPIOC_BASE+3)
#define PC4 (GPIOC_BASE+4)
#define PC5 (GPIOC_BASE+5)
#define PC6 (GPIOC_BASE+6)
#define PC7 (GPIOC_BASE+7)
#define PC8 (GPIOC_BASE+8)
#define PC9 (GPIOC_BASE+9)
#define PC10 (GPIOC_BASE+10)
#define PC11 (GPIOC_BASE+11)
#define PC12 (GPIOC_BASE+12)
#define PC13 (GPIOC_BASE+13)
#define PC14 (GPIOC_BASE+14)
#define PC15 (GPIOC_BASE+15)

#define PD0 (GPIOD_BASE)
#define PD1 (GPIOD_BASE+1)
#define PD2 (GPIOD_BASE+2)
#define PD3 (GPIOD_BASE+3)
#define PD4 (GPIOD_BASE+4)
#define PD5 (GPIOD_BASE+5)
#define PD6 (GPIOD_BASE+6)
#define PD7 (GPIOD_BASE+7)
#define PD8 (GPIOD_BASE+8)
#define PD9 (GPIOD_BASE+9)
#define PD10 (GPIOD_BASE+10)
#define PD11 (GPIOD_BASE+11)
#define PD12 (GPIOD_BASE+12)
#define PD13 (GPIOD_BASE+13)
#define PD14 (GPIOD_BASE+14)
#define PD15 (GPIOD_BASE+15)

#define PE0 (GPIOE_BASE)
#define PE1 (GPIOE_BASE+1)
#define PE2 (GPIOE_BASE+2)
#define PE3 (GPIOE_BASE+3)
#define PE4 (GPIOE_BASE+4)
#define PE5 (GPIOE_BASE+5)
#define PE6 (GPIOE_BASE+6)
#define PE7 (GPIOE_BASE+7)
#define PE8 (GPIOE_BASE+8)
#define PE9 (GPIOE_BASE+9)
#define PE10 (GPIOE_BASE+10)
#define PE11 (GPIOE_BASE+11)
#define PE12 (GPIOE_BASE+12)
#define PE13 (GPIOE_BASE+13)
#define PE14 (GPIOE_BASE+14)
#define PE15 (GPIOE_BASE+15)

#define PF0 (GPIOF_BASE)
#define PF1 (GPIOF_BASE+1)
#define PF2 (GPIOF_BASE+2)
#define PF3 (GPIOF_BASE+3)
#define PF4 (GPIOF_BASE+4)
#define PF5 (GPIOF_BASE+5)
#define PF6 (GPIOF_BASE+6)
#define PF7 (GPIOF_BASE+7)
#define PF8 (GPIOF_BASE+8)
#define PF9 (GPIOF_BASE+9)
#define PF10 (GPIOF_BASE+10)
#define PF11 (GPIOF_BASE+11)
#define PF12 (GPIOF_BASE+12)
#define PF13 (GPIOF_BASE+13)
#define PF14 (GPIOF_BASE+14)
#define PF15 (GPIOF_BASE+15)

#define PG0 (GPIOG_BASE)
#define PG1 (GPIOG_BASE+1)
#define PG2 (GPIOG_BASE+2)
#define PG3 (GPIOG_BASE+3)
#define PG4 (GPIOG_BASE+4)
#define PG5 (GPIOG_BASE+5)
#define PG6 (GPIOG_BASE+6)
#define PG7 (GPIOG_BASE+7)
#define PG8 (GPIOG_BASE+8)
#define PG9 (GPIOG_BASE+9)
#define PG10 (GPIOG_BASE+10)
#define PG11 (GPIOG_BASE+11)
#define PG12 (GPIOG_BASE+12)
#define PG13 (GPIOG_BASE+13)
#define PG14 (GPIOG_BASE+14)
#define PG15 (GPIOG_BASE+15)

void GPIO_Config(void);
void GPIO_begin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIOMode,GPIOSpeed_TypeDef GPIOSpeed);


/**********************************************************
**	模式配置参数：
**	GPIO_Mode_AIN 		模拟输入 				GPIO_Mode_IN_FLOATING 	浮空输入 		
**	GPIO_Mode_IPD 		下拉输入				GPIO_Mode_AF_OD 		复用开漏输出
**	GPIO_Mode_IPU 		上拉输入 				GPIO_Mode_Out_OD 		开漏输出 		
**	GPIO_Mode_Out_PP 	推挽输出				GPIO_Mode_AF_PP 		复用推挽输出	
**	GPIO_Pin的取值范围为 Pin0---Pin15	
***********************************************************/

/**********************************************************
**IO输出配置：
**位带操作：		PAout(1)=1;									//向PA7位写入1
**					if(PAout(1)==1);							//判断PA1是否等于1
**GPIO_SetBits(GPIOA,GPIO_Pin_10 | GPIO_Pin_15);				//设置指定的数据端口位
**GPIO_ResetBits(GPIOA,GPIO_Pin_10 | GPIO_Pin_15);				//清除指定的数据端口位
**GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10 | GPIO_Pin_15);		//读取指定IO的输入
**********************************************************/

#endif 
