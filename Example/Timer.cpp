#include "FileGroup.h"

#define LED1_Pin PA0
#define LED2_Pin PA1

void Timer1_Callback()
{
    Serial.println("Timer 1 Interrupt!");
    togglePin(LED1_Pin);
}

void Timer2_Callback()
{
    Serial.println("Timer 2 Interrupt!");
    togglePin(LED2_Pin);
}

void setup()
{
    Serial.begin(115200);
    pinMode(LED1_Pin, OUTPUT);
    pinMode(LED2_Pin, OUTPUT);
    
    Timer_SetInterrupt(TIM1, 1000000/*@1000ms*/, Timer1_Callback);
    Timer_SetInterrupt(TIM2, 100000 /*@100ms*/ , Timer2_Callback);
    TIM_Cmd(TIM1, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}

void loop()
{
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
