# Arduino-For-Keil
> https://github.com/FASTSHIFT/Arduino-For-Keil

# 更新日志
## [v1.0] - 2022.3.31
* 1.完成重构
* 2.ArduinoAPI共用
* 3.支持AT32F4xx和AT32F43x

## [v1.1] - 2022.9.5
* 1.添加AT32F421移植

## [v1.2] - 2022.11.19
* 1.更新AT32F43x/421标准库
* 2.简化itoa，去除无用宏
* 3.配置F421默认时钟为120MHz
* 4.修复F421 GPIO读取异常
* 5.F43x添加Serial4、wdt看门狗支持
* 6.修复F43x GCC警告

## [v1.3] - 2023.1.31
* 1.更新AT32F43x HardwareSerial添加UART5;callback添加userdata
* 2.更新AT32F43x WatchDog接口

## [v1.4] - 2023.6.3
* 1.移动Tone到ArduinoAPI作为公共组件
* 2.更新STM32F4xx到新框架
* 3.AT32F43x添加dwt高精度时间戳支持
* 4.更新gitignore排除临时文件
