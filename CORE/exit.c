#include "exit.h"

//外部中断初始化函数
void EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟

/******************************EXIT配置初始化***********************************************/
		//GPIOE.5 中断线以及中断初始化配置
/*1*/  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);		//选择GPIO作为外部中断线路

/*2*/  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;						//设置中断线
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;				//设置触发模式，中断触发（事件触发）
/*3*/ 	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;			//设置触发方式，上升沿触发
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    //GPIOC.3	  中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC ,GPIO_PinSource5);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

/******************************NVIC配置初始化***********************************************/
 
/*4*/  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;					//使能按键所在的外部中断通道
/*5*/		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;		//抢占优先级2 
/*5*/		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;				//子优先级2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//使能外部中断通道
		NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
		
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;					//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;		//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;				//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
 

}

/**********************************配置中断函数***************************/
 
void EXTI3_IRQHandler(void)
{
	int status;
	
	if(EXTI_GetITStatus(EXTI_Line3)!=RESET)		//判断中断线是否产生中断
	{
		delay_ms(10);    //消抖			 
		if(PEout(3)==0)	
		{
			status=PEout(5);
			PEout(5)=~status;
		}
		delay_ms(10);  
		EXTI_ClearITPendingBit(EXTI_Line3);  	//清除LINE15线路挂起位
	}
}

void EXTI9_5_IRQHandler(void)
{
	int i=10;

	if(EXTI_GetITStatus(EXTI_Line5)!=RESET)		//判断中断线是否产生中断
	{
		delay_ms(10);    //消抖			
		if(PEout(5)==1)	
		{
			PEout(6)=0;
			while(i--)
			{
				delay_ms(200);
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line5);  //清除LINE15线路挂起位
	}
}
