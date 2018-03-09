#ifndef HARDWARESPI_H
#define HARDWARESPI_H

extern "C"{
#include "spi.h"
}
#include "Arduino.h"

// SPI_HAS_TRANSACTION means SPI has
//   - beginTransaction()
//   - endTransaction()
//   - usingInterrupt()
//   - SPISetting(clock, bitOrder, dataMode)
//#define SPI_HAS_TRANSACTION

#define SPI_CLOCK_DIV2   SPI_BaudRatePrescaler_2
#define SPI_CLOCK_DIV4   SPI_BaudRatePrescaler_4
#define SPI_CLOCK_DIV8   SPI_BaudRatePrescaler_8
#define SPI_CLOCK_DIV16  SPI_BaudRatePrescaler_16
#define SPI_CLOCK_DIV32  SPI_BaudRatePrescaler_32
#define SPI_CLOCK_DIV64  SPI_BaudRatePrescaler_64
#define SPI_CLOCK_DIV128 SPI_BaudRatePrescaler_128
#define SPI_CLOCK_DIV256 SPI_BaudRatePrescaler_256

#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

#define DATA_SIZE_8BIT SPI_DataSize_8b
#define DATA_SIZE_16BIT SPI_DataSize_16b

#define SPI_RETRY_TIMES 500

#define BitOrder uint16_t
#define spi_dev SPI_TypeDef

typedef enum{
	SPI_STATE_IDLE,
	SPI_STATE_READY,
	SPI_STATE_RECEIVE,
	SPI_STATE_TRANSMIT,
	SPI_STATE_TRANSFER
}spi_mode_t;

//class SPISettings
//{
//public:
//	SPISettings(uint32_t clock, BitOrder bitOrder, uint8_t dataMode) {
//		if (__builtin_constant_p(clock)) {
//			init_AlwaysInline(clock, bitOrder, dataMode, DATA_SIZE_8BIT);
//		} else {
//			init_MightInline(clock, bitOrder, dataMode, DATA_SIZE_8BIT);
//		}
//	}
//	SPISettings(uint32_t clock, BitOrder bitOrder, uint8_t dataMode, uint32_t dataSize) {
//		if (__builtin_constant_p(clock)) {
//			init_AlwaysInline(clock, bitOrder, dataMode, dataSize);
//		} else {
//			init_MightInline(clock, bitOrder, dataMode, dataSize);
//		}
//	}
//	SPISettings(uint32_t clock) {
//		if (__builtin_constant_p(clock)) {
//			init_AlwaysInline(clock, MSBFIRST, SPI_MODE0, DATA_SIZE_8BIT);
//		} else {
//			init_MightInline(clock, MSBFIRST, SPI_MODE0, DATA_SIZE_8BIT);
//		}
//	}
//	SPISettings() { init_AlwaysInline(4000000, MSBFIRST, SPI_MODE0, DATA_SIZE_8BIT); }
//private:
//	void init_MightInline(uint32_t clock, BitOrder bitOrder, uint8_t dataMode, uint32_t dataSize) {
//		init_AlwaysInline(clock, bitOrder, dataMode, dataSize);
//	}
//	void init_AlwaysInline(uint32_t clock, BitOrder bitOrder, uint8_t dataMode, uint32_t dataSize) __attribute__((__always_inline__)) {
//		this->clock = clock;
//		this->bitOrder = bitOrder;
//		this->dataMode = dataMode;
//		this->dataSize = dataSize;
//	}
//	uint32_t clock;
//  uint32_t dataSize;
//  uint32_t clockDivider;
//	BitOrder bitOrder;
//	uint8_t dataMode;
//  uint8_t _SSPin;
//	volatile spi_mode_t state;
//	spi_dev *spi_d;
//	//dma_channel spiRxDmaChannel, spiTxDmaChannel;
//	//dma_dev* spiDmaDev;
//  void (*receiveCallback)(void) = NULL;
//  void (*transmitCallback)(void) = NULL;
//	
//	friend class SPIClass;
//};


