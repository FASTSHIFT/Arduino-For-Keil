/*
 * MIT License
 * Copyright (c) 2019 _VIFEXTech
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "HardwareSerial.h"

/**
  * @brief  串口对象构造函数
  * @param  串口外设地址
  * @retval 无
  */
HardwareSerial::HardwareSerial(USART_TypeDef *_USARTx)
{
    this->USARTx = _USARTx;
    USART_Function = 0;
    _rx_buffer_head = _rx_buffer_tail = 0;
}

/**
  * @brief  串口中断入口
  * @param  无
  * @retval 无
  */
void HardwareSerial::IRQHandler()
{
    if(USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)
    {
        uint8_t c = USART_ReceiveData(USARTx);
        uint16_t i = (uint16_t)(_rx_buffer_head + 1) % SERIAL_RX_BUFFER_SIZE;
        if (i != _rx_buffer_tail)
        {
            _rx_buffer[_rx_buffer_head] = c;
            _rx_buffer_head = i;
        }

        if(USART_Function)
            USART_Function();

        USART_ClearITPendingBit(USARTx, USART_IT_RXNE);
    }
}

/**
  * @brief  串口初始化
  * @param  BaudRate: 波特率
  * @retval 无
  */
void HardwareSerial::begin(uint32_t BaudRate)
{
    begin(BaudRate, SERIAL_Config_Default);
}

/**
  * @brief  串口初始化
  * @param  BaudRate: 波特率
  * @param  Config: 配置参数
  * @retval 无
  */
void HardwareSerial::begin(uint32_t BaudRate, SERIAL_Config Config)
{
    begin(BaudRate, Config, USART_PreemptionPriority_Default, USART_SubPriority_Default);
}

/**
  * @brief  串口初始化
  * @param  BaudRate: 波特率
  * @param  Config: 配置参数
  * @param  PreemptionPriority: 抢占优先级
  * @param  SubPriority: 从优先级
  * @retval 无
  */
void HardwareSerial::begin(uint32_t BaudRate, SERIAL_Config Config, uint8_t PreemptionPriority, uint8_t SubPriority)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_TypeDef *GPIOx;
    uint16_t Tx_Pin, Rx_Pin;

    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    uint8_t USARTx_IRQn;

    if(USARTx == USART1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
        Tx_Pin = GPIO_Pin_9;
        Rx_Pin = GPIO_Pin_10;
        GPIOx = GPIOA;
        USARTx_IRQn = USART1_IRQn;
    }
    else if(USARTx == USART2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        Tx_Pin = GPIO_Pin_2;
        Rx_Pin = GPIO_Pin_3;
        GPIOx = GPIOA;
        USARTx_IRQn = USART2_IRQn;
    }
    else if(USARTx == USART3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        Tx_Pin = GPIO_Pin_10;
        Rx_Pin = GPIO_Pin_11;
        GPIOx = GPIOB;
        USARTx_IRQn = USART3_IRQn;
    }

    //USART_TX
    GPIO_InitStructure.GPIO_Pin = Tx_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOx, &GPIO_InitStructure);//初始化

    //USART_RX
    GPIO_InitStructure.GPIO_Pin = Rx_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOx, &GPIO_InitStructure);//初始化

    //USART NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority ;//抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;        //子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器

    //USART 初始化设置
    USART_InitStructure.USART_BaudRate = BaudRate;//串口波特率
    USART_InitStructure.USART_WordLength = USART_GetWordLength(Config);//字长数据格式
    USART_InitStructure.USART_Parity = USART_GetParity(Config);//奇偶校验位
    USART_InitStructure.USART_StopBits = USART_GetStopBits(Config);//停止位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式

    USART_Init(USARTx, &USART_InitStructure); //初始化串口
    USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);//开启串口接受中断
    USART_Cmd(USARTx, ENABLE);                    //使能串口
}

/**
  * @brief  关闭串口
  * @param  无
  * @retval 无
  */
void HardwareSerial::end(void)
{
    USART_Cmd(USARTx, DISABLE);
}

/**
  * @brief  串口中断回调
  * @param  Function: 回调函数
  * @retval 无
  */
void HardwareSerial::attachInterrupt(USART_CallbackFunction_t Function)
{
    USART_Function = Function;
}

/**
  * @brief  获取可从串行端口读取的字节数
  * @param  无
  * @retval 可读取的字节数
  */
int HardwareSerial::available(void)
{
    return ((unsigned int)(SERIAL_RX_BUFFER_SIZE + _rx_buffer_head - _rx_buffer_tail)) % SERIAL_RX_BUFFER_SIZE;
}

/**
  * @brief  读取传入的串行数据(字符)
  * @param  无
  * @retval 可用的传入串行数据的第一个字节 (如果没有可用的数据, 则为-1)
  */
int HardwareSerial::read(void)
{
    // if the head isn't ahead of the tail, we don't have any characters
    if (_rx_buffer_head == _rx_buffer_tail)
    {
        return -1;
    }
    else
    {
        unsigned char c = _rx_buffer[_rx_buffer_tail];
        _rx_buffer_tail = (rx_buffer_index_t)(_rx_buffer_tail + 1) % SERIAL_RX_BUFFER_SIZE;
        return c;
    }
}

/**
  * @brief  返回传入串行数据的下一个字节(字符), 而不将其从内部串行缓冲区中删除
  * @param  无
  * @retval 可用的传入串行数据的第一个字节 (如果没有可用的数据, 则为-1)
  */
int HardwareSerial::peek(void)
{
    if (_rx_buffer_head == _rx_buffer_tail)
    {
        return -1;
    }
    else
    {
        return _rx_buffer[_rx_buffer_tail];
    }
}

/**
  * @brief  清空串口缓存
  * @param  无
  * @retval 无
  */
void HardwareSerial::flush(void)
{
    _rx_buffer_head = _rx_buffer_tail;
}

/**
  * @brief  串口写入一个字节
  * @param  写入的字节
  * @retval 字节
  */
size_t HardwareSerial::write(uint8_t n)
{
    while(!USART_GetFlagStatus(USARTx, USART_FLAG_TXE)){};
    USART_SendData(USARTx, n);
    return 1;
}

//Creat object for user
HardwareSerial Serial(USART1);//TX-PA9 RX-PA10
HardwareSerial Serial2(USART2);//TX-PA2 RX-PA3
HardwareSerial Serial3(USART3);//TX-PB10 RX-PB11

//USARTx_IRQHandler
extern "C" {
    void USART1_IRQHandler(void)
    {
        Serial.IRQHandler();
    }

    void USART2_IRQHandler(void)
    {
        Serial2.IRQHandler();
    }

    void USART3_IRQHandler(void)
    {
        Serial3.IRQHandler();
    }
}
