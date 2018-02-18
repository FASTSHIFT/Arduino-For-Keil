#include "HardwareSPI.h"

SPI_TypeDef* SPIx;

HardwareSPI::HardwareSPI(SPI_TypeDef* _SPIx)
{
	SPIx = _SPIx;
}

void HardwareSPI::begin(void) 
{
	SPI_begin(SPIx);
	SPI_Settings(	SPIx,
								SPI_Mode_Master,
								SPI_DataSize_8b,
								SPI_MODE0,
								SPI_NSS_Soft,
								SPI_BaudRatePrescaler_8,
								SPI_FirstBit_MSB);
}

void HardwareSPI::begin(uint32_t speedMaximum,uint16_t dataOrder,uint16_t dataMode)
{	
	begin();
	Settings(speedMaximum,dataOrder,dataMode);
}

void HardwareSPI::beginSlave(void)
{
  SPI_begin(SPIx);
	SPI_Settings(	SPIx,
								SPI_Mode_Slave,
								SPI_DataSize_8b,
								SPI_MODE0,
								SPI_NSS_Hard,
								SPI_BaudRatePrescaler_8,
								SPI_FirstBit_MSB);
}

void HardwareSPI::Settings(uint32_t speedMaximum,uint16_t dataOrder,uint16_t dataMode)
{
	uint16_t SPI_BaudRatePrescaler;
	speedMaximum = 72000000 / speedMaximum;
			 if(speedMaximum <= 2)SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	else if(speedMaximum <= 4)SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	else if(speedMaximum <= 8)SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	else if(speedMaximum <= 16)SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	else if(speedMaximum <= 32)SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	else if(speedMaximum <= 64)SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	else if(speedMaximum <= 128)SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	else SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	
	setClockDivider(SPI_BaudRatePrescaler);
	setBitOrder(dataOrder);
	setDataMode(dataMode);
}

void HardwareSPI::end(void)
{
  SPI_Cmd(SPIx, DISABLE);
}

void HardwareSPI::setClockDivider(uint16_t clockDivider)
{
	SPI_setClockDivider(SPIx,clockDivider);
}

void HardwareSPI::setBitOrder(uint16_t bitOrder)
{
	if(bitOrder)SPI_setBitOrder(SPIx,SPI_FirstBit_MSB);//MSBFIRST 1
	else SPI_setBitOrder(SPIx,SPI_FirstBit_LSB);
}

/*	Victor Perez. Added to test changing datasize from 8 to 16 bit modes on the fly.
*	Input parameter should be SPI_CR1_DFF set to 0 or 1 on a 32bit word.
*	
*/
void HardwareSPI::setDataSize(uint16_t datasize)
{
	SPI_setDataSize(SPIx,datasize);
}

void HardwareSPI::setDataMode(uint8_t dataMode)
{
/* Notes.  As far as I can tell, the AVR numbers for dataMode appear to match the numbers required by the STM32

From the AVR doc http://www.atmel.com/images/doc2585.pdf section 2.4

SPI Mode 	CPOL 	CPHA 	Shift SCK-edge 	Capture SCK-edge
0 			0 		0 		Falling 		Rising
1 			0 		1 		Rising 			Falling
2 			1 		0 		Rising 			Falling
3 			1 		1 		Falling 		Rising
 
 
On the STM32 it appears to be

bit 1 - CPOL : Clock polarity
    (This bit should not be changed when communication is ongoing)
    0 : CLK to 0 when idle
    1 : CLK to 1 when idle
 
bit 0 - CPHA : Clock phase
    (This bit should not be changed when communication is ongoing)
    0 : The first clock transition is the first data capture edge
    1 : The second clock transition is the first data capture edge
 
If someone finds this is not the case or sees a logic error with this let me know ;-) 
 */
	SPI_setDataMode(SPIx,dataMode);
}

void HardwareSPI::beginTransaction(void)
{
//	setBitOrder(settings.bitOrder);
//	setDataMode(settings.dataMode);
//	setDataSize(settings.dataSize);
//	setClockDivider(determine_baud_rate(_currentSetting->spi_d, settings.clock));
	SPI_begin(SPIx);
}

void HardwareSPI::beginTransactionSlave(void)
{
//	setBitOrder(settings.bitOrder);
//	setDataMode(settings.dataMode);
//	setDataSize(settings.dataSize);
	beginSlave();
}

void HardwareSPI::endTransaction(void)
{
	SPI_Cmd(SPIx, DISABLE);
}


uint8_t HardwareSPI::read(void) 
{
    uint8_t buf[1];
    this->read(buf, 1);
    return buf[0];
}

