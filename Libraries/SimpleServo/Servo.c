#include "Servo.h"

// 20 millisecond period config.  For a 1-based prescaler,
//
//    (prescaler * overflow / CYC_MSEC) msec = 1 timer cycle = 20 msec
// => prescaler * overflow = 20 * CYC_MSEC
//
// This picks the smallest prescaler that allows an overflow < 2^16.
#define Constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define Round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

#define F_CPU 72000000U
#define CYCLES_PER_MICROSECOND  (F_CPU / 1000000U)
#define MAX_OVERFLOW    ((1 << 16) - 1)
#define CYC_MSEC        (1000 * CYCLES_PER_MICROSECOND)
#define TAU_MSEC        20
#define TAU_USEC        (TAU_MSEC * 1000)
#define TAU_CYC         (TAU_MSEC * CYC_MSEC)
#define SERVO_PRESCALER (TAU_CYC / MAX_OVERFLOW + 1)
#define SERVO_OVERFLOW  ((uint16_t)Round((double)TAU_CYC / SERVO_PRESCALER))

// Unit conversions
#define US_TO_COMPARE(us) ((uint16_t)Map((us), 0, TAU_USEC, 0, SERVO_OVERFLOW))
#define COMPARE_TO_US(c)  ((uint32_t)Map((c), 0, SERVO_OVERFLOW, 0, TAU_USEC))
#define ANGLE_TO_US(a)    ((uint16_t)(Map((a), minAngle, maxAngle, minPW, maxPW)))
#define US_TO_ANGLE(us)   ((int16_t)(Map((us), minPW, maxPW, minAngle, maxAngle)))

long Map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
																			 
void TIMER_Init(TIM_TypeDef* TIMx,uint8_t CHx)
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

             if(TIMx == TIM1)RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    else if(TIMx == TIM2)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    else if(TIMx == TIM3)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    else if(TIMx == TIM4)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    //TIM_TimeBaseStructure.TIM_Period = arr;
    //TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    switch(CHx)
    {
        case 1:
            TIM_OC1Init(TIMx, &TIM_OCInitStructure);
            TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
        case 2:
            TIM_OC2Init(TIMx, &TIM_OCInitStructure);
            TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
        case 3:
            TIM_OC3Init(TIMx, &TIM_OCInitStructure);
            TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
        case 4:
            TIM_OC4Init(TIMx, &TIM_OCInitStructure);
            TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);
            break;
    }
    
    TIM_Cmd(TIMx, ENABLE);
    if(TIMx==TIM1 || TIMx==TIM8)TIM_CtrlPWMOutputs(TIMx, ENABLE);
}

void Servo_attach() 
{
    TIMER_Init(TIM3,3);//PB0
    TIM_Cmd(TIM3,DISABLE);
    //timer_set_prescaler(tdev, SERVO_PRESCALER - 1); // prescaler is 1-based
    TIM3->PSC = SERVO_PRESCALER - 1;
    //timer_set_reload(tdev, SERVO_OVERFLOW);
    TIM3->ARR = SERVO_OVERFLOW;
    //timer_generate_update(tdev);
    TIM3->EGR = TIM_PSCReloadMode_Immediate; 
    TIM_Cmd(TIM3,ENABLE);
}

void Servo_writeMicroseconds(uint16_t pulseWidth) 
{
    //pulseWidth = constrain(pulseWidth, this->minPW, this->maxPW);
    TIM3->CCR3=US_TO_COMPARE(pulseWidth);
}

void Servo_write(int degrees)
{
    degrees = Constrain(degrees, minAngle, maxAngle);
    Servo_writeMicroseconds(ANGLE_TO_US(degrees));
}

