#include "FileGroup.h"

//HardwareSerial Serial(USART1);//TX-PA9 RX-PA10
//HardwareSerial Serial2(USART2);//TX-PA2 RX-PA3
//HardwareSerial Serial3(USART3);//TX-PB10 RX-PB11
//HardwareSPI SPI(SPI1);//SCK-PB13 MISO-PB14 MOSI-PB15
//HardwareSPI SPI_2(SPI2);//SCK-PA5 MISO-PA6 MOSI-PA7
//TwoWire Wire(SCL_Pin, SDA_Pin, SOFT_FAST);

void setup()
{
	
}

void loop()
{
	
}

//*****************MAIN******************//
int main(void)
{
	delay_init();
	//ADCx_Init();//If use analogRead
	//ADCx_DMA_Init();
	setup();
	for(;;)loop();
}

//*****************UPGRADE LOGS******************//

//Finish in 2017.11.20 V_1.0 支持pinMode、digitalWrite、digitalRead、delay等基础Arduino API
//Upgrade 			 11.21 V_1.1 改用C++编写程序，可以兼容部分Arduino库函数
//Upgrade 			 11.23 V_1.2 移植Print、Stream、WString等class
//Upgrade 			 12.12 V_1.3 支持millis、micros、Wire class、部分Serial class API(项目已上传GitHub<Arduino_For_Keil>)
//Upgrade 	2018.1.10  V_1.4 全面支持Serial class，以及定时器库
//Upgrade 			 1.12  V_1.5 尝试加入SD卡文件系统库支持，命名"_Keilduino"
//Upgrade 			 1.21  V_1.6 支持analogRead(DMA/NoDMA)、analogWrite
//Upgrade 			 1.23  V_1.7 支持所有GPIO外部中断
//Upgrade 			 1.31  V_1.8 修复引脚Pxx定义错误的智障BUG，TIM2~TIM4已支持输出PWM
//Upgrade 			 2.1 	 V_1.9 HardwareSerial中加入end()、attachInterrupt()
//Upgrade 			 2.10  V_2.0 修改函数指针定义方式 
