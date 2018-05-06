#include "usart.h"

volatile uint16_t _rx_buffer_head[SERIAL_NUM];
volatile uint16_t _rx_buffer_tail[SERIAL_NUM];		
uint8_t _rx_buffer[SERIAL_NUM][SERIAL_RX_BUFFER_SIZE];

void_func_point USART_func[SERIAL_NUM]={null_func, null_func, null_func};
  
void usart_init(USART_TypeDef* USARTx,uint32_t bound,uint8_t PreemptionPriority,uint8_t SubPriority)
{	
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	uint32_t Tx_Pin,Rx_Pin;
	uint32_t ItChannel;
	GPIO_TypeDef *GPIOx;
	
	switch((int)USARTx)
	{
		case (int)USART1:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	
			Tx_Pin = GPIO_Pin_9;
			Rx_Pin = GPIO_Pin_10;
			GPIOx = GPIOA;
			ItChannel = USART1_IRQn;
		}
		break;
		case (int)USART2:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
			Tx_Pin = GPIO_Pin_2;
			Rx_Pin = GPIO_Pin_3;
			GPIOx = GPIOA;
			ItChannel = USART2_IRQn;
		}
		break;
		case (int)USART3:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			Tx_Pin = GPIO_Pin_10;
			Rx_Pin = GPIO_Pin_11;
			GPIOx = GPIOB;
			ItChannel = USART3_IRQn;
		}
		break;
	}
	
	 
  
	//USART_TX
  GPIO_InitStructure.GPIO_Pin = Tx_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOx, &GPIO_InitStructure);//初始化
   
  //USART_RX
  GPIO_InitStructure.GPIO_Pin = Rx_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOx, &GPIO_InitStructure);//初始化

  //Usart NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = ItChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority ;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USARTx, &USART_InitStructure); //初始化串口
  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USARTx, ENABLE);                    //使能串口
}

void usart_putc(USART_TypeDef* USARTx,uint8_t ch)
{
  while((USARTx->SR&0X40)==0){};//循环发送,直到发送完毕   
  USARTx->DR = (u8) ch;
}

void usart_attachInterrupt(uint8_t USART_Num,void_func_point f)
{
	USART_func[USART_Num] = f;
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
		
		if(USART_func[0]!=null_func)
		{
			USART_func[0]();
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
		
		if(USART_func[1]!=null_func)
		{
			USART_func[1]();
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
		
		if(USART_func[2]!=null_func)
		{
			USART_func[2]();
		}
		
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}

