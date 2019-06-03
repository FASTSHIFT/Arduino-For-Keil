//Designed By _VIFEXTech
//2017.8.16  v1.0
//12.5       v1.1 增加初始化函数，以及地址设置函数
//2018.6.16  v1.2 支持双向半双工数据回传
//6.18       v1.3 加入模式设定合法性检测，支持使用指针获取模块地址
//6.19       v1.4 支持硬件SPI，更高效的IO操作
//8.17       v1.5 寄存器级别优化
//9.6		 v1.6 优化SPI
//9.7		 v1.7 将GPIO指针初始化放入构造函数
//9.21		 v1.8 增加 IsDetect() 判断是否检测到NRF
//10.5		 v1.9 支持设置速率、发射功率、频率
//10.11		 v2.0 根据丢包率判断信号强度
//10.21		 v2.1 优化信号强度检测
//11.17		 v2.2 将宏定义常量转移到class的枚举类型中
//11.28		 v2.3 增加注释，删除重复的初始化函数

#ifndef __NRF_H
#define __NRF_H

#define _NRF_LIB_VERSION "v2.3"

#include "Arduino.h"

//#define NRF_SPI_OBJECT SPI  //取消注释以支持硬件SPI方式

#ifdef NRF_SPI_OBJECT
#include "SPI.h"
#define SPI_Transfer(x) NRF_SPI_OBJECT.transfer(x)
#endif

#if defined (__AVR__)
#ifdef NRF_SPI_OBJECT
#define NRF_SPI_BEGIN() {NRF_SPI_OBJECT.begin(),NRF_SPI_OBJECT.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));}
#endif
typedef volatile uint8_t PortReg_t;
typedef uint8_t PortMask_t;
#define USE_FAST_IO
#elif defined(__STM32__)
#ifdef NRF_SPI_OBJECT
#define NRF_SPI_BEGIN() {NRF_SPI_OBJECT.begin(SPISettings(10000000, MSBFIRST, SPI_MODE0));}
#endif
#if defined(__STM32F0__)
typedef volatile uint16_t PortReg_t;
#else
typedef volatile uint32_t PortReg_t;
#endif
typedef uint16_t PortMask_t;
#define USE_FAST_IO
#endif


#ifdef USE_FAST_IO //使用寄存器直接控制GPIO
#define FAST_IO_HIGH(port,mask) (*port|=mask)
#define FAST_IO_LOW(port,mask)  (*port&=~mask)
#define FAST_IO_READ(port,mask) ((*port&mask)!=0)

#define NRF_MISO_READ() FAST_IO_READ(misoport,misopinmask)
#define NRF_MOSI_HIGH()	FAST_IO_HIGH(mosiport,mosipinmask)
#define NRF_MOSI_LOW() 	FAST_IO_LOW(mosiport,mosipinmask)
#define NRF_SCK_HIGH()	FAST_IO_HIGH(sckport,sckpinmask)
#define NRF_SCK_LOW() 	FAST_IO_LOW(sckport,sckpinmask)
#define NRF_CE_HIGH() 	FAST_IO_HIGH(ceport,cepinmask)
#define NRF_CE_LOW() 	FAST_IO_LOW(ceport,cepinmask)
#define NRF_CSN_HIGH() 	FAST_IO_HIGH(csnport,csnpinmask)
#define NRF_CSN_LOW() 	FAST_IO_LOW(csnport,csnpinmask)

#else            //使用标准函数控制控制GPIO
#define NRF_MISO_READ() digitalRead(MISO_Pin)
#define NRF_MOSI_HIGH()	digitalWrite(MOSI_Pin,1)
#define NRF_MOSI_LOW() 	digitalWrite(MOSI_Pin,0)
#define NRF_SCK_HIGH()	digitalWrite(SCK_Pin,1)
#define NRF_SCK_LOW() 	digitalWrite(SCK_Pin,0)
#define NRF_CE_HIGH() 	digitalWrite(CE_Pin,1)
#define NRF_CE_LOW() 	digitalWrite(CE_Pin,0)
#define NRF_CSN_HIGH() 	digitalWrite(CSN_Pin,1)
#define NRF_CSN_LOW() 	digitalWrite(CSN_Pin,0)
#endif

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

class NRF {
public:
    NRF(uint8_t mosi, uint8_t miso, uint8_t sck, uint8_t ce, uint8_t csn);
#ifdef NRF_SPI_OBJECT
    NRF(uint8_t ce, uint8_t csn);
#endif

    typedef enum {
        RX_MODE,  // 接收模式
        TX_MODE,  // 发送模式
        TXRX_MODE,// 发送/接收模式(半双工通信，用于主机)
        RXTX_MODE // 接收/发送模式(半双工通信，用于从机)
    } NRF_Mode_TypeDef;//通信模式选项

    typedef enum {
        SPEED_250Kbps = 0x27,
        SPEED_1Mbps = 0x07,
        SPEED_2Mbps = 0x0F
    } NRF_Speed_TypeDef;//通信速率选项

    typedef enum {
        POWER_m18dBm = 0x00,
        POWER_m12dBm = 0x02,
        POWER_m6dBm = 0x04,
        POWER_0dBm  = 0x06
    } NRF_Power_TypeDef;//通信功率选项

    void SetAddress(uint8_t addr0, uint8_t addr1, uint8_t addr2, uint8_t addr3, uint8_t addr4);
    void SetAddress(uint8_t* addr);
    void SetSpeed(NRF_Speed_TypeDef speed);
    void SetFreqency(uint8_t freq);
    uint8_t GetAddress(uint8_t addr);
    void init(NRF_Mode_TypeDef mode, uint8_t *buff, uint8_t TX_length, uint8_t RX_length);
    bool IsDetect();
    uint8_t Status();
    uint8_t Tran(uint8_t *txbuff);
    uint8_t Recv(uint8_t *rxbuff);
    uint8_t TranRecv(uint8_t *txbuff, uint8_t *rxbuff);
    uint8_t RecvTran(uint8_t *rxbuff, uint8_t *txbuff);
    void TX_Mode(uint8_t *txbuff);
    void RX_Mode();
    float GetSignalStrength();

    enum COMMAND {
        READ_REG = 0x00,
        WRITE_REG = 0x20,
        RD_RX_PLOAD = 0x61,
        WR_TX_PLOAD = 0xA0,
        FLUSH_TX = 0xE1,
        FLUSH_RX = 0xE2,
        REUSE_TX_PL = 0xE3,
        NOP = 0xFF
    };//预指令

    enum FLAG {
        RX_DR = 0x40,
        TX_DS = 0x20,
        MAX_RT = 0x10
    };//中断标志位

    enum REGISTER {
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
    PortReg_t *mosiport, *misoport, *sckport, *ceport, *csnport;
    PortMask_t mosipinmask, misopinmask, sckpinmask, cepinmask, csnpinmask;
#endif
};

#endif
