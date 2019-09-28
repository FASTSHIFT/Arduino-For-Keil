#include "NRF.h"

#ifdef NRF_SPI_OBJECT
#if defined (__AVR__)
#define NRF_SPI_BEGIN() {NRF_SPI_OBJECT.begin(),NRF_SPI_OBJECT.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));}
#elif defined(__STM32__)
#define NRF_SPI_BEGIN() {NRF_SPI_OBJECT.begin(SPISettings(10000000, MSBFIRST, SPI_MODE0));}
#endif
#define SPI_Transfer(x) NRF_SPI_OBJECT.transfer(x)
#endif

#ifdef USE_FAST_IO //使用寄存器直接控制GPIO
#define FAST_IO_HIGH(port,mask) (*port|=mask)
#define FAST_IO_LOW(port,mask)  (*port&=~mask)
#define FAST_IO_READ(port,mask) ((*port&mask)!=0)

#define NRF_MISO_READ() FAST_IO_READ(misoport,misopinmask)
#define NRF_MOSI_HIGH() FAST_IO_HIGH(mosiport,mosipinmask)
#define NRF_MOSI_LOW()  FAST_IO_LOW(mosiport,mosipinmask)
#define NRF_SCK_HIGH()  FAST_IO_HIGH(sckport,sckpinmask)
#define NRF_SCK_LOW()   FAST_IO_LOW(sckport,sckpinmask)
#define NRF_CE_HIGH()   FAST_IO_HIGH(ceport,cepinmask)
#define NRF_CE_LOW()    FAST_IO_LOW(ceport,cepinmask)
#define NRF_CSN_HIGH()  FAST_IO_HIGH(csnport,csnpinmask)
#define NRF_CSN_LOW()   FAST_IO_LOW(csnport,csnpinmask)

#else            //使用Arduino标准函数控制控制GPIO
#define NRF_MISO_READ() digitalRead(MISO_Pin)
#define NRF_MOSI_HIGH() digitalWrite(MOSI_Pin,1)
#define NRF_MOSI_LOW()  digitalWrite(MOSI_Pin,0)
#define NRF_SCK_HIGH()  digitalWrite(SCK_Pin,1)
#define NRF_SCK_LOW()   digitalWrite(SCK_Pin,0)
#define NRF_CE_HIGH()   digitalWrite(CE_Pin,1)
#define NRF_CE_LOW()    digitalWrite(CE_Pin,0)
#define NRF_CSN_HIGH()  digitalWrite(CSN_Pin,1)
#define NRF_CSN_LOW()   digitalWrite(CSN_Pin,0)
#endif

/**
  * @brief  构造函数，传入连接NRF的引脚编号，软件SPI方式
  * @param  mosi: 连接MOSI的引脚编号
  * @param  miso: 连接MISO的引脚编号
  * @param  sck:  连接SCK 的引脚编号
  * @param  se:   连接CE  的引脚编号
  * @param  csn:  连接CSN的引脚编号
  * @retval 无
  */
NRF::NRF(uint8_t mosi, uint8_t miso, uint8_t sck, uint8_t ce, uint8_t csn)
{
    MOSI_Pin = mosi;//引脚编号传入
    MISO_Pin = miso;
    SCK_Pin = sck;
    CE_Pin = ce;
    CSN_Pin = csn;
    hwSPI = false;//不使用硬件SPI

    RF_Speed = SPEED_1Mbps;//通信速率:1Mb/s
    RF_Power = POWER_0dBm;//通信功率:0dBm(最大)
    RF_Freq = 40;//通信频率40
    RF_RetryDelay = 0x10;//自动重发延时(us) = 250 + n * 250 + 86 us
    RF_RetryTime = 15;//自动重发次数15
    Retry_Cnt = 0;//重试计数
    ADR_WIDTH = 5;//地址宽度5字节

#ifdef USE_FAST_IO //取GPIO寄存器地址，用于高速控制IO
    mosiport    = portOutputRegister(digitalPinToPort(MOSI_Pin));
    mosipinmask = digitalPinToBitMask(MOSI_Pin);
    misoport    = portInputRegister(digitalPinToPort(MISO_Pin));
    misopinmask = digitalPinToBitMask(MISO_Pin);
    sckport     = portOutputRegister(digitalPinToPort(SCK_Pin));
    sckpinmask  = digitalPinToBitMask(SCK_Pin);

    ceport  = portOutputRegister(digitalPinToPort(CE_Pin));
    cepinmask   = digitalPinToBitMask(CE_Pin);
    csnport = portOutputRegister(digitalPinToPort(CSN_Pin));
    csnpinmask  = digitalPinToBitMask(CSN_Pin);
#endif
}

