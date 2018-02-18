#include "spi.h"
#include "Arduino.h"

void SPI_begin(SPI_TypeDef* SPIx)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	switch((int)SPIx)
	{
		case (int)SPI1:
			RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE);
//			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
//			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出	
//			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//			GPIO_Init(GPIOA, &GPIO_InitStructure);
//			GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
			pinMode(PA5,GPIO_Mode_AF_PP);
			pinMode(PA6,GPIO_Mode_AF_PP);
			pinMode(PA7,GPIO_Mode_AF_OD);
			break;
		case (int)SPI2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
			RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
			break;
	}
}

void SPI_Settings(	SPI_TypeDef* SPIx,
										u16 SPI_Mode_MS,
										u16 SPI_DataSize,
										u16 SPI_MODE,
										u16 SPI_NSS,
										u16 SPI_BaudRatePrescaler,
										u16 SPI_FirstBit)
{
	SPI_InitTypeDef  SPI_InitStructure;
	u16 SPI_CPOL,SPI_CPHA;
	SPI_Cmd(SPIx,DISABLE);
	
	switch(SPI_MODE)
	{
		case 0:
			SPI_CPOL = SPI_CPOL_Low;
			SPI_CPHA = SPI_CPHA_1Edge;
			break;
		case 1:
			SPI_CPOL = SPI_CPOL_Low;
			SPI_CPHA = SPI_CPHA_2Edge;
			break;
		case 2:
			SPI_CPOL = SPI_CPOL_High;
			SPI_CPHA = SPI_CPHA_1Edge;
			break;
		case 3:
			SPI_CPOL = SPI_CPOL_High;
			SPI_CPHA = SPI_CPHA_2Edge;
			break;
	}

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_MS;		//设置SPI工作模式:(SPI_Mode_Master,SPI_Mode_Slave)
	SPI_InitStructure.SPI_DataSize = SPI_DataSize;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制(SPI_NSS_Soft,SPI_NSS_Hard)
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;		//定义波特率预分频的值
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始(SPI_FirstBit_MSB,SPI_FirstBit_LSB)
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPIx, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPIx, ENABLE); //使能SPI外设
	
	//SPI1_SetSpeed(SPI_BaudRatePrescaler_8); //spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）  
	
	//SPI1_ReadWriteByte(0xff);//启动传输		 
}

void SPI_setBitOrder(SPI_TypeDef* SPIx,u16 SPI_FirstBit)
{
	SPI_InitTypeDef  SPI_InitStructure;
	SPI_Cmd(SPIx,DISABLE);
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit;
	SPI_Init(SPIx, &SPI_InitStructure);
	SPI_Cmd(SPIx, ENABLE);
}

void SPI_setDataSize(SPI_TypeDef* SPIx,u16 datasize)
{
	SPI_InitTypeDef  SPI_InitStructure;
	SPI_Cmd(SPIx,DISABLE);
	SPI_InitStructure.SPI_FirstBit = datasize;
	SPI_Init(SPIx, &SPI_InitStructure);
	SPI_Cmd(SPIx, ENABLE);
}

void SPI_setDataMode(SPI_TypeDef* SPIx,uint8_t dataMode)
{
	SPI_InitTypeDef  SPI_InitStructure;
	
	u16 SPI_CPOL,SPI_CPHA;
	SPI_Cmd(SPIx,DISABLE);
	
	switch(dataMode)
	{
		case 0:
			SPI_CPOL = SPI_CPOL_Low;
			SPI_CPHA = SPI_CPHA_1Edge;
			break;
		case 1:
			SPI_CPOL = SPI_CPOL_Low;
			SPI_CPHA = SPI_CPHA_2Edge;
			break;
		case 2:
			SPI_CPOL = SPI_CPOL_High;
			SPI_CPHA = SPI_CPHA_1Edge;
			break;
		case 3:
			SPI_CPOL = SPI_CPOL_High;
			SPI_CPHA = SPI_CPHA_2Edge;
			break;
	}
	
	SPI_InitStructure.SPI_CPOL = SPI_CPOL;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA;	//数据捕获于第二个时钟沿
	SPI_Init(SPIx, &SPI_InitStructure);
	SPI_Cmd(SPIx, ENABLE);
}


//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)

void SPI_setClockDivider(SPI_TypeDef* SPIx,u16 clockDivider)
{
	SPI_InitTypeDef  SPI_InitStructure;
	SPI_Cmd(SPIx,DISABLE);
	SPI_InitStructure.SPI_BaudRatePrescaler = clockDivider;
	SPI_Init(SPIx, &SPI_InitStructure);
	SPI_Cmd(SPIx,ENABLE);
	//SPIx->CR1&=0XFFC7; 
	//SPIx->CR1|=SpeedSet;	//设置SPI速度  
	//SPIx->CR1|=1<<6; 		//SPI设备使能 
}

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI_ReadWriteByte(SPI_TypeDef* SPIx,u8 TxData)
{		
	u8 retry=0,data;		
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPIx, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)return 0;
	}
		
	data=SPI_I2S_ReceiveData(SPIx); //返回通过SPIx最近接收的数据	
	return data;				    
}