void HardwareSPI::read(uint8_t *buf, uint32_t len)
{
  uint32_t rxed = 0;
	uint16_t retry=0;
	
	while (rxed < len)
	{
		while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
			retry++;
			if(retry>=SPI_RETRY_TIMES)return;
		}
		buf[rxed++] = (uint8_t)SPI_I2S_ReceiveData(SPIx); //返回通过SPIx最近接收的数据	
	}
}

void HardwareSPI::write(uint16_t data)
{
	uint16_t retry=0;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>=SPI_RETRY_TIMES)return;
	}			  
	SPI_I2S_SendData(SPIx, data); //通过外设SPIx发送一个数据
  //  this->write(&data, 1);

	/* Added for 16bit data Victor Perez. Roger Clark 
	 * Improved speed by just directly writing the single byte to the SPI data reg and wait for completion,	 * by taking the Tx code from transfer(byte)
	 * The original method, of calling write(*data, length) .
	 * This almost doubles the speed of this function.
	 */
  
//	spi_tx_reg(_currentSetting->spi_d, data); // "2. Write the first data item to be transmitted into the SPI_DR register (this clears the TXE flag)."
//	while (spi_is_tx_empty(_currentSetting->spi_d) == 0); // "5. Wait until TXE=1 ..."
//	while (spi_is_busy(_currentSetting->spi_d) != 0); // "... and then wait until BSY=0 before disabling the SPI." 
}

void HardwareSPI::write(const uint8_t *data, uint32_t length)
{
	uint32_t txed = 0;
	uint16_t retry=0;
	while (txed < length)
	{
		while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
			retry++;
			if(retry>SPI_RETRY_TIMES)return;
		}
		SPI_I2S_SendData(SPIx, data[txed]);
		txed++;
		//txed += spi_tx(_currentSetting->spi_d, data + txed, length - txed);
	}
//	while (spi_is_tx_empty(_currentSetting->spi_d) == 0); // "4. After writing the last data item into the SPI_DR register, wait until TXE=1 ..."
//	while (spi_is_busy(_currentSetting->spi_d) != 0); // "... then wait until BSY=0, this indicates that the transmission of the last data is complete."
//	// taken from SdSpiSTM32F1.cpp - Victor's lib, and adapted to support device selection
//	if (spi_is_rx_nonempty(_currentSetting->spi_d))
//	{
//		uint8_t b = spi_rx_reg(_currentSetting->spi_d);
//	}
}

uint16_t HardwareSPI::transfer16(uint16_t wr_data) const
{
	u16 retry=0;
	u16 rd_data;		
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>=SPI_RETRY_TIMES)return 0;
	}			  
	SPI_I2S_SendData(SPIx, wr_data); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>=SPI_RETRY_TIMES)return 0;
	}
		
	rd_data = SPI_I2S_ReceiveData(SPIx); //返回通过SPIx最近接收的数据	
//	spi_tx_reg(_currentSetting->spi_d, wr_data); // "2. Write the first data item to be transmitted into the SPI_DR register (this clears the TXE flag)."
//	while (spi_is_rx_nonempty(_currentSetting->spi_d) == 0); // "4. Wait until RXNE=1 ..."
//	uint16_t rd_data = spi_rx_reg(_currentSetting->spi_d); // "... and read the last received data."
////	while (spi_is_tx_empty(_currentSetting->spi_d) == 0); // "5. Wait until TXE=1 ..."
////	while (spi_is_busy(_currentSetting->spi_d) != 0); // "... and then wait until BSY=0 before disabling the SPI."
	return rd_data;
}

uint8_t HardwareSPI::transfer(uint8_t wr_data) const
{
	u16 retry=0;
	u8 rd_data;		
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>=SPI_RETRY_TIMES)return 0;
	}			  
	SPI_I2S_SendData(SPIx, wr_data); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>=SPI_RETRY_TIMES)return 0;
	}
		
	rd_data = SPI_I2S_ReceiveData(SPIx); //返回通过SPIx最近接收的数据
	return rd_data;
//	spi_tx_reg(_currentSetting->spi_d, byte); // "2. Write the first data item to be transmitted into the SPI_DR register (this clears the TXE flag)."
//  	while (spi_is_rx_nonempty(_currentSetting->spi_d) == 0); // "4. Wait until RXNE=1 ..."
//  	uint8 b = spi_rx_reg(_currentSetting->spi_d); // "... and read the last received data."
//  	while (spi_is_tx_empty(_currentSetting->spi_d) == 0); // "5. Wait until TXE=1 ..."
//  	while (spi_is_busy(_currentSetting->spi_d) != 0); // "... and then wait until BSY=0 before disabling the SPI."
//    return b;
}