#ifdef NRF_SPI_OBJECT
/**
  * @brief  构造函数，传入连接NRF的引脚编号，硬件SPI方式
  * @param  se:   连接CE  的引脚编号
  * @param  csn:  连接CSN的引脚编号
  * @retval 无
  */
NRF::NRF(uint8_t ce, uint8_t csn)
{
    CE_Pin = ce;//引脚编号传入
    CSN_Pin = csn;
    hwSPI = true;//使用硬件SPI

    RF_Speed = SPEED_1Mbps;//通信速率:1Mb/s
    RF_Power = POWER_0dBm;//通信功率:0dBm(最大)
    RF_Freq = 40;//通信频率40
    RF_RetryDelay = 0x10;//自动重发延时(us) = 250 + n * 250 + 86 us
    RF_RetryTime = 15;//自动重发次数15
    Retry_Cnt = 0;//重试计数
    ADR_WIDTH = 5;//地址宽度5字节

#ifdef USE_FAST_IO //取GPIO寄存器地址，用于高速控制IO
    ceport  = portOutputRegister(digitalPinToPort(CE_Pin));
    cepinmask   = digitalPinToBitMask(CE_Pin);
    csnport = portOutputRegister(digitalPinToPort(CSN_Pin));
    csnpinmask  = digitalPinToBitMask(CSN_Pin);
#endif
}
#endif

/**
  * @brief  NRF初始化
  * @param  mode: 选择模式
  * @param  *buff: 发送缓冲区地址指针(如果只配置为接收模式，可以传入0)
  * @param  TX_length: 发送缓冲区长度(如果只配置为接收模式，可以传入0)
  * @param  RX_length: 接收缓冲区长度(如果只配置为发送模式，可以传入0)
  * @retval 无
  */
bool NRF::init(NRF_Mode_TypeDef mode, uint8_t *buff, uint8_t TX_length, uint8_t RX_length)
{
#ifdef NRF_SPI_OBJECT
    if(hwSPI)NRF_SPI_BEGIN();//硬件SPI初始化
#else
    pinMode(MOSI_Pin, OUTPUT);
    pinMode(MISO_Pin, INPUT);
    pinMode(SCK_Pin, OUTPUT);
#endif

    pinMode(CE_Pin, OUTPUT);
    pinMode(CSN_Pin, OUTPUT);
    
    NRF_CE_HIGH();
    delay(100);
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
        NowState = TX_MODE;
        TX_Mode(buff);
        break;

    case RXTX_MODE:
        RX_PLOAD_WIDTH = RX_length;
        TX_PLOAD_WIDTH = TX_length;
        NowState = RX_MODE;
        RX_Mode();
        break;
    }
    
    ClearFlag();
    
    return Status();
}

/**
  * @brief  设置NRF通信地址(5字节)
  * @param  addr0:地址第0个字节
  * @param  addr1:地址第1个字节
  * @param  addr2:地址第2个字节
  * @param  addr3:地址第3个字节
  * @param  addr4:地址第4个字节
  * @retval 无
  */
void NRF::SetAddress(uint8_t addr0, uint8_t addr1, uint8_t addr2, uint8_t addr3, uint8_t addr4)
{
    ADDRESS[0] = addr0;
    ADDRESS[1] = addr1;
    ADDRESS[2] = addr2;
    ADDRESS[3] = addr3;
    ADDRESS[4] = addr4;

    NRF_CE_LOW();
    SPI_Write_Buf(WRITE_REG + TX_ADDR, ADDRESS, ADR_WIDTH);
    SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, ADDRESS, ADR_WIDTH);
    NRF_CE_HIGH();
}

/**
  * @brief  设置NRF通信地址(5字节)
  * @param  addr: 一个包含5个字节的数组的指针
  * @retval 无
  */
void NRF::SetAddress(uint8_t* addr)
{
    ADDRESS[0] = addr[0];
    ADDRESS[1] = addr[1];
    ADDRESS[2] = addr[2];
    ADDRESS[3] = addr[3];
    ADDRESS[4] = addr[4];

    NRF_CE_LOW();
    SPI_Write_Buf(WRITE_REG + TX_ADDR, ADDRESS, ADR_WIDTH);
    SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, ADDRESS, ADR_WIDTH);
    NRF_CE_HIGH();
}

/**
  * @brief  设置NRF发送功率
  * @param  power: 功率值(见NRF_Power_TypeDef)
  * @retval 无
  */
