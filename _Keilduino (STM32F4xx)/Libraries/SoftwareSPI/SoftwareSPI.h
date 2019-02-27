#ifndef __SOFTWARESPI_H
#define __SOFTWARESPI_H

#include "Arduino.h"

#if defined(__AVR__)
    #define SWSPI_FAST_IO
    typedef volatile uint8_t SWSPI_PortReg_t;
    typedef uint8_t SWSPI_PortMask_t;
    
#elif defined(__STM32__)
    #define SWSPI_FAST_IO

    typedef GPIO_TypeDef SWSPI_PortReg_t;
    typedef uint16_t SWPortMask_t;
#endif

typedef enum
{
    SPI_MODE0,
    SPI_MODE1,
    SPI_MODE2,
    SPI_MODE3
} SPI_MODE_TypeDef;

#define DATA_SIZE_8BIT SPI_DataSize_8b
#define DATA_SIZE_16BIT SPI_DataSize_16b

typedef uint16_t BitOrder;

class SPISettings
{
public:
    #define __builtin_constant_p(x) 1
    SPISettings(uint32_t clock, BitOrder bitOrder, uint8_t dataMode) {
        if (__builtin_constant_p(clock)) {
            init_AlwaysInline(clock, bitOrder, dataMode, DATA_SIZE_8BIT);
        } else {
            init_MightInline(clock, bitOrder, dataMode, DATA_SIZE_8BIT);
        }
    }
    SPISettings(uint32_t clock, BitOrder bitOrder, uint8_t dataMode, uint32_t dataSize) {
        if (__builtin_constant_p(clock)) {
            init_AlwaysInline(clock, bitOrder, dataMode, dataSize);
        } else {
            init_MightInline(clock, bitOrder, dataMode, dataSize);
        }
    }
    SPISettings(uint32_t clock) {
        if (__builtin_constant_p(clock)) {
            init_AlwaysInline(clock, MSBFIRST, SPI_MODE0, DATA_SIZE_8BIT);
        } else {
            init_MightInline(clock, MSBFIRST, SPI_MODE0, DATA_SIZE_8BIT);
        }
    }
    SPISettings() {
        init_AlwaysInline(4000000, MSBFIRST, SPI_MODE0, DATA_SIZE_8BIT);
    }
private:
    void init_MightInline(uint32_t clock, BitOrder bitOrder, uint8_t dataMode, uint32_t dataSize) {
        init_AlwaysInline(clock, bitOrder, dataMode, dataSize);
    }
    void init_AlwaysInline(uint32_t clock, BitOrder bitOrder, uint8_t dataMode, uint32_t dataSize) __attribute__((__always_inline__)) {
        this->clock = clock;
        this->bitOrder = bitOrder;
        this->dataMode = dataMode;
        this->dataSize = dataSize;
    }
    uint32_t clock;
    uint32_t dataSize;
    uint32_t clockDivider;
    BitOrder bitOrder;
    uint8_t dataMode;

    friend class SoftwareSPIClass;
};


class SoftwareSPIClass
{
public:
    SoftwareSPIClass(uint8_t sck_pin,uint8_t mosi_pin,uint8_t miso_pin);

    void begin(void);
    void begin(uint32_t clock, uint16_t dataOrder, uint16_t dataMode);
    void begin(SPISettings settings);
    void beginTransaction(SPISettings settings);

    void endTransaction(void);
    void end(void);

    void setClock(uint32_t clock);
    void setClockDivider(uint32_t div);
    void setBitOrder(uint16_t bitOrder);
    void setDataMode(uint8_t dataMode);
    void setDataSize(uint16_t datasize);

    uint8_t read(void);
    void read(uint8_t *buffer, uint32_t length);
    void write(uint8_t data);
    void write(const uint8_t *buffer, uint32_t length);
    uint8_t transfer(uint8_t data) const;
    uint16_t transfer16(uint16_t data) const;
    uint8_t send(uint8_t data);
    uint8_t send(uint8_t *data, uint32_t length);
    uint8_t recv(void);

private:
    uint8_t SCK_Pin;
    uint8_t MOSI_Pin;
    uint8_t MISO_Pin;
#ifdef SWSPI_FAST_IO
    SWSPI_PortReg_t *sck_port, *mosi_port, *miso_port;
    SWPortMask_t sck_pinmask, mosi_pinmask, miso_pinmask;
#endif
};

extern SoftwareSPIClass swSPI;

#endif
