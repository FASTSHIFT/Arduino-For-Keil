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
    GPIO_JTAG_Disable();
    Delay_Init();
//  ADCx_Init(ADC1);
//  ADCx_DMA_Init();
    setup();
    for(;;)loop();
}