void NRF::SetPower(NRF_Power_TypeDef power)
{
    RF_Power = power;

    NRF_CE_LOW();
    SPI_RW_Reg(WRITE_REG + RF_SETUP, RF_Speed | RF_Power);
    NRF_CE_HIGH();
}

/**
  * @brief  设置NRF通信速率
  * @param  speed:速率(SPEED_250Kbps,SPEED_1Mbps,SPEED_2Mbps)
  * @retval 无
  */
void NRF::SetSpeed(NRF_Speed_TypeDef speed)
{
    RF_Speed = speed;

    NRF_CE_LOW();
    SPI_RW_Reg(WRITE_REG + RF_SETUP, RF_Speed);//250K:0x27, 1M:0x07, 2M:0x0F
    NRF_CE_HIGH();
}

/**
  * @brief  设置NRF通信频率
  * @param  freq:频率(0~127)
  * @retval 无
  */
void NRF::SetFreqency(uint8_t freq)
{
    RF_Freq = freq % 128;

    NRF_CE_LOW();
    SPI_RW_Reg(WRITE_REG + RF_CH, RF_Freq);//0~127
    NRF_CE_HIGH();
}

/**
  * @brief  获取当前通信地址
  * @param  addr:第几位地址
  * @retval addr对应地址
  */
uint8_t NRF::GetAddress(uint8_t addr)
{
    if(addr < 5)return ADDRESS[addr];
    else return 0;
}

/**
  * @brief  获取当前通信地址
  * @param  addr:地址
  * @retval 无
  */
void NRF::GetAddress(uint8_t *addr, bool istx)
{
    NRF_CE_LOW();
    if(istx)
        SPI_Read_Buf(READ_REG + TX_ADDR, addr, ADR_WIDTH);
    else
        SPI_Read_Buf(READ_REG + RX_ADDR_P0, addr, ADR_WIDTH);
    NRF_CE_HIGH();
}

/**
  * @brief  获取当前通信速度
  * @param  无
  * @retval 0~2 -> 250Kbps~2Mbps
  */
uint8_t NRF::GetSpeed()
{
    uint8_t speed = 0;
    if(RF_Speed == SPEED_250Kbps)
        speed = 0;
    else if(RF_Speed == SPEED_1Mbps)
        speed = 1;
    else if(RF_Speed == SPEED_2Mbps)
        speed = 2;
    return speed;
}

/**
  * @brief  获取当前通信频率
  * @param  无
  * @retval 频率值(+2400MHz)
  */
uint8_t NRF::GetFreqency()
{
    return RF_Freq;
}

/**
  * @brief  获取NRF信号强度(发送成功率)
  * @param  无
  * @retval 信号强度(0.0~100.0%)
  */
float NRF::GetSignalStrength()
{
    if(Tran_Cnt == 0) return 0.0;

    float SignalStrength = (float)TranSuccess_Cnt * 100.0 / Tran_Cnt;
    TranSuccess_Cnt = 0;
    Tran_Cnt = 0;
    return SignalStrength;
}

/**
  * @brief  获取当前是否处于发送状态或者接收状态
  * @param  无
  * @retval 当前状态
  */
uint8_t NRF::GetTxRxState()
{
    return NowState;
}

/**
  * @brief  SPI读写
  * @param  Data:数据
  * @retval SPI返回的数据
  */
uint8_t NRF::SPI_RW(uint8_t Data)
{
    if(hwSPI)//如果使用硬件SPI
    {
#ifdef NRF_SPI_OBJECT
        return (SPI_Transfer(Data));
#else
        return 0;
#endif
    }
    else
    {
        for(uint8_t i = 0; i < 8; i++)
        {
            (Data & 0x80) ? NRF_MOSI_HIGH() : NRF_MOSI_LOW();
            NRF_SCK_HIGH();
            Data <<= 1;
            if(NRF_MISO_READ()) Data |= 1;
            NRF_SCK_LOW();
        }
        return Data;
    }
}

/**
  * @brief  读写NRF寄存器
  * @param  reg:寄存器
  * @param  value:值
  * @retval 寄存器的状态
  */
uint8_t NRF::SPI_RW_Reg(uint8_t reg, uint8_t value)
{
    uint8_t status;

    NRF_CSN_LOW();
    status = SPI_RW(reg);
    SPI_RW(value);
    NRF_CSN_HIGH();
    return status;
}

/**
  * @brief  读NRF寄存器
  * @param  reg:寄存器
  * @retval 寄存器的状态
  */
