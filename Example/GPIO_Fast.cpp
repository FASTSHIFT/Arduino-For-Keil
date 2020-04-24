#include "Arduino.h"

#define Write_Pin PA0
#define Read_Pin  PA1

void setup()
{
    pinMode(Write_Pin, OUTPUT);
    pinMode(Read_Pin, INPUT);
}

void loop()
{
    int value = digitalRead_FAST(Read_Pin);
    
    digitalWrite_HIGH(Write_Pin);
    digitalWrite_LOW(Write_Pin);
}

/**
  * @brief  Main Function
  * @param  None
  * @retval None
  */
int main(void)
{
    Delay_Init();
    setup();
    for(;;)loop();
}