uint8_t HardwareSPI::send(uint8_t data)
{
	uint8_t buf[] = {data};
	return this->send(buf, 1);
}

uint8_t HardwareSPI::send(uint8_t *buf, uint32_t len)
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

uint8_t HardwareSPI::recv(void)
{
    return this->read();
}
	
	
	
//--------------------------DMA----------------------------------------//
///*  Roger Clark and Victor Perez, 2015
//*	Performs a DMA SPI transfer with at least a receive buffer.
//*	If a TX buffer is not provided, FF is sent over and over for the lenght of the transfer. 
//*	On exit TX buffer is not modified, and RX buffer cotains the received data.
//*	Still in progress.
//*/
//uint8 HardwareSPI::dmaTransfer(uint8 *transmitBuf, uint8 *receiveBuf, uint16 length) {
//	if (length == 0) return 0;
//	uint8 b = 0;
//	if (spi_is_rx_nonempty(_currentSetting->spi_d) == 1) b = spi_rx_reg(_currentSetting->spi_d); //Clear the RX buffer in case a byte is waiting on it.
////	dma1_ch3_Active=true;
//    dma_init(_currentSetting->spiDmaDev);
////	dma_attach_interrupt(DMA1, DMA_CH3, &HardwareSPI::DMA1_CH3_Event);
//	
//	// RX
//	spi_rx_dma_enable(_currentSetting->spi_d);
//	dma_setup_transfer(_currentSetting->spiDmaDev, _currentSetting->spiRxDmaChannel, &_currentSetting->spi_d->regs->DR, DMA_SIZE_8BITS,
//                     receiveBuf, DMA_SIZE_8BITS, (DMA_MINC_MODE | DMA_TRNS_CMPLT));// receive buffer DMA
//	dma_set_num_transfers(_currentSetting->spiDmaDev, _currentSetting->spiRxDmaChannel, length);
//	
//	// TX
//	spi_tx_dma_enable(_currentSetting->spi_d);	
//	if (!transmitBuf) {
//	static uint8_t ff = 0XFF;
//	transmitBuf = &ff;
//	dma_setup_transfer(_currentSetting->spiDmaDev, _currentSetting->spiTxDmaChannel, &_currentSetting->spi_d->regs->DR, DMA_SIZE_8BITS,
//                       transmitBuf, DMA_SIZE_8BITS, (DMA_FROM_MEM | DMA_TRNS_CMPLT));// Transmit FF repeatedly
//	}
//	else {
//    dma_setup_transfer(_currentSetting->spiDmaDev, _currentSetting->spiTxDmaChannel, &_currentSetting->spi_d->regs->DR, DMA_SIZE_8BITS,
//                       transmitBuf, DMA_SIZE_8BITS, (DMA_MINC_MODE |  DMA_FROM_MEM | DMA_TRNS_CMPLT));// Transmit buffer DMA 
//	}
//	dma_set_num_transfers(_currentSetting->spiDmaDev, _currentSetting->spiTxDmaChannel, length);

//	dma_enable(_currentSetting->spiDmaDev, _currentSetting->spiRxDmaChannel);// enable receive
//	dma_enable(_currentSetting->spiDmaDev, _currentSetting->spiTxDmaChannel);// enable transmit
//	
////	while (dma1_ch3_Active);
////	if (receiveBuf) {
//    uint32_t m = millis();
//	while ((dma_get_isr_bits(_currentSetting->spiDmaDev, _currentSetting->spiTxDmaChannel) & 0x2)==0) {//Avoid interrupts and just loop waiting for the flag to be set.
//      if ((millis() - m) > 100)  {
////        dma1_ch3_Active = 0;
//		b = 2;
//		break;
//      }
//    }
//	dma_clear_isr_bits(_currentSetting->spiDmaDev, _currentSetting->spiTxDmaChannel);

////	}
//	while (spi_is_tx_empty(_currentSetting->spi_d) == 0); // "5. Wait until TXE=1 ..."
//	while (spi_is_busy(_currentSetting->spi_d) != 0); // "... and then wait until BSY=0 before disabling the SPI." 
//    dma_disable(_currentSetting->spiDmaDev, _currentSetting->spiTxDmaChannel);
//	dma_disable(_currentSetting->spiDmaDev, _currentSetting->spiRxDmaChannel);
//	spi_rx_dma_disable(_currentSetting->spi_d); // And disable generation of DMA request from the SPI port so other peripherals can use the channels
//	spi_tx_dma_disable(_currentSetting->spi_d);
//	if (spi_is_rx_nonempty(_currentSetting->spi_d) != 0){; // "4. Wait until RXNE=1 ..."
//		uint8 x = spi_rx_reg(_currentSetting->spi_d); // "... and read the last received data."
//	}
//    return b;
//}

