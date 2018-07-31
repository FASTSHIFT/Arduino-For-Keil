# Arduino-For-Keil

这是一个跨平台移植的项目，可以在Keil MDK5环境下使用Arduino的函数对STM32F10x系列单片机进行编程。
它本质上是一个建立好的Keil工程，然后在它的基础上封装一套类似于Arduino的底层函数库，可以像在Arduino IDE一样直接调用写好的API，
只需将Arduino支持的第三方库的文件夹加入 "Libraries" 文件夹下即可，然后打开"USER"文件夹启动keil工程，在"main.cpp"这个文件下进行编写程序，
可以直接使用Arduino的示例源代码。

## 注意： 
     1.请不要删除"main.cpp"中的FileGroup.h和main函数。 
     2.添加第三方库函数的时候要加入完整的路径. 
     3.不能保证兼容所有的Arduino第三方库函数，有的函数库可能要进行修改才能正常运行，具体修改方式请按照编译器的提示，或者可以提交Issues。 
      
     它的底层是基于 STM32标准外设库([3.5版本](http://www.keil.com/dd2/pack/#eula-container)) 二次封装，部分源代码来自 stm32duino，用法与Arduino基本一致，支持与寄存器和标准外设库的函数混用。
     
     
## 目前在 STM32F10x 成功移植的API有： 
### 基本：
### 时间:
          delay(Time_ms)
          delayMicroseconds(Time_us)
          millis()
          micros()
          TimerSet(TIMx,InterruptTime_us,Callback_function)
### 输入输出：
          pinMode(Pin,Mode)
          attachInterrrupt(Pin,Trigger_Mode,Callback_function)
          digitalWrite(Pin,State)
          digitalRead(Pin)
          analogRead(Pin)
          analogWrite(Pin,DutyCycle)
          shiftOut(dataPin,clockPin,bitOrder,value)
          shiftIn(dataPin,clockPin,bitOrder)
          tone(Pin,Frequency,Time_ms)
          
     Print类 //支持printf
     String类 
     Stream类 
      
### 外设相关： 
     Serial类 
     Wire类
     SPI类
 
