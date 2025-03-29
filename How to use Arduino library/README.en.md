[![ch](https://img.shields.io/badge/lang-ch-red.svg)](https://github.com/FASTSHIFT/Arduino-For-Keil/tree/main/How%20to%20use%20Arduino%20library/README.md)

## Example of Porting a Third-Party Library
* 1. Find the third-party library you want to call from the Arduino installed libraries folder.
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library/1.png)

* 2. Place the third-party library you just found into the Libraries folder within the Keil project template.
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library/2.png)

* 3. Start the Keil project.
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library/3.png)

* 4. Add the source code files of the third-party library to the Keil project group.
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library/4.png)

* 5. Find the folder of the third-party library.
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library/5.png)

* 6. Add the path.
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library/6.png)

* 7. Locate the example programs that come with the third-party library, usually found in the examples folder within the library directory.
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library/7.png)

* 8. Copy the example programs.
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library/8.png)

* 9. Replace the setup() and loop() functions in your main.cpp file of the Keil project template with the copied example program.
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library/9.png)

* 10. Modify the pin definitions in the example program to match the STM32 pins.
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library/10.png)

* 11. Click "Compile." If there are no errors during compilation, it indicates that the porting was successful.
![image](https://github.com/FASTSHIFT/Arduino-For-Keil/blob/master/How%20to%20use%20Arduino%20library/11.png)
