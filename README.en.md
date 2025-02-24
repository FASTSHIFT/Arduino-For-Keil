[![ch](https://img.shields.io/badge/lang-ch-red.svg)](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/README.md)

# Arduino for Keil
## 1. Outline
>This is a lightweight Arduino framework that makes STM32 series microcontrollers compatible with Arduino syntax, allowing for compilation and debugging in Keil.

Advantages compared to traditional development methods:
>
>   1.Shares the [ecosystem](https://github.com/topics/arduino-library) with Arduino, reducing the learning curve and simplifying the development process.
>
>   2.Key parts use an optimization approach of **registers + macros**, reducing function call overhead and improving runtime efficiency.
>
>   3.The simplified Arduino framework has a smaller code size and faster compilation and execution speed compared to [stm32duino](https://github.com/stm32duino) and the HAL library.

## 2. To use
>   The underlying layer is based on the **AT32/STM32 Standard Peripheral Library** for secondary encapsulation, which must be installed for proper compilation. The download link is provided below.

[AT32F403A Standard Peripheral Library](http://www.arterytek.com/download/Pack_Keil_AT32F4xx_CH_V1.3.4.zip)

[AT32F43x/421 Standard Peripheral Library](https://www.arterytek.com/download/PACK/Keil5_AT32MCU_AddOn_V2.1.7.zip)

[STM32F0xx Standard Peripheral Library](https://keilpack.azureedge.net/pack/Keil.STM32F0xx_DFP.1.0.1.pack)

[STM32F10x Standard Peripheral Library](https://keilpack.azureedge.net/pack/Keil.STM32F1xx_DFP.1.1.0.pack)

[STM32F3xx Standard Peripheral Library](https://keilpack.azureedge.net/pack/Keil.STM32F3xx_DFP.1.2.0.pack)

[STM32F4xx Standard Peripheral Library](https://keilpack.azureedge.net/pack/Keil.STM32F4xx_DFP.1.0.8.pack)

[STM32L1xx Standard Peripheral Library](https://keilpack.azureedge.net/pack/Keil.STM32L1xx_DFP.1.0.2.pack)

**Note:** If you have already installed a higher version of the pack, you need to use the package manager provided by Keil to remove the higher version of the pack.

>The general usage can be directly referenced from [Arduino syntax](https://www.arduino.cc/reference/en/).
>
>Advanced usage can be referenced in [Example](https://github.com/FASTSHIFT/Arduino-For-Keil/tree/master/Example).
>
>For third-party library porting, refer to [Third-Party Library Porting Example](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library).

Supports the mixed use of registers and functions from the standard peripheral library, **ensuring development flexibility**:
```C
void setup()
{
    pinMode(PA0,OUTPUT);                //Use the Arduino function to configure PA0 as an output mode
}

void loop()
{
    GPIOA->BSRR = GPIO_Pin_0;           //Use the register to pull the PA0 level high
    delay(1000);                        //Delay for one second
    GPIO_ResetBits(GPIOA, GPIO_Pin_0);  //Use the functions from the standard peripheral library to pull the PA0 level low
    delay(1000);                        //Delay for one second
}
```

## 3. System Framework
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/Framework.png)

## 4. Notice
     1.Please do not delete the main function in "main.cpp". 
     2.When adding third-party library functions, be sure to include the full path and the corresponding .cpp source file. 
     3.Due to platform differences, some library functions may need modifications to compile correctly. Please follow the compiler's suggestions for specific changes, or you can submit Issues for assistance.
