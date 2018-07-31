#include "exti.h"

void_func_point EXTI_Function[16]={null_func, null_func, null_func, null_func, null_func, null_func, null_func, null_func, 
															 null_func, null_func, null_func, null_func, null_func, null_func, null_func, null_func};

void attach_EXTI(uint8_t Pin, void_func_point f, EXTITrigger_TypeDef Trigger_Mode, uint8_t ChannelPriority)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	IRQn_Type EXTIx_IRQn;
	uint8_t Pinx = Get_Pinx(Pin);
	
	if(Pinx>15)return;
	EXTI_Function[Pinx] = f;
	
	//GPIO中断线以及中断初始化配置
	SYSCFG_EXTILineConfig(Get_EXTI_PortSourceGPIOx(Pin),Get_EXTI_PinSourcex(Pin));

	EXTI_InitStructure.EXTI_Line = Get_EXTI_Linex(Pin);						//设置中断线
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;				//设置触发模式，中断触发（事件触发）
	EXTI_InitStructure.EXTI_Trigger = Trigger_Mode;			//设置触发方式
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	if(Pinx <= 1)
	{
		EXTIx_IRQn = EXTI0_1_IRQn;
	}
	else if(Pinx >=2 && Pinx <=3) EXTIx_IRQn = EXTI2_3_IRQn;
	else if(Pinx >=4 && Pinx <=15) EXTIx_IRQn = EXTI4_15_IRQn;
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTIx_IRQn;					//使能所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPriority = ChannelPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 
}

void attachInterrupt(uint8_t Pin,void_func_point f,EXTITrigger_TypeDef Trigger_Mode)
{
	attach_EXTI(Pin,f,Trigger_Mode,0x02);
}


void EXTI0_1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		EXTI_Function[0]();
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
	if(EXTI_GetITStatus(EXTI_Line1)!=RESET)
	{
		EXTI_Function[1]();
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

void EXTI2_3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2)!=RESET)
	{
		EXTI_Function[2]();
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
	if(EXTI_GetITStatus(EXTI_Line3)!=RESET)
	{
		EXTI_Function[3]();
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}

void EXTI4_15_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
	{
		EXTI_Function[4]();
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
	if(EXTI_GetITStatus(EXTI_Line5)!=RESET)
	{
		EXTI_Function[5]();
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	if(EXTI_GetITStatus(EXTI_Line6)!=RESET)
	{
		EXTI_Function[6]();
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	if(EXTI_GetITStatus(EXTI_Line7)!=RESET)
	{
		EXTI_Function[7]();
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	if(EXTI_GetITStatus(EXTI_Line8)!=RESET)
	{
		EXTI_Function[8]();
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	if(EXTI_GetITStatus(EXTI_Line9)!=RESET)
	{
		EXTI_Function[9]();
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
	
	if(EXTI_GetITStatus(EXTI_Line10)!=RESET)
	{
		EXTI_Function[10]();
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	if(EXTI_GetITStatus(EXTI_Line11)!=RESET)
	{
		EXTI_Function[11]();
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
	if(EXTI_GetITStatus(EXTI_Line12)!=RESET)
	{
		EXTI_Function[12]();
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	if(EXTI_GetITStatus(EXTI_Line13)!=RESET)
	{
		EXTI_Function[13]();
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	if(EXTI_GetITStatus(EXTI_Line14)!=RESET)
	{
		EXTI_Function[14]();
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	if(EXTI_GetITStatus(EXTI_Line15)!=RESET)
	{
		EXTI_Function[15]();
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}
