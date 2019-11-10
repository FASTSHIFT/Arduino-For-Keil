/**********************************************************************************************
                           Designed By _VIFEXTech For NRF24L01
Finish  2017.8.16  v1.0
Upgrade 2017.12.5  v1.1 增加初始化函数，以及地址设置函数
Upgrade 2018.6.16  v1.2 支持双向半双工数据回传
Upgrade 2018.6.18  v1.3 加入模式设定合法性检测，支持使用指针获取模块地址
Upgrade 2018.6.19  v1.4 支持硬件SPI，更高效的IO操作
Upgrade 2018.8.17  v1.5 寄存器级别优化
Upgrade 2018.9.6   v1.6 优化SPI
Upgrade 2018.9.7   v1.7 将GPIO指针初始化放入构造函数
Upgrade 2018.9.21  v1.8 增加 IsDetect() 判断是否检测到NRF
Upgrade 2018.10.5  v1.9 支持设置速率、发射功率、频率
Upgrade 2018.10.11 v2.0 根据丢包率判断信号强度
Upgrade 2018.10.21 v2.1 优化信号强度检测
Upgrade 2018.11.17 v2.2 将宏定义常量转移到class的枚举类型中
Upgrade 2018.11.28 v2.3 增加注释，删除重复的初始化函数
Upgrade 2019.3.15  v2.4 增加GetSpeed(),GetFreqency(),GetPower(),SetPower(NRF_Power_TypeDef power)函数，
                        增加namespace NRF_TypeDef限制类型定义作用域
Upgrade 2019.3.22  v2.5 增加TranRecvSafe()，RecvTranSafe()函数，提升在较差通信环境时双向通信的稳定性
Upgrade 2019.3.26  v2.6 将MISO引脚配置成浮空输入
Upgrade 2019.7.8   v2.7 添加掉电模式
Upgrade 2019.9.7   v2.8 修复NRF初始化失败，更新TranRecvSafe、IsDetect函数
**************************************************************************************************/

#ifndef __NRF_H
#define __NRF_H

#define __NRF_LIB_VERSION "v2.8"

#include "Arduino.h"

//#define NRF_SPI_OBJECT SPI  //取消注释以支持硬件SPI方式

#ifdef NRF_SPI_OBJECT
#include "SPI.h"
#endif

namespace NRF_TypeDef
{

#if defined (__AVR__)
typedef volatile uint8_t PortReg_t;
typedef uint8_t PortMask_t;
#define USE_FAST_IO
#elif defined(__STM32__)
#if defined(__STM32F0__)
typedef volatile uint16_t PortReg_t;
#else
typedef volatile uint32_t PortReg_t;
#endif
typedef uint16_t PortMask_t;
#define USE_FAST_IO
#endif

}//end of namespace NRF

/**NRF24L01 引脚分布图(模块正面,俯视)
  *----------------+-------+
  *|               |       |
  *|        _______|_______|
  *|        |8:IRQ | 7:MISO|
  *|        |6:MOSI| 5:SCK |
  *|        |4:CSN | 3:CE  |
  *|        |2:VCC | 1:GND |
  *----------------+-------+
  */

class NRF
{
public:
    NRF(uint8_t mosi, uint8_t miso, uint8_t sck, uint8_t ce, uint8_t csn);
#ifdef NRF_SPI_OBJECT
    NRF(uint8_t ce, uint8_t csn);
#endif

    typedef enum
    {
        RX_MODE,  // 接收模式
        TX_MODE,  // 发送模式
        TXRX_MODE,// 发送/接收模式(半双工通信，用于主机)
        RXTX_MODE // 接收/发送模式(半双工通信，用于从机)
    } NRF_Mode_TypeDef;//通信模式选项

    typedef enum
    {
        SPEED_250Kbps = 0x27,
        SPEED_1Mbps = 0x07,
        SPEED_2Mbps = 0x0F
    } NRF_Speed_TypeDef;//通信速率选项

    typedef enum
    {
        POWER_m18dBm = 0x00,
        POWER_m12dBm = 0x02,
        POWER_m6dBm = 0x04,
        POWER_0dBm  = 0x06
    } NRF_Power_TypeDef;//通信功率选项

