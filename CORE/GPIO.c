#include "GPIO.h"

/**********************************************************
** 函数名:	 GPIO_Config
** 功能描述: 所有的GPIO口配置放在这里
** 输入参数: 无
** 输出参数: 无
***********************************************************/
void GPIO_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;  //定义GPIO结构体	

/**1**/	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOC, ENABLE);	//使能GPIO口时钟
	
/**2**/	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;						//IO引脚设置
/**3**/	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出			//IO模式配置
/**4**/	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//IO速率配置
/**5**/	GPIO_Init(GPIOE, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void GPIO_begin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIOMode, GPIOSpeed_TypeDef GPIOSpeed)
{
	GPIO_InitTypeDef GPIO_InitStructure;  //定义GPIO结构体
	
			 if(GPIOx == GPIOA)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能GPIO口时钟
	else if(GPIOx == GPIOB)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else if(GPIOx == GPIOC)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	else if(GPIOx == GPIOD)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	else if(GPIOx == GPIOE)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	else if(GPIOx == GPIOF)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	else if(GPIOx == GPIOG)RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	else return;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIOMode;
	GPIO_InitStructure.GPIO_Speed = GPIOSpeed;		
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}
