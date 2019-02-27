#include "SoftwareSPI.h"

#ifdef SWSPI_FAST_IO
    #if defined(__AVR__)
        #define FAST_IO_HIGH(port,mask) (*port|=mask)
        #define FAST_IO_LOW(port,mask)  (*port&=~mask)
        #define FAST_IO_READ(port,mask) ((*port&mask)!=0)
    #elif defined(__STM32__)
        #define FAST_IO_HIGH(port,mask) GPIO_HIGH(port,mask)
        #define FAST_IO_LOW(port,mask)  GPIO_LOW(port,mask)
        #define FAST_IO_READ(port,mask) GPIO_READ(port,mask)
    #endif
    
    #define SCK_HIGH()  FAST_IO_HIGH(sck_port,sck_pinmask)
    #define SCK_LOW()   FAST_IO_LOW(sck_port,sck_pinmask)
    #define MOSI_HIGH() FAST_IO_HIGH(mosi_port,mosi_pinmask)
    #define MOSI_LOW()  FAST_IO_LOW(mosi_port,mosi_pinmask)
    #define MISO_READ() FAST_IO_READ(miso_port,miso_pinmask)
#else
    #define SCK_HIGH()  digitalWrite(SCK_Pin,HIGH)
    #define SCK_LOW()   digitalWrite(SCK_Pin,LOW)
    #define MOSI_HIGH() digitalWrite(MOSI_Pin,HIGH)
    #define MOSI_LOW()  digitalWrite(MOSI_Pin,LOW)
    #define MISO_READ() digitalRead(MISO_Pin)
#endif

SoftwareSPIClass::SoftwareSPIClass(uint8_t sck_pin, uint8_t mosi_pin, uint8_t miso_pin)
{
    SCK_Pin = sck_pin;
    MOSI_Pin = mosi_pin;
    MISO_Pin = miso_pin;
#ifdef SWSPI_FAST_IO
    #if defined(__AVR__)
        sck_port = portOutputRegister(digitalPinToPort(SCK_Pin));
        sck_pinmask	= digitalPinToBitMask(SCK_Pin);
        mosi_port = portOutputRegister(digitalPinToPort(MOSI_Pin));
        mosi_pinmask = digitalPinToBitMask(MOSI_Pin);
        miso_port = portInputRegister(digitalPinToPort(MISO_Pin));
        miso_pinmask = digitalPinToBitMask(MISO_Pin);
    #elif defined(__STM32__)
        sck_port = digitalPinToPort(SCK_Pin);
        sck_pinmask	= digitalPinToBitMask(SCK_Pin);
        mosi_port = digitalPinToPort(MOSI_Pin);
        mosi_pinmask = digitalPinToBitMask(MOSI_Pin);
        miso_port = digitalPinToPort(MISO_Pin);
        miso_pinmask = digitalPinToBitMask(MISO_Pin);
    #endif
#endif
}

void SoftwareSPIClass::begin(void)
{
    pinMode(SCK_Pin, OUTPUT);
    pinMode(MOSI_Pin, OUTPUT);
    pinMode(MISO_Pin, INPUT);
}

void SoftwareSPIClass::begin(uint32_t clock, uint16_t dataOrder, uint16_t dataMode)
{
    begin();
    setClock(clock);
    setBitOrder(dataOrder);
    setDataMode(dataMode);
}

void SoftwareSPIClass::begin(SPISettings settings)
{
    begin();
    setClock(settings.clock);
    setBitOrder(settings.bitOrder);
    setDataMode(settings.dataMode);
}

void SoftwareSPIClass::end(void)
{
}

void SoftwareSPIClass::setClock(uint32_t clock)
{
}

void SoftwareSPIClass::setClockDivider(uint32_t div)
{
}

void SoftwareSPIClass::setBitOrder(uint16_t bitOrder)
{
}

void SoftwareSPIClass::setDataSize(uint16_t datasize)
{
}

void SoftwareSPIClass::setDataMode(uint8_t dataMode)
{
}

void SoftwareSPIClass::beginTransaction(SPISettings settings)
{
}

void SoftwareSPIClass::endTransaction(void)
{
}


uint8_t SoftwareSPIClass::read(void)
{
    return transfer(0);
}

void SoftwareSPIClass::read(uint8_t *buf, uint32_t len)
{
    uint32_t rxed = 0;

    while (rxed < len)
    {
        buf[rxed++] = transfer(0); 
    }
}

void SoftwareSPIClass::write(uint8_t data)
{
    for(uint8_t bit = 0x80; bit; bit >>= 1)
    {
        (data & bit) ? MOSI_HIGH() : MOSI_LOW();
        SCK_HIGH();
        SCK_LOW();
    }
}

void SoftwareSPIClass::write(const uint8_t *data, uint32_t length)
{
    uint32_t txed = 0;
    while (txed < length)
    {
        write(data[txed]);
        txed++;
    }
}

uint16_t SoftwareSPIClass::transfer16(uint16_t wr_data) const
{
    for(uint8_t i = 0; i < 16; i++)
    {
        (wr_data & 0x8000) ? MOSI_HIGH() : MOSI_LOW();
        SCK_HIGH();
        wr_data <<= 1;
        if(MISO_READ()) wr_data |= 1;
        SCK_LOW();
   }
   return wr_data;
}

uint8_t SoftwareSPIClass::transfer(uint8_t wr_data) const
{
    for(uint8_t i = 0; i < 8; i++)
    {
        (wr_data & 0x80) ? MOSI_HIGH() : MOSI_LOW();
        SCK_HIGH();
        wr_data <<= 1;
        if(MISO_READ()) wr_data |= 1;
        SCK_LOW();
   }
   return wr_data;
}

uint8_t SoftwareSPIClass::send(uint8_t data)
{
    return transfer(data);
}

uint8_t SoftwareSPIClass::send(uint8_t *buf, uint32_t len)
{
    uint32_t txed = 0;
    uint8_t ret = 0;
    while (txed < len)
    {
        this->write(buf[txed++]);
        ret = this->read();
    }
    return ret;
}

uint8_t SoftwareSPIClass::recv(void)
{
    return this->read();
}
