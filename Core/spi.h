<<<<<<< HEAD
#ifndef __SPI_H
#define __SPI_H

#ifdef __cplusplus
extern "C"{
#endif

#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "Arduino.h"

void SPI_begin(SPI_TypeDef* SPIx);
void SPI_Settings(	SPI_TypeDef* SPIx,
										u16 SPI_Mode_MS,
										u16 SPI_DataSize,
										u16 SPI_MODE,
										u16 SPI_NSS,
										u16 SPI_BaudRatePrescaler,
										u16 SPI_FirstBit);
void SPI_setBitOrder(SPI_TypeDef* SPIx,u16 SPI_FirstBit);
void SPI_setDataSize(SPI_TypeDef* SPIx,u16 datasize);
void SPI_setClockDivider(SPI_TypeDef* SPIx,u16 clockDivider);
void SPI_setDataMode(SPI_TypeDef* SPIx,uint8_t dataMode);
u8 SPI_ReadWriteByte(SPI_TypeDef* SPIx,u8 TxData);

#ifdef __cplusplus
}
#endif
		 
#endif

=======
#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x.h"
#include "stm32f10x_spi.h"

void SPI_begin(SPI_TypeDef* SPIx);
void SPI_Settings(	SPI_TypeDef* SPIx,
										u16 SPI_Mode_MS,
										u16 SPI_DataSize,
										u16 SPI_MODE,
										u16 SPI_NSS,
										u16 SPI_BaudRatePrescaler,
										u16 SPI_FirstBit);
void SPI_setBitOrder(SPI_TypeDef* SPIx,u16 SPI_FirstBit);
void SPI_setDataSize(SPI_TypeDef* SPIx,u16 datasize);
void SPI_setClockDivider(SPI_TypeDef* SPIx,u16 clockDivider);
void SPI_setDataMode(SPI_TypeDef* SPIx,uint8_t dataMode);
u8 SPI_ReadWriteByte(SPI_TypeDef* SPIx,u8 TxData);//SPI总线读写一个字节
		 
#endif

>>>>>>> 301cbab54594d1de84f6a7e456407b3a84e8d255
