#include "NRF.h"

byte ADDRESS[ADR_WIDTH] = {10, 20, 30, 40, 50};

NRF::NRF(uint8_t _mosi, uint8_t _miso, uint8_t _sck, uint8_t _ce, uint8_t _csn)
{
  _MOSI=_mosi;
  _MISO=_miso;
  _SCK=_sck;
  _CE=_ce;
  _CSN=_csn;
}

void NRF::init(uint8_t mode, uint8_t RX_length)
{
	init(RX_MODE,0,0,RX_length);
}

void NRF::init(uint8_t mode, byte *buff, uint8_t TX_length)
{
	init(TX_MODE,buff,TX_length,0);
}

void NRF::init(uint8_t mode, byte *buff, uint8_t TX_length, uint8_t RX_length)
{
  pinMode(_CE, OUTPUT);
  pinMode(_CSN, OUTPUT);
  pinMode(_MOSI, OUTPUT);
  pinMode(_MISO, INPUT);
  pinMode(_SCK, OUTPUT);

  digitalWrite(_CE,0);
  digitalWrite(_CSN,1);

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

void NRF::SetAddress(uint8_t addr0, uint8_t addr1, uint8_t addr2, uint8_t addr3, uint8_t addr4)
{
	ADDRESS[0] = addr0;
	ADDRESS[1] = addr1;
	ADDRESS[2] = addr2;
	ADDRESS[3] = addr3;
	ADDRESS[4] = addr4;
}

uint8_t NRF::GetAddress(uint8_t addr)
{
	if(addr<5)return ADDRESS[addr];
	else return 0;
}

byte NRF::SPI_RW(unsigned char Byte)
{
  for(byte i=0;i<8;i++)
  {
    if(Byte&0x80) digitalWrite(_MOSI,1);
    else digitalWrite(_MOSI,0);

    digitalWrite(_SCK,1);
    Byte<<=1;
    if(digitalRead(_MISO)==1) Byte|=1;
    digitalWrite(_SCK,0);
  }

  return Byte;
}

byte NRF::SPI_RW_Reg(byte reg,byte value)
{
  byte status_;

  digitalWrite(_CSN,0);
  status_=SPI_RW(reg);
  SPI_RW(value);
  digitalWrite(_CSN,1);
  return status_;
}

byte NRF::SPI_Read(byte reg)
{
  byte reg_val;
  digitalWrite(_CSN,0);
  SPI_RW(reg);
  reg_val=SPI_RW(0);
  digitalWrite(_CSN,1);
  return reg_val;
}

byte NRF::Status(void)
{
  return SPI_Read(STATUS_);
}

byte NRF::SPI_Write_Buf(byte reg,byte *pBuf,byte bytes)
{
  byte status_;

  digitalWrite(_CSN,0);
  status_=SPI_RW(reg);
  for(byte i=0;i<bytes;i++) SPI_RW(*pBuf++);
  digitalWrite(_CSN,1);
  return status_;
}

byte NRF::SPI_Read_Buf(byte reg,byte *pBuf,byte bytes)
{
  byte status_;

  digitalWrite(_CSN,0);
  status_=SPI_RW(reg);
  for(byte i=0;i<bytes;i++) pBuf[i]=SPI_RW(0);
  digitalWrite(_CSN,1);
  return status_;
}

void NRF::TX_Mode(byte *txbuff)
{
  digitalWrite(_CE,0);
  SPI_Write_Buf(_WRITE_REG+TX_ADDR,ADDRESS,ADR_WIDTH);
  SPI_Write_Buf(_WRITE_REG+RX_ADDR_P0,ADDRESS,ADR_WIDTH);

  SPI_RW_Reg(_WRITE_REG+EN_AA,0x01);
  SPI_RW_Reg(_WRITE_REG+EN_RXADDR,0x01);
  SPI_RW_Reg(_WRITE_REG+CONFIG,0x0E);
  SPI_RW_Reg(_WRITE_REG+RF_CH,40);
  SPI_RW_Reg(_WRITE_REG+RF_SETUP,0x07);
  SPI_RW_Reg(_WRITE_REG+SETUP_RETR,0x1A);
  SPI_Write_Buf(WR_TX_PLOAD,txbuff,TX_PLOAD_WIDTH);

  digitalWrite(_CE,1);
}

void NRF::RX_Mode()
{
  digitalWrite(_CE,0);
  SPI_Write_Buf(_WRITE_REG+RX_ADDR_P0,ADDRESS,ADR_WIDTH);
  SPI_RW_Reg(_WRITE_REG+EN_AA,0x01);
  SPI_RW_Reg(_WRITE_REG+EN_RXADDR,0x01);
  SPI_RW_Reg(_WRITE_REG+RF_CH,40);
  SPI_RW_Reg(_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);
  SPI_RW_Reg(_WRITE_REG+RF_SETUP,0x07);
  SPI_RW_Reg(_WRITE_REG+CONFIG,0x0F);
  digitalWrite(_CE,1);
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
