#ifndef __SERVO_H__
#define __SERVO_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "stm32f10x.h"
#include "stdint.h"

#define minPW            544
#define maxPW            2400
#define minAngle         0
#define maxAngle         180

void Servo_attach(void);
void Servo_write(int degrees);

#ifdef __cplusplus
}
#endif	

#endif  /* _SERVO_H_ */