class SPIClass
{
	public:
		SPIClass(SPI_TypeDef* _SPIx);
		/**
		 * @brief Equivalent to begin(SPI_1_125MHZ, MSBFIRST, 0).
		 */
		void begin(void);
		void begin(uint32_t speedMaximum,uint16_t dataOrder,uint16_t dataMode);
		/**
		 * @brief Turn on a SPI port and set its GPIO pin modes for use as a slave.
		 *
		 * SPI port is enabled in full duplex mode, with software slave management.
		 *
		 * @param bitOrder Either LSBFIRST (little-endian) or MSBFIRST(big-endian)
		 * @param mode SPI mode to use
		 */
		void beginSlave(uint32_t bitOrder, uint32_t mode);
		/**
		 * @brief Equivalent to beginSlave(MSBFIRST, 0).
		 */
		void beginSlave(void);
		/**
		 * @brief Disables the SPI port, but leaves its GPIO pin modes unchanged.
		 */
		void end(void);
		void Settings(uint32_t speedMaximum,uint16_t dataOrder,uint16_t dataMode);
		void beginTransaction(void);
		void endTransaction(void);

		void beginTransactionSlave(void);

		void setClockDivider(uint16_t clockDivider);
		void setBitOrder(uint16_t bitOrder);
		void setDataMode(uint8_t dataMode);		
		
		// SPI Configuration methods
		void attachInterrupt(void);
		void detachInterrupt(void);
		
		/*	Victor Perez. Added to change datasize from 8 to 16 bit modes on the fly.
		*	Input parameter should be SPI_CR1_DFF set to 0 or 1 on a 32bit word.
		*	Requires an added function spi_data_size on  STM32F1 / cores / maple / libmaple / spi.c 
		*/
		void setDataSize(uint16_t datasize);
		
		
			/*
			 * I/O
			 */

			/**
			 * @brief Return the next unread byte.
			 *
			 * If there is no unread byte waiting, this function will block
			 * until one is received.
			 */
			uint8_t read(void);

			/**
			 * @brief Read length bytes, storing them into buffer.
			 * @param buffer Buffer to store received bytes into.
			 * @param length Number of bytes to store in buffer.  This
			 *               function will block until the desired number of
			 *               bytes have been read.
			 */
			void read(uint8_t *buffer, uint32_t length);

			/**
			 * @brief Transmit a byte.
			 * @param data Byte to transmit.
			 */
	//    void write(uint8 data);

			/**
			 * @brief Transmit a half word.
			 * @param data to transmit.
			 */
			void write(uint16_t data);	
		
			/**
			 * @brief Transmit multiple bytes.
			 * @param buffer Bytes to transmit.
			 * @param length Number of bytes in buffer to transmit.
			 */
			void write(const uint8_t *buffer, uint32_t length);

			/**
			 * @brief Transmit a byte, then return the next unread byte.
			 *
			 * This function transmits before receiving.
			 *
			 * @param data Byte to transmit.
			 * @return Next unread byte.
			 */
			uint8_t transfer(uint8_t data) const;
			uint16_t transfer16(uint16_t data) const;
			
			uint8_t send(uint8_t data);

			/**
			 * @brief Deprecated.
			 *
			 * Use HardwareSPI::write() in combination with
			 * HardwareSPI::read() (or HardwareSPI::transfer()) instead.
			 *
			 * @see HardwareSPI::write()
			 * @see HardwareSPI::read()
			 * @see HardwareSPI::transfer()
			 */
			uint8_t send(uint8_t *data, uint32_t length);

			/**
			 * @brief Deprecated.
			 *
			 * Use HardwareSPI::read() instead.
			 *
			 * @see HardwareSPI::read()
			 */
			uint8_t recv(void);
		
	private:
		SPI_TypeDef* SPIx;
};

extern SPIClass SPI;
extern SPIClass SPI_2;

#endif
