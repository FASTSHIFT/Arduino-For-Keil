#include "FileGroup.h"

void setup() {
    // put your setup code here, to run once:
}

void loop() {
    // put your main code here, to run repeatedly:
}

/**
  * @brief  Main Function
  * @param  None
  * @retval None
  */
int main(void)
{
//  InternalClocks_Init();
    Delay_Init();
    ADCx_Init(ADC1);
    setup();
    for(;;)loop();
}
