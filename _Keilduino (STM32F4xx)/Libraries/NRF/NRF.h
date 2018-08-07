//Designed By _VIFEXTech
//2017.8.16		V_1.0
//12.5				V_1.1 增加初始化函数，以及地址设置函数
//2018.6.16		V_1.2 支持双向半双工数据回传
//6.18				V_1.3 加入模式设定合法性检测，支持使用指针获取模块地址
//6.19				V_1.4 支持硬件SPI，更高效的IO操作

#ifndef __NRF_H
#define __NRF_H

#include "Arduino.h"

//#define USE_HARDWARE_SPI

#ifdef USE_HARDWARE_SPI
	#include "SPI.h"
	#define SPI_Transfer(x) SPI.transfer(x)
	
#if defined (__AVR__)
	#define SPI_BEGIN() {SPI.begin(), SPI.setDataMode(SPI_MODE0), SPI.setClockDivider(SPI_2XCLOCK_MASK);}
#elif defined(__STM32F1__)
	#define SPI_BEGIN() {SPI.begin(10000000, MSBFIRST, SPI_MODE0), SPI_Cmd(SPI1,ENABLE);}
#else 
	#warning "Please Setup Hardware SPI"
#endif

#endif //USE_HARDWARE_SPI

#if defined(__STM32F1__)
	#define NRF_MISO_READ() digitalRead(_MISO)
	#define NRF_MOSI_HIGH()	digitalWrite_HIGH(_MOSI)
	#define NRF_MOSI_LOW() 	digitalWrite_LOW(_MOSI)
	#define NRF_SCK_HIGH()	digitalWrite_HIGH(_SCK)
	#define NRF_SCK_LOW() 	digitalWrite_LOW(_SCK)
	#define NRF_CE_HIGH()		digitalWrite_HIGH(_CE)
	#define NRF_CE_LOW() 		digitalWrite_LOW(_CE)
	#define NRF_CSN_HIGH() 	digitalWrite_HIGH(_CSN)
	#define NRF_CSN_LOW() 	digitalWrite_LOW(_CSN)
#else
	#define NRF_MISO_READ() digitalRead(_MISO)
	#define NRF_MOSI_HIGH()	digitalWrite(_MOSI,1)
	#define NRF_MOSI_LOW() 	digitalWrite(_MOSI,0)
	#define NRF_SCK_HIGH()	digitalWrite(_SCK,1)
	#define NRF_SCK_LOW() 	digitalWrite(_SCK,0)
	#define NRF_CE_HIGH() 	digitalWrite(_CE,1)
	#define NRF_CE_LOW() 		digitalWrite(_CE,0)
	#define NRF_CSN_HIGH() 	digitalWrite(_CSN,1)
	#define NRF_CSN_LOW() 	digitalWrite(_CSN,0)
#endif

//*************预指令**************//
#define NRF_READ_REG 0x00
#define NRF_WRITE_REG 0x20
#define RD_RX_PLOAD 0x61
#define WR_TX_PLOAD 0xA0
#define FLUSH_TX 0xE1
#define FLUSH_RX 0xE2
#define REUSE_TX_PL 0xE3
#define NOP 0xFF
//***********中断标志位************//
#define RX_DR 0x40
#define TX_DS 0x20
#define MAX_RT 0x10
//**************寄存器*************//
#define CONFIG 0x00
#define EN_AA 0x01
#define EN_RXADDR 0x02
#define SETUP_AW 0x03
#define SETUP_RETR 0x04
#define RF_CH 0x05
#define RF_SETUP 0x06
#define STATUS_ 0x07
#define OBSERVE_TX 0x08
#define CD 0x09
#define RX_ADDR_P0 0x0A
#define RX_ADDR_P1 0x0B
#define RX_ADDR_P2 0x0C
#define RX_ADDR_P3 0x0D
#define RX_ADDR_P4 0x0E
#define RX_ADDR_P5 0x0F
#define TX_ADDR 0x10
#define RX_PW_P0 0x11
#define RX_PW_P1 0x12
#define RX_PW_P2 0x13
#define RX_PW_P3 0x14
#define RX_PW_P4 0x15
#define RX_PW_P5 0x16
#define FIFO_STATUS 0x17

typedef enum
{
	RX_MODE,
	TX_MODE,
	TXRX_MODE,
	RXTX_MODE
}NRF_Mode_TypeDef;

//*************数据宽度**************//
#define ADR_WIDTH 5

//8:IRQ 	7:MISO
//6:MOSI 	5:SCK
//4:CSN 	3:CE
//2:VCC		1:GND

class NRF
{
	public:
		NRF(uint8_t _mosi, uint8_t _miso, uint8_t _sck, uint8_t _ce, uint8_t _csn);
#ifdef USE_HARDWARE_SPI
		NRF(uint8_t _ce, uint8_t _csn);
#endif
		void SetAddress(uint8_t addr0, uint8_t addr1, uint8_t addr2, uint8_t addr3, uint8_t addr4);
		uint8_t GetAddress(uint8_t addr);
		void init(NRF_Mode_TypeDef mode, uint8_t RX_length);
		void init(NRF_Mode_TypeDef mode, uint8_t *buff, uint8_t TX_length);
		void init(NRF_Mode_TypeDef mode, uint8_t *buff, uint8_t TX_length, uint8_t RX_length);
		uint8_t Status();
		uint8_t Tran(uint8_t *txbuff);
		uint8_t Recv(uint8_t *rxbuff);
		uint8_t TranRecv(uint8_t *txbuff, uint8_t *rxbuff);
		uint8_t RecvTran(uint8_t *rxbuff, uint8_t *txbuff);

	private:
		uint8_t SPI_RW(uint8_t Data);
		uint8_t SPI_RW_Reg(uint8_t reg,uint8_t value);
		uint8_t SPI_Read(uint8_t reg);
		uint8_t SPI_Write_Buf(uint8_t reg,uint8_t *pBuf,uint8_t bytes);
		uint8_t SPI_Read_Buf(uint8_t reg,uint8_t *pBuf,uint8_t bytes);
		void TX_Mode(uint8_t *txbuff);
		void RX_Mode();

		uint8_t ADDRESS[ADR_WIDTH];
		uint8_t TX_PLOAD_WIDTH;
		uint8_t RX_PLOAD_WIDTH;
		uint8_t _MOSI;
		uint8_t _MISO;
		uint8_t _SCK;
		uint8_t _CE;
		uint8_t _CSN;
};

#endif
