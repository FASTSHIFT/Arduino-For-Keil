#include "exti.h"

void_func_point EXIT_func[16]={null_func, null_func, null_func, null_func, null_func, null_func, null_func, null_func, 
															 null_func, null_func, null_func, null_func, null_func, null_func, null_func, null_func};

void attach_EXTI(uint8_t Pin, void_func_point f, EXTITrigger_TypeDef Trigger_Mode, uint8_t PreemptionPriority, uint8_t SubPriority)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	IRQn_Type EXTIx_IRQn;
	uint8_t Pinx = Get_Pinx(Pin);
	
	if(Pinx>15)return;
	EXIT_func[Pinx] = f;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	//GPIO中断线以及中断初始化配置
	GPIO_EXTILineConfig(Get_GPIOx(Pin),Pinx);		//选择GPIO作为外部中断线路

	EXTI_InitStructure.EXTI_Line = 1 << Pinx;						//设置中断线
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;				//设置触发模式，中断触发（事件触发）
	EXTI_InitStructure.EXTI_Trigger = Trigger_Mode;			//设置触发方式
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	if(Pinx<=4)
	{
		switch(Pinx)
		{
			case 0:EXTIx_IRQn = EXTI0_IRQn;break;
			case 1:EXTIx_IRQn = EXTI1_IRQn;break;
			case 2:EXTIx_IRQn = EXTI2_IRQn;break;
			case 3:EXTIx_IRQn = EXTI3_IRQn;break;
			case 4:EXTIx_IRQn = EXTI4_IRQn;break;
		}
	}
	else if(Pinx >=5 && Pinx <=9) EXTIx_IRQn = EXTI9_5_IRQn;
	else if(Pinx >=10 && Pinx <=15) EXTIx_IRQn = EXTI15_10_IRQn;
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTIx_IRQn;					//使能所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;		//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;				//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 
}

void attachInterrupt(uint8_t Pin,void_func_point f,EXTITrigger_TypeDef Trigger_Mode)
{
	attach_EXTI(Pin,f,Trigger_Mode,0x02,0x01);
}


void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		EXIT_func[0]();
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1)!=RESET)
	{
		EXIT_func[1]();
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2)!=RESET)
	{
		EXIT_func[2]();
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
}

void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3)!=RESET)
	{
		EXIT_func[3]();
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
	{
		EXIT_func[4]();
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5)!=RESET)
	{
		EXIT_func[5]();
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	if(EXTI_GetITStatus(EXTI_Line6)!=RESET)
	{
		EXIT_func[6]();
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	if(EXTI_GetITStatus(EXTI_Line7)!=RESET)
	{
		EXIT_func[7]();
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	if(EXTI_GetITStatus(EXTI_Line8)!=RESET)
	{
		EXIT_func[8]();
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	if(EXTI_GetITStatus(EXTI_Line9)!=RESET)
	{
		EXIT_func[9]();
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line10)!=RESET)
	{
		EXIT_func[10]();
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	if(EXTI_GetITStatus(EXTI_Line11)!=RESET)
	{
		EXIT_func[11]();
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
	if(EXTI_GetITStatus(EXTI_Line12)!=RESET)
	{
		EXIT_func[12]();
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	if(EXTI_GetITStatus(EXTI_Line13)!=RESET)
	{
		EXIT_func[13]();
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	if(EXTI_GetITStatus(EXTI_Line14)!=RESET)
	{
		EXIT_func[14]();
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	if(EXTI_GetITStatus(EXTI_Line15)!=RESET)
	{
		EXIT_func[15]();
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}
