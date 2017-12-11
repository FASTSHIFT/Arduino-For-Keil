#include "NRF.h"

byte ADDRESS[ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01};

byte TX_PLOAD_WIDTH;
byte RX_PLOAD_WIDTH;

int _MOSI;
int _MISO;
int _SCK;
int _CE;
int _CSN;

NRF::NRF(int _mosi, int _miso, int _sck, int _ce,int _csn)
{
  _MOSI=_mosi;
  _MISO=_miso;
  _SCK=_sck;
  _CE=_ce;
  _CSN=_csn;
}

void NRF::init(byte mode, byte *buff, byte TX_length, byte RX_length)
{
  pinMode(_CE, OUTPUT);
  pinMode(_CSN, OUTPUT);
  pinMode(_MOSI, OUTPUT);
  pinMode(_MISO, INPUT);
  pinMode(_SCK, OUTPUT);

  digitalWrite(_CE,LOW);
  digitalWrite(_CSN,HIGH);

  switch(mode)
  {
    case RX_MODE:
      RX_PLOAD_WIDTH=RX_length;
      RX_Mode();
      break;
    
    case TX_MODE:
      TX_PLOAD_WIDTH=TX_length;
      TX_Mode(buff);
      break;
    
    case TXRX_MODE:
      RX_PLOAD_WIDTH=RX_length;
      TX_PLOAD_WIDTH=TX_length;
      TX_Mode(buff);
      break;
  }
}

void NRF::SetAdress(byte *ADDR)
{
	for(byte i=0;i<ADR_WIDTH;i++)
	{
		ADDRESS[i]=ADDR[i];
	}
}

byte NRF::SPI_RW(unsigned char Byte)
{
  for(byte i=0;i<8;i++)
  {
    if(Byte&0x80) digitalWrite(_MOSI,HIGH);
    else digitalWrite(_MOSI,LOW);

    digitalWrite(_SCK,HIGH);
    Byte<<=1;
    if(digitalRead(_MISO)==1) Byte|=1;
    digitalWrite(_SCK,LOW);
  }

  return Byte;
}

byte NRF::SPI_RW_Reg(byte reg,byte value)
{
  byte status_;

  digitalWrite(_CSN,LOW);
  status_=SPI_RW(reg);
  SPI_RW(value);
  digitalWrite(_CSN,HIGH);
  return status_;
}

byte NRF::SPI_Read(byte reg)
{
  byte reg_val;
  digitalWrite(_CSN,LOW);
  SPI_RW(reg);
  reg_val=SPI_RW(0);
  digitalWrite(_CSN,HIGH);
  return reg_val;
}

byte NRF::Status(void)
{
  //if(SPI_Read(STATUS_)!=0)return 1;
  return SPI_Read(STATUS_);
}

byte NRF::SPI_Write_Buf(byte reg,byte *pBuf,byte bytes)
{
  byte status_;

  digitalWrite(_CSN,LOW);
  status_=SPI_RW(reg);
  for(byte i=0;i<bytes;i++) SPI_RW(*pBuf++);
  digitalWrite(_CSN,HIGH);
  return status_;
}

byte NRF::SPI_Read_Buf(byte reg,byte *pBuf,byte bytes)
{
  byte status_;

  digitalWrite(_CSN,LOW);
  status_=SPI_RW(reg);
  for(byte i=0;i<bytes;i++) pBuf[i]=SPI_RW(0);
  digitalWrite(_CSN,HIGH);
  return status_;
}

void NRF::TX_Mode(byte *txbuff)
{
  digitalWrite(_CE,LOW);
  SPI_Write_Buf(_WRITE_REG+TX_ADDR,ADDRESS,ADR_WIDTH);
  SPI_Write_Buf(_WRITE_REG+RX_ADDR_P0,ADDRESS,ADR_WIDTH);

  SPI_RW_Reg(_WRITE_REG+EN_AA,0x01);
  SPI_RW_Reg(_WRITE_REG+EN_RXADDR,0x01);
  SPI_RW_Reg(_WRITE_REG+CONFIG,0x0E);
  SPI_RW_Reg(_WRITE_REG+RF_CH,40);
  SPI_RW_Reg(_WRITE_REG+RF_SETUP,0x07);
  SPI_RW_Reg(_WRITE_REG+SETUP_RETR,0x1A);
  SPI_Write_Buf(WR_TX_PLOAD,txbuff,TX_PLOAD_WIDTH);

  digitalWrite(_CE,HIGH);
}

void NRF::RX_Mode()
{
  digitalWrite(_CE,LOW);
  SPI_Write_Buf(_WRITE_REG+RX_ADDR_P0,ADDRESS,ADR_WIDTH);
  SPI_RW_Reg(_WRITE_REG+EN_AA,0x01);
  SPI_RW_Reg(_WRITE_REG+EN_RXADDR,0x01);
  SPI_RW_Reg(_WRITE_REG+RF_CH,40);
  SPI_RW_Reg(_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);
  SPI_RW_Reg(_WRITE_REG+RF_SETUP,0x07);
  SPI_RW_Reg(_WRITE_REG+CONFIG,0x0F);
  digitalWrite(_CE,HIGH);
}

byte NRF::Tran(byte *txbuff)
{
  byte status_=SPI_Read(STATUS_);
  if(status_&TX_DS)
  {
    SPI_RW_Reg(FLUSH_TX,0);
    SPI_Write_Buf(WR_TX_PLOAD,txbuff,TX_PLOAD_WIDTH);
  }
  if(status_&MAX_RT)
  {
    SPI_RW_Reg(FLUSH_TX,0);
    SPI_Write_Buf(WR_TX_PLOAD,txbuff,TX_PLOAD_WIDTH);
  }
  SPI_RW_Reg(_WRITE_REG+STATUS_,status_);
  return status_;
}

byte NRF::Recv(byte *rxbuff)
{
  byte status_=SPI_Read(STATUS_);
  if(status_&RX_DR)
  {
    SPI_Read_Buf(RD_RX_PLOAD,rxbuff,RX_PLOAD_WIDTH);
    SPI_RW_Reg(FLUSH_RX,0);
  }
  SPI_RW_Reg(_WRITE_REG+STATUS_,status_);
  return status_;
}

byte NRF::Tran_Recv(byte *txbuff, byte *rxbuff)
{
	static byte Tx_Cnt=0,Rx_Cnt=0;
	static byte TXMode=0,ret=0;
	if(TXMode)
	{
		if(Tran(txbuff)==TX_DS||Tran(txbuff)==MAX_RT||Tx_Cnt==Recon_Delay_Cnt)
		{
			RX_Mode();
			ret=0;
			TXMode=0;
			Tx_Cnt=0;
		}
		Tx_Cnt++;
	}
	else
	{
		if(Recv(rxbuff)==RX_DR||Rx_Cnt==Recon_Delay_Cnt)
		{
			TX_Mode(txbuff);
			ret=1;
			TXMode=1;
			Rx_Cnt=0;
		}
		Rx_Cnt++;
	}
	return ret;
}
