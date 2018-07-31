#include "usart.h"

volatile uint16_t _rx_buffer_head[SERIAL_NUM];
volatile uint16_t _rx_buffer_tail[SERIAL_NUM];		
volatile uint8_t _rx_buffer[SERIAL_NUM][SERIAL_RX_BUFFER_SIZE];

void_func_point USART_Function[SERIAL_NUM]={null_func, null_func, null_func};
  
void usart_init(USART_TypeDef* USARTx,uint32_t BaudRate,uint8_t ChannelPriority)
{	
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	uint16_t Tx_Pin, Rx_Pin;
	uint16_t ItChannel;
	uint32_t RCC_AHBPeriph_GPIOx;
	GPIO_TypeDef *GPIOx;
	
	if(USARTx == USART1)
	{
#if defined (STM32F030F4P6)
		Tx_Pin = GPIO_Pin_2;
		Rx_Pin = GPIO_Pin_3;
#else
		Tx_Pin = GPIO_Pin_9;
		Rx_Pin = GPIO_Pin_10;
#endif
		GPIOx = GPIOA;
		RCC_AHBPeriph_GPIOx = RCC_AHBPeriph_GPIOA;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		ItChannel = USART1_IRQn;
	}
	else if(USARTx == USART2)
	{
		Tx_Pin = GPIO_Pin_2;
		Rx_Pin = GPIO_Pin_3;
		GPIOx = GPIOA;
		RCC_AHBPeriph_GPIOx = RCC_AHBPeriph_GPIOA;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		ItChannel = USART2_IRQn;
	}
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOx, ENABLE);  
	
  GPIO_PinAFConfig(GPIOx, Get_GPIO_PinSource(Tx_Pin), GPIO_AF_1);
  GPIO_PinAFConfig(GPIOx, Get_GPIO_PinSource(Rx_Pin), GPIO_AF_1);
   
  GPIO_InitStructure.GPIO_Pin =  Tx_Pin | Rx_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOx, &GPIO_InitStructure);

	//Usart NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = ItChannel;
  NVIC_InitStructure.NVIC_IRQChannelPriority = ChannelPriority;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
  
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = BaudRate;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USARTx, &USART_InitStructure); //初始化串口

  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USARTx, ENABLE);                   //使能串口
}

void usart_putc(USART_TypeDef* USARTx,uint8_t ch)
{
	while(!((USARTx->ISR) & USART_ISR_TC)){};
  USARTx->TDR = ch;
}

void usart_attachInterrupt(uint8_t USART_Num,void_func_point f)
{
	USART_Function[USART_Num] = f;
}

//**********************USARTx_IRQHandler****************************//

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		uint8_t c = USART_ReceiveData(USART1);
		uint16_t i = (uint16_t)(_rx_buffer_head[0] + 1) % SERIAL_RX_BUFFER_SIZE;
		if (i != _rx_buffer_tail[0]) 
		{
			_rx_buffer[0][_rx_buffer_head[0]] = c;
			_rx_buffer_head[0] = i;
		}
		
		if(USART_Function[0]!=null_func)
		{
			USART_Function[0]();
		}
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		uint8_t c = USART_ReceiveData(USART2);
		uint16_t i = (uint16_t)(_rx_buffer_head[1] + 1) % SERIAL_RX_BUFFER_SIZE;
		if (i != _rx_buffer_tail[1]) 
		{
			_rx_buffer[1][_rx_buffer_head[1]] = c;
			_rx_buffer_head[1] = i;
		}
		
		if(USART_Function[1]!=null_func)
		{
			USART_Function[1]();
		}
		
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		uint8_t c = USART_ReceiveData(USART3);
		uint16_t i = (uint16_t)(_rx_buffer_head[2] + 1) % SERIAL_RX_BUFFER_SIZE;
		if (i != _rx_buffer_tail[2]) 
		{
			_rx_buffer[2][_rx_buffer_head[2]] = c;
			_rx_buffer_head[2] = i;
		}
		
		if(USART_Function[2]!=null_func)
		{
			USART_Function[2]();
		}
		
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}

