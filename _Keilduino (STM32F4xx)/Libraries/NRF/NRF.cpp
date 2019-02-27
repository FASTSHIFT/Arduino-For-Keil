#include "NRF.h"

uint8_t ADDRESS[ADR_WIDTH] = {10, 20, 30, 40, 50};

NRF::NRF(uint8_t _mosi, uint8_t _miso, uint8_t _sck, uint8_t _ce, uint8_t _csn)
{
    _MOSI = _mosi;
    _MISO = _miso;
    _SCK = _sck;
    _CE = _ce;
    _CSN = _csn;
}

#ifdef USE_HARDWARE_SPI
NRF::NRF(uint8_t _ce, uint8_t _csn)
{
    _CE = _ce;
    _CSN = _csn;
}
#endif

void NRF::init(NRF_Mode_TypeDef mode, uint8_t RX_length)
{
    init(RX_MODE, 0, 0, RX_length);
}

void NRF::init(NRF_Mode_TypeDef mode, uint8_t *buff, uint8_t TX_length)
{
    init(TX_MODE, buff, TX_length, 0);
}

void NRF::init(NRF_Mode_TypeDef mode, uint8_t *buff, uint8_t TX_length, uint8_t RX_length)
{	
#ifdef USE_HARDWARE_SPI
    SPI_BEGIN();
#else	
	#ifdef USE_FAST_IO
		mosiport	= portOutputRegister(digitalPinToPort(_MOSI));
		mosipinmask	= digitalPinToBitMask(_MOSI);
		misoport	= portInputRegister(digitalPinToPort(_MISO));
		misopinmask	= digitalPinToBitMask(_MISO);
		sckport		= portOutputRegister(digitalPinToPort(_SCK));
		sckpinmask	= digitalPinToBitMask(_SCK);
	#endif
	
    pinMode(_MOSI, OUTPUT);
    pinMode(_MISO, INPUT);
    pinMode(_SCK, OUTPUT);
#endif
	
#ifdef USE_FAST_IO
	ceport	= portOutputRegister(digitalPinToPort(_CE));
	cepinmask	= digitalPinToBitMask(_CE);
	csnport	= portOutputRegister(digitalPinToPort(_CSN));
	csnpinmask	= digitalPinToBitMask(_CSN);
#endif

    pinMode(_CE, OUTPUT);
    pinMode(_CSN, OUTPUT);

    NRF_CE_LOW();
    NRF_CSN_HIGH();

    switch(mode)
    {
    case RX_MODE:
        RX_PLOAD_WIDTH = RX_length;
        RX_Mode();
        break;

    case TX_MODE:
        TX_PLOAD_WIDTH = TX_length;
        TX_Mode(buff);
        break;

    case TXRX_MODE:
        RX_PLOAD_WIDTH = RX_length;
        TX_PLOAD_WIDTH = TX_length;
        TX_Mode(buff);
        break;

    case RXTX_MODE:
        RX_PLOAD_WIDTH = RX_length;
        TX_PLOAD_WIDTH = TX_length;
        RX_Mode();
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
    if(addr < 5)return ADDRESS[addr];
    else return 0;
}

uint8_t NRF::SPI_RW(uint8_t Data)
{
#ifdef USE_HARDWARE_SPI
    return (SPI_Transfer(Data));
#else
    for(uint8_t i = 0; i < 8; i++)
    {
        if(Data & 0x80) NRF_MOSI_HIGH();
        else NRF_MOSI_LOW();

        NRF_SCK_HIGH();
        Data <<= 1;
        if(NRF_MISO_READ()) Data |= 1;
        NRF_SCK_LOW();
    }
    return Data;
#endif
}

uint8_t NRF::SPI_RW_Reg(uint8_t reg, uint8_t value)
{
    uint8_t status_;

    NRF_CSN_LOW();
    status_ = SPI_RW(reg);
    SPI_RW(value);
    NRF_CSN_HIGH();
    return status_;
}

uint8_t NRF::SPI_Read(uint8_t reg)
{
    uint8_t reg_val;
    NRF_CSN_LOW();
    SPI_RW(reg);
    reg_val = SPI_RW(0);
    NRF_CSN_HIGH();
    return reg_val;
}

uint8_t NRF::Status(void)
{
    return SPI_Read(STATUS_);
}

uint8_t NRF::SPI_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes)
{
    uint8_t status_;

    NRF_CSN_LOW();
    status_ = SPI_RW(reg);
    for(uint8_t i = 0; i < bytes; i++) SPI_RW(*pBuf++);
    NRF_CSN_HIGH();
    return status_;
}

