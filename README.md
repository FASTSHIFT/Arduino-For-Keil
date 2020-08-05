# Arduino for Keil
## 1.概要
>这是一个轻量级的Arduino框架，使**STM32**系列单片机兼容Arduino语法，在Keil上进行编译调试。

相对于传统开发方式的优势:
>
>   1.与Arduino共用[生态](https://github.com/topics/arduino-library)，降低学习门槛，简化开发过程。
>
>   2.关键部分使用了 **寄存器+宏** 的优化方式，减少函数调用开销，提升运行效率。
>
>   3.简化的Arduino框架，相对[stm32duino](https://github.com/stm32duino)与HAL库，拥有更小的代码体积、更快的编译和执行速度。

## 2.使用
>   底层基于 **AT32/STM32标准外设库** 二次封装，安装之后才能正常编译，下载链接位于下方。

[AT32F4xx标准外设库](http://www.arterytek.com/download/Pack_Keil_AT32F4xx_V1.3.1.zip)

[STM32F0xx标准外设库](https://keilpack.azureedge.net/pack/Keil.STM32F0xx_DFP.1.0.1.pack)

[STM32F10x标准外设库](https://keilpack.azureedge.net/pack/Keil.STM32F1xx_DFP.1.1.0.pack)

[STM32F3xx标准外设库](https://keilpack.azureedge.net/pack/Keil.STM32F3xx_DFP.1.2.0.pack)

[STM32F4xx标准外设库](https://keilpack.azureedge.net/pack/Keil.STM32F4xx_DFP.1.0.8.pack)

[STM32L1xx标准外设库](https://keilpack.azureedge.net/pack/Keil.STM32L1xx_DFP.1.0.2.pack)

**注意** 如果您已经安装了更高版本的pack，需要使用keil自带的包管理器将高版本的pack进行Remove

>一般用法可以直接参照[Arduino语法](https://www.arduino.cc/reference/en/)。
>
>高级用法参考[Example](https://github.com/FASTSHIFT/Arduino-For-Keil/tree/master/Example)内的示例。
>
>第三方库移植参考[第三方库移植示例](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library)。

支持与寄存器和标准外设库的函数混用，**保证开发的灵活性**:
```C
void setup()
{
    pinMode(PA0,OUTPUT);                //使用Arduino函数将配置PA0为输出模式
}

void loop()
{
    GPIOA->BSRR = GPIO_Pin_0;           //使用寄存器将PA0电平拉高
    delay(1000);                        //延时一秒
    GPIO_ResetBits(GPIOA, GPIO_Pin_0);  //使用标准外设库的函数将PA0电平拉低
    delay(1000);                        //延时一秒
}
```

## 3.系统框架
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/Framework.png)

## 4.注意
     1.请不要删除"main.cpp"中的main函数。 
     2.添加第三方库函数的时候要加入完整的路径，以及.cpp源文件. 
     3.由于平台的不同，有的函数库可能要进行修改才能正常编译，具体修改方式请按照编译器的提示，或者可以提交Issues。 