///*  Roger Clark and Victor Perez, 2015
//*	Performs a DMA SPI send using a TX buffer.
//*	On exit TX buffer is not modified.
//*	Still in progress.
//*/
//uint8 HardwareSPI::dmaSend(uint8 *transmitBuf, uint16 length, bool minc) {
//	if (length == 0) return 0;
//	uint32 flags = ((DMA_MINC_MODE * minc) |  DMA_FROM_MEM | DMA_TRNS_CMPLT);
//	uint8 b = 0;
////	dma1_ch3_Active=true;
//    dma_init(_currentSetting->spiDmaDev);
////	dma_attach_interrupt(DMA1, DMA_CH3, &HardwareSPI::DMA1_CH3_Event);

//	// TX
//	spi_tx_dma_enable(_currentSetting->spi_d);	
//    dma_setup_transfer(_currentSetting->spiDmaDev, _currentSetting->spiTxDmaChannel, &_currentSetting->spi_d->regs->DR, DMA_SIZE_8BITS,
//                       transmitBuf, DMA_SIZE_8BITS, flags);// Transmit buffer DMA 
//	dma_set_num_transfers(_currentSetting->spiDmaDev, _currentSetting->spiTxDmaChannel, length);
//	dma_enable(_currentSetting->spiDmaDev, _currentSetting->spiTxDmaChannel);// enable transmit
//	
////    while (dma1_ch3_Active);
//	while ((dma_get_isr_bits(_currentSetting->spiDmaDev, _currentSetting->spiTxDmaChannel) & 0x2)==0); //Avoid interrupts and just loop waiting for the flag to be set.
//	dma_clear_isr_bits(_currentSetting->spiDmaDev, _currentSetting->spiTxDmaChannel);

//	while (spi_is_tx_empty(_currentSetting->spi_d) == 0); // "5. Wait until TXE=1 ..."
//	while (spi_is_busy(_currentSetting->spi_d) != 0); // "... and then wait until BSY=0 before disabling the SPI." 
//	dma_disable(_currentSetting->spiDmaDev, _currentSetting->spiTxDmaChannel);
//	spi_tx_dma_disable(_currentSetting->spi_d);
//	if (spi_is_rx_nonempty(_currentSetting->spi_d) != 0){; // "4. Wait until RXNE=1 ..."
//		uint8 x = spi_rx_reg(_currentSetting->spi_d); // "... and read the last received data."
//	}
//    return b;
//}

//uint8 HardwareSPI::dmaSend(uint16 *transmitBuf, uint16 length, bool minc) {
//	if (length == 0) return 0;
//	uint32 flags = ((DMA_MINC_MODE * minc) |  DMA_FROM_MEM | DMA_TRNS_CMPLT);
//	uint8 b;
//	dma1_ch3_Active=true;
//    dma_init(_currentSetting->spiDmaDev);
////	dma_attach_interrupt(DMA1, DMA_CH3, &HardwareSPI::DMA1_CH3_Event);

//	// TX
//	spi_tx_dma_enable(_currentSetting->spi_d);	
//    dma_setup_transfer(_currentSetting->spiDmaDev, _currentSetting->spiTxDmaChannel, &_currentSetting->spi_d->regs->DR, DMA_SIZE_16BITS,
//                       transmitBuf, DMA_SIZE_16BITS, flags);// Transmit buffer DMA 
//	dma_set_num_transfers(_currentSetting->spiDmaDev, _currentSetting->spiTxDmaChannel, length);
//	dma_enable(_currentSetting->spiDmaDev, _currentSetting->spiTxDmaChannel);// enable transmit
//	
////    while (dma1_ch3_Active);
//	while ((dma_get_isr_bits(_currentSetting->spiDmaDev, _currentSetting->spiTxDmaChannel) & 0x2)==0); //Avoid interrupts and just loop waiting for the flag to be set.
//	dma_clear_isr_bits(_currentSetting->spiDmaDev, _currentSetting->spiTxDmaChannel);

//	while (spi_is_tx_empty(_currentSetting->spi_d) == 0); // "5. Wait until TXE=1 ..."
//	while (spi_is_busy(_currentSetting->spi_d) != 0); // "... and then wait until BSY=0 before disabling the SPI." 
//	dma_disable(_currentSetting->spiDmaDev, _currentSetting->spiTxDmaChannel);
//	spi_tx_dma_disable(_currentSetting->spi_d);
//	if (spi_is_rx_nonempty(_currentSetting->spi_d) != 0){; // "4. Wait until RXNE=1 ..."
//		b = spi_rx_reg(_currentSetting->spi_d); // "... and read the last received data."
//	}
//    return b;
//}