uint8_t NRF::SPI_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes)
{
    uint8_t status_;

    NRF_CSN_LOW();
    status_ = SPI_RW(reg);
    for(uint8_t i = 0; i < bytes; i++) pBuf[i] = SPI_RW(0);
    NRF_CSN_HIGH();
    return status_;
}

void NRF::TX_Mode(uint8_t *txbuff)
{
    NRF_CE_LOW();
    SPI_Write_Buf(NRF_WRITE_REG + TX_ADDR, ADDRESS, ADR_WIDTH);
    SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, ADDRESS, ADR_WIDTH);

    SPI_RW_Reg(NRF_WRITE_REG + EN_AA, 0x01);
    SPI_RW_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);
    SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x0E);
    SPI_RW_Reg(NRF_WRITE_REG + RF_CH, 40);
    SPI_RW_Reg(NRF_WRITE_REG + RF_SETUP, 0x07);
    SPI_RW_Reg(NRF_WRITE_REG + SETUP_RETR, 0x1A);
    SPI_Write_Buf(WR_TX_PLOAD, txbuff, TX_PLOAD_WIDTH);

    NRF_CE_HIGH();
}

void NRF::RX_Mode()
{
    NRF_CE_LOW();
    SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, ADDRESS, ADR_WIDTH);
    SPI_RW_Reg(NRF_WRITE_REG + EN_AA, 0x01);
    SPI_RW_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);
    SPI_RW_Reg(NRF_WRITE_REG + RF_CH, 40);
    SPI_RW_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);
    SPI_RW_Reg(NRF_WRITE_REG + RF_SETUP, 0x07);
    SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x0F);
    NRF_CE_HIGH();
}

uint8_t NRF::Tran(uint8_t *txbuff)
{
    uint8_t status_ = SPI_Read(STATUS_);
    if(status_ & TX_DS)
    {
        SPI_RW_Reg(FLUSH_TX, 0);
        SPI_Write_Buf(WR_TX_PLOAD, txbuff, TX_PLOAD_WIDTH);
    }
    if(status_ & MAX_RT)
    {
        SPI_RW_Reg(FLUSH_TX, 0);
        SPI_Write_Buf(WR_TX_PLOAD, txbuff, TX_PLOAD_WIDTH);
    }
    SPI_RW_Reg(NRF_WRITE_REG + STATUS_, status_);
    return status_;
}

uint8_t NRF::Recv(uint8_t *rxbuff)
{
    uint8_t status_ = SPI_Read(STATUS_);
    if(status_ & RX_DR)
    {
        SPI_Read_Buf(RD_RX_PLOAD, rxbuff, RX_PLOAD_WIDTH);
        SPI_RW_Reg(FLUSH_RX, 0);
    }
    SPI_RW_Reg(NRF_WRITE_REG + STATUS_, status_);
    return status_;
}

uint8_t NRF::TranRecv(uint8_t *txbuff, uint8_t *rxbuff)
{
    static uint8_t NowState = TX_MODE, Retry_Cnt = 0;
    if(NowState == TX_MODE)
    {
        uint8_t TX_State = Tran(txbuff);
        if(TX_State & TX_DS || TX_State & MAX_RT)
        {
            NowState = RX_MODE;
            RX_Mode();
        }
    }
    else
    {
        uint8_t RX_State = Recv(rxbuff);
        if(RX_State & RX_DR || Retry_Cnt == 2)
        {
            Retry_Cnt = 0;
            NowState = TX_MODE;
            TX_Mode(txbuff);
        }
        else Retry_Cnt++;
    }
    return NowState;
}

uint8_t NRF::RecvTran(uint8_t *rxbuff, uint8_t *txbuff)
{
    static uint8_t NowState = RX_MODE, Retry_Cnt = 0;
    if(NowState == RX_MODE)
    {
        uint8_t RX_State = Recv(rxbuff);
        if(RX_State & RX_DR || Retry_Cnt == 1)
        {
            NowState = TX_MODE;
            TX_Mode(txbuff);
        }
        else Retry_Cnt++;
    }
    else
    {
        uint8_t TX_State = Tran(txbuff);
        if(TX_State & TX_DS || TX_State & MAX_RT)
        {
            NowState = RX_MODE;
            RX_Mode();
        }
    }
    return NowState;
}
