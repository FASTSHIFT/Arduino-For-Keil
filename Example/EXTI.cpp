#include "Arduino.h"

#define LED_Pin PA0
#define KEY_Pin PA1

void LED_Toogle()
{
    Serial.println("KEY is pressed!");
    togglePin(LED_Pin);
}

void setup()
{
    Serial.begin(115200);
    pinMode(LED_Pin, OUTPUT);
    pinMode(KEY_Pin, INPUT_PULLUP);
    attachInterrupt(KEY_Pin, LED_Toogle, FALLING);
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
