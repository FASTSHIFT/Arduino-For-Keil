# A Cross-platform migration Project @FASTSHIFT
# Arduino for AT32F4xx

# https://github.com/FASTSHIFT/Arduino-For-Keil

//*********************************UPGRADE LOGS************************************//

//Finish in 2019.11.10    V_1.0 基于AT32F4xx标准外设库(1.2.0版)，移植了全部的ArduinoAPI
//Upgrade   2019.11.14    V_1.1 更新timer库，更精确的定时频率
//Upgrade   2019.11.23    V_1.2 修复定时器库频率不对的BUG
//Upgrade   2019.12.1     V_1.3 更新timer库，使用独立的开根号运算函数，不依赖math.h库
//Upgrade   2019.12.6     V_1.4 工程框架修改，工程文件与代码文件分离，使Core里的一些函数命名更加规范更贴近标准库，添加了GNU支持
//Upgrade   2020.1.15     V_1.5 由于Print库的printf不支持浮点输出，将Print库还原，使用vsnprintf转换printf 