/* 
 * File:   pwm.h
 * Author: orras
 *
 * Created on April 22, 2023, 11:15 PM
 */

#ifndef PWM_H
#define	PWM_H

#include "config.h"

#define PWM_CLOCK_FREQ      8000000UL

#define PWM_STATIC_FREQ     100


#ifdef	__cplusplus
extern "C" {
#endif

void PWM_Init(void);
void PWM_Set(float pwmFreq, uint8_t dutyPercent);
void PWM_SetDuty(uint8_t dutyPercent);
void PWM_SetDirect(uint32_t period, uint32_t duty);


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

