# Arduino_For_Keil

这个项目可以使你在Keil MDK5环境下也可以使用Arduino的函数对STM32F103系列单片机进行编程，理论上可以兼容原本只能在Arduino IDE下编译的函数库，不过需要对移植的库进行修改。
它的底层是基于STM32标准外设库(3.5版本)二次封装。因为本人目前还在学习阶段，所以这个移植工程还有不少BUG，目前只是成功测试了部分的函数。
目前测试成功的函数有：
  基本：
    时间：delay(),delayMicroseconds(),millis(),micros();
    输入输出：pinMode(),digitalWrite(),digitalRead(),analogRead(),shiftOut(),shiftIn();
    Print类
    String类
    Stream类
    
  外设相关：
    串口：Serial.begin(),Serial.write(),Serial.print(),Serial.println();
    Wire类
    

  This project allows you use Arduino library on Keil MDK5 to program the STM32F103 microcontroller series, can theoretically be compatible with the original only driver under the ArduinoIDE compiler, but the need to modify the transplantation of the library. 
  It is based on the STM32 Standard Peripheral Library (V 3.5) encapsulation. because I am still in the learning, so this transplant project still has many BUGs. At present, I have successfully tested some of the functions.
  
  The function of the current successful test:            
    Basic:
      Time: delay(), delayMicroseconds(), millis(), micros();
      Input output: pinMode(), digitalWrite(), digitalRead(), analogRead(), shiftOut(), shiftIn();
      Print class
      String class
      Stream class
      
    Peripherals:
      Serial: Serial.begin(), Serial.write(), Serial.print(), Serial.println();
      Wire class
