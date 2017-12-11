//Designed By _VIFEXTech
//2017.8.16 V_1.0

#ifndef NRF_H
#define NRF_H
extern "C"
{
//#include "stm32f10x.h"
#include "FileGroup.h"
//#include "Arduino.h"
}

//*************预指令**************//
#define _READ_REG 0x00
#define _WRITE_REG 0x20
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

#define RX_MODE 0
#define TX_MODE 1
#define	TXRX_MODE 2

//*************地址数据宽度**************//
#define ADR_WIDTH 5

#define Recon_Delay_Cnt 5//重连延时计数，范围在1~255
//值越大重连延时越大；也可以理解为是最大发送接收次数

//8:IRQ 	7:MISO
//6:MOSI 	5:SCK
//4:CSN 	3:CE
//2:VCC		1:GND

class NRF
{
	public:
		NRF(int _mosi, int _miso, int _sck, int _ce,int _csn);
		void init(byte mode, byte *buff, byte TX_length, byte RX_length);
		void SetAdress(byte *ADDR);
		byte Status();
		byte Tran(byte *txbuff);
		byte Recv(byte *rxbuff);
		byte Tran_Recv(byte *txbuff, byte *rxbuff);


	private:
		byte SPI_RW(unsigned char Byte);
		byte SPI_RW_Reg(byte reg,byte value);
		byte SPI_Read(byte reg);
		byte SPI_Write_Buf(byte reg,byte *pBuf,byte bytes);
		byte SPI_Read_Buf(byte reg,byte *pBuf,byte bytes);
		void TX_Mode(byte *txbuff);
		void RX_Mode();

		byte ADDRESS[ADR_WIDTH];
		byte TX_PLOAD_WIDTH;
		byte RX_PLOAD_WIDTH;
		int _MOSI;
		int _MISO;
		int _SCK;
		int _CE;
		int _CSN;
};

#endif