    void SetAddress(uint8_t addr0, uint8_t addr1, uint8_t addr2, uint8_t addr3, uint8_t addr4);
    void SetAddress(uint8_t* addr);
    void SetSpeed(NRF_Speed_TypeDef speed);
    void SetFreqency(uint8_t freq);
    void SetPower(NRF_Power_TypeDef power);
    uint8_t GetAddress(uint8_t addr);
    void GetAddress(uint8_t *addr, bool istx = true);
    uint8_t GetSpeed();
    uint8_t GetFreqency();
    uint8_t GetPower();
    uint8_t GetTxRxState();
    float GetSignalStrength();

    bool init(NRF_Mode_TypeDef mode, uint8_t *buff, uint8_t TX_length, uint8_t RX_length);
    bool IsDetect();
    uint8_t Status();
    uint8_t ClearFlag();
    uint8_t Tran(uint8_t *txbuff);
    uint8_t Recv(uint8_t *rxbuff);
    uint8_t TranRecv(uint8_t *txbuff, uint8_t *rxbuff);
    uint8_t RecvTran(uint8_t *rxbuff, uint8_t *txbuff);
    uint8_t TranRecvSafe(uint8_t *txbuff, uint8_t *rxbuff);
    uint8_t RecvTranSafe(uint8_t *rxbuff, uint8_t *txbuff);
    void TX_Mode(uint8_t *txbuff);
    void RX_Mode();
    void LowPower_Mode();

    enum COMMAND
    {
        READ_REG = 0x00,
        WRITE_REG = 0x20,
        RD_RX_PLOAD = 0x61,
        WR_TX_PLOAD = 0xA0,
        FLUSH_TX = 0xE1,
        FLUSH_RX = 0xE2,
        REUSE_TX_PL = 0xE3,
        NOP = 0xFF
    };//预指令

    enum FLAG
    {
        RX_DR = 0x40,
        TX_DS = 0x20,
        MAX_RT = 0x10
    };//中断标志位

    enum REGISTER
    {
        CONFIG = 0x00,
        EN_AA = 0x01,
        EN_RXADDR = 0x02,
        SETUP_AW = 0x03,
        SETUP_RETR = 0x04,
        RF_CH = 0x05,
        RF_SETUP = 0x06,
        STATUS = 0x07,
        OBSERVE_TX = 0x08,
        CD = 0x09,
        RX_ADDR_P0 = 0x0A,
        RX_ADDR_P1 = 0x0B,
        RX_ADDR_P2 = 0x0C,
        RX_ADDR_P3 = 0x0D,
        RX_ADDR_P4 = 0x0E,
        RX_ADDR_P5 = 0x0F,
        TX_ADDR = 0x10,
        RX_PW_P0 = 0x11,
        RX_PW_P1 = 0x12,
        RX_PW_P2 = 0x13,
        RX_PW_P3 = 0x14,
        RX_PW_P4 = 0x15,
        RX_PW_P5 = 0x16,
        FIFO_STATUS = 0x17
    };//寄存器

private:
    uint8_t SPI_RW(uint8_t Data);
    uint8_t SPI_RW_Reg(uint8_t reg, uint8_t value);
    uint8_t SPI_Read(uint8_t reg);
    uint8_t SPI_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes);
    uint8_t SPI_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes);
    bool hwSPI;
    uint8_t ADR_WIDTH;
    uint8_t ADDRESS[5];
    uint8_t TX_PLOAD_WIDTH;
    uint8_t RX_PLOAD_WIDTH;
    uint8_t MOSI_Pin, MISO_Pin, SCK_Pin, CE_Pin, CSN_Pin;
    NRF_Speed_TypeDef RF_Speed;
    NRF_Power_TypeDef RF_Power;
    uint8_t RF_Freq;
    uint8_t RF_RetryDelay;
    uint8_t RF_RetryTime;
    uint8_t NowState, Retry_Cnt;

    uint32_t Tran_Cnt, TranSuccess_Cnt;
#if defined(USE_FAST_IO)
    NRF_TypeDef::PortReg_t *mosiport, *misoport, *sckport, *ceport, *csnport;
    NRF_TypeDef::PortMask_t mosipinmask, misopinmask, sckpinmask, cepinmask, csnpinmask;
#endif
};

#endif