uint8_t NRF::SPI_Read(uint8_t reg)
{
    uint8_t reg_val;
    NRF_CSN_LOW();
    SPI_RW(reg);
    reg_val = SPI_RW(0);
    NRF_CSN_HIGH();
    return reg_val;
}

/**
  * @brief  读NRF状态
  * @param  无
  * @retval NRF状态寄存器的值
  */
uint8_t NRF::Status(void)
{
    return SPI_Read(STATUS);
}

uint8_t NRF::ClearFlag()
{
    uint8_t status = SPI_Read(STATUS);
    SPI_RW_Reg(WRITE_REG + STATUS, status);//清状态寄存器标志位
    return status;
}

/**
  * @brief  判断NRF硬件连线是否正确
  * @param  无
  * @retval true 已正确连接; false 连接不正确
  */
bool NRF::IsDetect()
{
    uint8_t detect = true;
    uint8_t addr[5];
    GetAddress(addr);
    for(uint8_t i = 0;i < sizeof(addr);i++)
    {
        if(addr[i] != ADDRESS[i])
        {
            detect = false;
        }
    }
    
    return detect;
}

/**
  * @brief  写入一串数据到NRF寄存器
  * @param  reg:寄存器
  * @param  *pBuf:数组头地址
  * @param  bytes:数据字节数
  * @retval 寄存器的状态
  */
uint8_t NRF::SPI_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes)
{
    uint8_t status;

    NRF_CSN_LOW();
    status = SPI_RW(reg);
    for(uint8_t i = 0; i < bytes; i++) SPI_RW(*pBuf++);
    NRF_CSN_HIGH();
    return status;
}

/**
  * @brief  读入一串数据到NRF寄存器
  * @param  reg:寄存器
  * @param  *pBuf:数组头地址
  * @param  bytes:数据字节数
  * @retval 寄存器的状态
  */
uint8_t NRF::SPI_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes)
{
    uint8_t status;

    NRF_CSN_LOW();
    status = SPI_RW(reg);
    for(uint8_t i = 0; i < bytes; i++) pBuf[i] = SPI_RW(0);
    NRF_CSN_HIGH();
    return status;
}

/**
  * @brief  设置为发送模式
  * @param  *txbuff:发送缓冲区地址
  * @retval 无
  */
void NRF::TX_Mode(uint8_t *txbuff)
{
    NRF_CE_LOW();
    SPI_Write_Buf(WRITE_REG + TX_ADDR, ADDRESS, ADR_WIDTH);
    SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, ADDRESS, ADR_WIDTH);

    SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);
    SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);
    SPI_RW_Reg(WRITE_REG + CONFIG, 0x0E);
    SPI_RW_Reg(WRITE_REG + RF_CH, RF_Freq);
    SPI_RW_Reg(WRITE_REG + RF_SETUP, RF_Speed | RF_Power);
    SPI_RW_Reg(WRITE_REG + SETUP_RETR, RF_RetryDelay | RF_RetryTime);
    SPI_Write_Buf(WR_TX_PLOAD, txbuff, TX_PLOAD_WIDTH);

    NRF_CE_HIGH();
}

/**
  * @brief  设置为接收模式
  * @param  无
  * @retval 无
  */
void NRF::RX_Mode()
{
    NRF_CE_LOW();
    SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, ADDRESS, ADR_WIDTH);
    SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);

    SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);
    SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);
    SPI_RW_Reg(WRITE_REG + CONFIG, 0x0F);
    SPI_RW_Reg(WRITE_REG + RF_CH, RF_Freq);
    SPI_RW_Reg(WRITE_REG + RF_SETUP, RF_Speed | RF_Power);

    NRF_CE_HIGH();
}

/**
  * @brief  设置为低功耗模式
  * @param  无
  * @retval 无
  */
void NRF::LowPower_Mode()
{
    NRF_CE_LOW();
    SPI_RW_Reg(WRITE_REG + CONFIG, 0x0C);
}

/**
  * @brief  发送缓冲区数据
  * @param  *txbuff:发送缓冲区地址
  * @retval 状态寄存器的值
  */
uint8_t NRF::Tran(uint8_t *txbuff)
{
    uint8_t status = SPI_Read(STATUS);//读取状态寄存器
    if(status & TX_DS)//是否发送成功
    {
        TranSuccess_Cnt ++;//发送成功计数++
        Tran_Cnt ++;//发送计数++
        SPI_RW_Reg(FLUSH_TX, 0);//清缓冲区
        SPI_Write_Buf(WR_TX_PLOAD, txbuff, TX_PLOAD_WIDTH);//发送数据
        SPI_RW_Reg(WRITE_REG + STATUS, status);//清状态寄存器标志位
    }
    if(status & MAX_RT)
    {
        Tran_Cnt ++;//发送计数++
        SPI_RW_Reg(FLUSH_TX, 0);//清缓冲区
        SPI_Write_Buf(WR_TX_PLOAD, txbuff, TX_PLOAD_WIDTH);//发送数据
        SPI_RW_Reg(WRITE_REG + STATUS, status);//清状态寄存器标志位
    }

    return status;
}

