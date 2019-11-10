#include "FileGroup.h"

void setup()
{
    Serial.begin(115200);
	pinMode(PA0, INPUT_ANALOG_DMA);
    pinMode(PA1, INPUT_ANALOG_DMA);
    pinMode(PA2, INPUT_ANALOG_DMA);
    pinMode(PA3, INPUT_ANALOG_DMA);
    pinMode(PA4, INPUT_ANALOG_DMA);
    pinMode(PA5, INPUT_ANALOG_DMA);
    pinMode(PA6, INPUT_ANALOG_DMA);
    pinMode(PA7, INPUT_ANALOG_DMA);
    
    pinMode(PB0, INPUT_ANALOG_DMA);
    pinMode(PB1, INPUT_ANALOG_DMA);
    
    pinMode(PC0, INPUT_ANALOG_DMA);
    pinMode(PC1, INPUT_ANALOG_DMA);
    pinMode(PC2, INPUT_ANALOG_DMA);
    pinMode(PC3, INPUT_ANALOG_DMA);
    pinMode(PC4, INPUT_ANALOG_DMA);
    pinMode(PC5, INPUT_ANALOG_DMA);
    ADC_DMA_Init();
}

void loop()
{
    Serial.printf(
        "PA0=%d,PA1=%d,PA2=%d,PA3=%d,PA4=%d,PA5=%d,PA6=%d,PA7=%d ",
        analogRead_DMA(PA0),
        analogRead_DMA(PA1),
        analogRead_DMA(PA2),
        analogRead_DMA(PA3),
        analogRead_DMA(PA4),
        analogRead_DMA(PA5),
        analogRead_DMA(PA6),
        analogRead_DMA(PA7)
    );
    
    Serial.printf(
        "-- PB0=%d,PB1=%d ",
        analogRead_DMA(PB0),
        analogRead_DMA(PB1)
    );
    
    Serial.printf(
        "-- PC0=%d,PC1=%d,PC2=%d,PC3=%d,PC4=%d,PC5=%d\r\n",
        analogRead_DMA(PC0),
        analogRead_DMA(PC1),
        analogRead_DMA(PC2),
        analogRead_DMA(PC3),
        analogRead_DMA(PC4),
        analogRead_DMA(PC5)
    );
}

/**
  * @brief  Main Function
  * @param  None
  * @retval None
  */
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    GPIO_JTAG_Disable();
//  SysClock_Init(F_CPU_128MHz);
    Delay_Init();
//  ADCx_Init(ADC1);
    setup();
    for(;;)loop();
}