//void HardwareSPI::attachInterrupt(void) {
//	// Should be enableInterrupt()
//}

//void HardwareSPI::detachInterrupt(void) {
//	// Should be disableInterrupt()
//}

///*
// * Pin accessors
// */

//uint8 HardwareSPI::misoPin(void) {
//    return dev_to_spi_pins(_currentSetting->spi_d)->miso;
//}

//uint8 HardwareSPI::mosiPin(void) {
//    return dev_to_spi_pins(_currentSetting->spi_d)->mosi;
//}

//uint8 HardwareSPI::sckPin(void) {
//    return dev_to_spi_pins(_currentSetting->spi_d)->sck;
//}

//uint8 HardwareSPI::nssPin(void) {
//    return dev_to_spi_pins(_currentSetting->spi_d)->nss;
//}

///*
// * Deprecated functions
// */

//uint8 HardwareSPI::send(uint8 data) {
//    uint8 buf[] = {data};
//    return this->send(buf, 1);
//}

//uint8 HardwareSPI::send(uint8 *buf, uint32 len) {
//    uint32 txed = 0;
//    uint8 ret = 0;
//    while (txed < len) {
//        this->write(buf[txed++]);
//        ret = this->read();
//    }
//    return ret;
//}

//uint8 HardwareSPI::recv(void) {
//    return this->read();
//}


///*
// * Auxiliary functions
// */

//static const spi_pins* dev_to_spi_pins(spi_dev *dev) {
//    switch (dev->clk_id) {
//#if BOARD_NR_SPI >= 1
//    case RCC_SPI1: return board_spi_pins;
//#endif
//#if BOARD_NR_SPI >= 2
//    case RCC_SPI2: return board_spi_pins + 1;
//#endif
//#if BOARD_NR_SPI >= 3
//    case RCC_SPI3: return board_spi_pins + 2;
//#endif
//    default:       return NULL;
//    }
//}

//static void disable_pwm(const stm32_pin_info *i) {
//    if (i->timer_device) {
//        timer_set_mode(i->timer_device, i->timer_channel, TIMER_DISABLED);
//    }
//}

//static void configure_gpios(spi_dev *dev, bool as_master) {
//    const spi_pins *pins = dev_to_spi_pins(dev);

//    if (!pins) {
//        return;
//    }

//    const stm32_pin_info *nssi = &PIN_MAP[pins->nss];
//    const stm32_pin_info *scki = &PIN_MAP[pins->sck];
//    const stm32_pin_info *misoi = &PIN_MAP[pins->miso];
//    const stm32_pin_info *mosii = &PIN_MAP[pins->mosi];

//    disable_pwm(nssi);
//    disable_pwm(scki);
//    disable_pwm(misoi);
//    disable_pwm(mosii);

//    spi_config_gpios(dev, as_master, nssi->gpio_device, nssi->gpio_bit,
//                     scki->gpio_device, scki->gpio_bit, misoi->gpio_bit,
//                     mosii->gpio_bit);
//}

//static const spi_baud_rate baud_rates[8] __FLASH__ = {
//    SPI_BAUD_PCLK_DIV_2,
//    SPI_BAUD_PCLK_DIV_4,
//    SPI_BAUD_PCLK_DIV_8,
//    SPI_BAUD_PCLK_DIV_16,
//    SPI_BAUD_PCLK_DIV_32,
//    SPI_BAUD_PCLK_DIV_64,
//    SPI_BAUD_PCLK_DIV_128,
//    SPI_BAUD_PCLK_DIV_256,
//};

///*
// * Note: This assumes you're on a LeafLabs-style board
// * (CYCLES_PER_MICROSECOND == 72, APB2 at 72MHz, APB1 at 36MHz).
// */
//static spi_baud_rate determine_baud_rate(spi_dev *dev, uint32_t freq) {
//	uint32_t clock = 0, i;
//	#ifdef SPI_DEBUG
//	Serial.print("determine_baud_rate("); Serial.print(freq); Serial.println(")");
//	#endif
//    switch (rcc_dev_clk(dev->clk_id))
//    {
//    	case RCC_APB2: clock = STM32_PCLK2; break; // 72 Mhz
//    	case RCC_APB1: clock = STM32_PCLK1; break; // 36 Mhz
//    }
//    clock /= 2;
//    i = 0;
//    while (i < 7 && freq < clock) {
//      clock /= 2;
//      i++;
//    }
//	return baud_rates[i];
//}

//HardwareSPI SPI(1);
