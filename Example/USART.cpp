#include "Arduino.h"

#define LED_Pin PA0

void Serial_EventHandler()
{
    togglePin(LED_Pin);
}

void setup()
{
    pinMode(LED_Pin, OUTPUT);
    Serial.begin(115200);
    Serial.println("Serial printing...");
    Serial.setTimeout(10);
    Serial.attachInterrupt(Serial_EventHandler);
}

void loop()
{
    if(Serial.available())
    {
        String receivedString = Serial.readString();
        Serial.printf("Serial received:%s\r\n", receivedString.c_str());
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
