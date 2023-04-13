/* 
 * File:   timer.h
 * Author: skorra
 *
 * Created on April 13, 2023, 10:42 AM
 */

#ifndef TIMER_H
#define	TIMER_H

#include <xc.h>
#include <stdbool.h>

#define TIMERS_COUNT        10

#ifdef	__cplusplus
extern "C" {
#endif

void Timer_Init(void);
void Timer_SetWatch(uint8_t timerId, uint16_t time);
bool Timer_GetTimerStatus(uint8_t timerId);

#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */