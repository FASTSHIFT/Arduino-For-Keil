#include "Arduino.h"

#define PWM_Pin PA0

void setup()
{
    /*PWM DutyCycle: 0~1000 -> 0~100%*/
    /*PWM Frequency: 10KHz*/
    PWM_Init(PWM_Pin, 1000, 10000);
}

void loop()
{
    for(int i = 0; i <= 1000; i++)
    {
        pwmWrite(PWM_Pin, i);
        delay(1);
    }
    for(int i = 1000; i > 0; i--)
    {
        analogWrite(PWM_Pin, i);
        delay(1);
    }
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
