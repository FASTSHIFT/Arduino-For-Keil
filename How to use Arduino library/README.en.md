[![ch](https://img.shields.io/badge/lang-ch-red.svg)](https://github.com/anilberg/Arduino-For-Keil/tree/main/How%20to%20use%20Arduino%20library/README.md)

## Example of Porting a Third-Party Library
* 1.从Arduino的安装库的文件夹中找到要调用的第三方库
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library/1.png)

* 2.将刚才找到的第三方库放到Keil工程模板的Libraries文件夹内
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library/2.png)

* 3.启动Keil工程
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library/3.png)

* 4.将第三方库的源代码文件添加到Keil的工程组
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library/4.png)

* 5.找到第三方库文件夹
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library/5.png)

* 6.添加路径
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library/6.png)

* 7.找到第三方库自带的示例程序(一般位于第三方库目录下的examples内)
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library/7.png)

* 8.复制示例程序
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library/8.png)

* 9.将示例程序复制到Keil工程模板的main.cpp里，替换 setup() 和 loop() 函数
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library/9.png)

* 10.修改示例程序里的引脚定义为STM32的引脚
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library/10.png)

* 11.点击编译，如果编译无错误表示移植成功
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library/11.png)