/**
  * @brief  接收数据到接收缓冲区
  * @param  *rxbuff:接收缓冲区地址
  * @retval 状态寄存器的值
  */
uint8_t NRF::Recv(uint8_t *rxbuff)
{
    uint8_t status = SPI_Read(STATUS);//读取状态寄存器
    if(status & RX_DR)//是否接收成功
    {
        SPI_Read_Buf(RD_RX_PLOAD, rxbuff, RX_PLOAD_WIDTH);//接收数据到接收缓冲区
        SPI_RW_Reg(FLUSH_RX, 0);//清接收缓冲区
    }
    SPI_RW_Reg(WRITE_REG + STATUS, status);//清状态寄存器标志位

    return status;
}

/**
  * @brief  发送缓冲区数据,接收数据到接收缓冲区(半双工通信，用于主机)
  * @param  *txbuff:发送缓冲区地址
  * @param  *rxbuff:接收缓冲区地址
  * @retval 状态寄存器的值
  */
uint8_t NRF::TranRecv(uint8_t *txbuff, uint8_t *rxbuff)
{
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
        if(RX_State & RX_DR || Retry_Cnt == 1)
        {
            Retry_Cnt = 0;
            NowState = TX_MODE;
            TX_Mode(txbuff);
        }
        else Retry_Cnt++;
    }
    return NowState;
}

/**
  * @brief  发送缓冲区数据,接收数据到接收缓冲区(半双工通信，用于主机，适用于信号较差的环境)
  * @param  *txbuff:发送缓冲区地址
  * @param  *rxbuff:接收缓冲区地址
  * @retval 状态寄存器的值
  */
uint8_t NRF::TranRecvSafe(uint8_t *txbuff, uint8_t *rxbuff)
{
    const uint8_t TX_WAIT = 4;
    uint8_t status = Status();
    if(NowState == TX_MODE)
    {
        Tran(txbuff);
        NowState = TX_WAIT;
    }
    else if(NowState == TX_WAIT)
    {
        if(status & TX_DS)
        {
            NowState = RX_MODE;
            RX_Mode();
        }
        else
        {
            TX_Mode(txbuff);
            Tran(txbuff);
        }
    }
    else //NowState == RX_MODE
    {
        Recv(rxbuff);
        
        TX_Mode(txbuff);
        NowState = TX_MODE;
    }
    
    return NowState;
}

/**
  * @brief  接收数据到接收缓冲区,发送缓冲区数据(半双工通信，用于从机)
  * @param  *rxbuff:接收缓冲区地址
  * @param  *txbuff:发送缓冲区地址
  * @retval 状态寄存器的值
  */
uint8_t NRF::RecvTran(uint8_t *rxbuff, uint8_t *txbuff)
{
    if(NowState == RX_MODE)
    {
        uint8_t RX_State = Recv(rxbuff);
        if(RX_State & RX_DR)
        {
            NowState = TX_MODE;
            TX_Mode(txbuff);
        }
    }
    else
    {
        uint8_t TX_State = Tran(txbuff);
        if(TX_State & TX_DS || TX_State & MAX_RT || Retry_Cnt == 1)
        {
            Retry_Cnt = 0;
            NowState = RX_MODE;
            RX_Mode();
        }
        else Retry_Cnt++;
    }
    return NowState;
}

/**
  * @brief  接收数据到接收缓冲区,发送缓冲区数据(半双工通信，用于从机，适用于信号较差的环境)
  * @param  *rxbuff:接收缓冲区地址
  * @param  *txbuff:发送缓冲区地址
  * @retval 状态寄存器的值
  */
uint8_t NRF::RecvTranSafe(uint8_t *rxbuff, uint8_t *txbuff)
{
    if(NowState == RX_MODE)
    {
        uint8_t RX_State = Recv(rxbuff);
        if(RX_State & RX_DR)
        {
            NowState = TX_MODE;
            TX_Mode(txbuff);
        }
        else
        {
            RX_Mode();
        }
    }
    else // NowState == TX_MODE
    {
        Tran(txbuff);
        RX_Mode();
        NowState = RX_MODE;
    }

    return NowState;
}
