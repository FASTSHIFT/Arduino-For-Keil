#ifndef __FILEGROUP_H
#define	__FILEGROUP_H

#include "stm32f10x.h"
#include "Arduino.h"
#include "math.h"
#include "delay.h"
#include "GPIO.h"
#include "ADC.h"
#include "usart.h"
#include "exit.h"
#include "time_exit.h"

//********ADC Channels(GPIOA)********//
#define JSL_X 0
#define JSL_Y 1
#define JSR_X 2
#define JSR_Y 3
#define ADL 4
#define ADR 5
#define KEYS 6
#define SW1 7

//*************GPIO**************//

//LED
#define DS1 PE5
#define DS2 PE6

//*************STATE**************//
#define On 1
#define Off 0

#define UART 0
#define nRF 1

#define CAR 0
#define TANK 1
#define CPTR 2
#define SPIDER 3

#define INF 1
#define OPT 2

//***************************Functoins*******************************//


#endif
