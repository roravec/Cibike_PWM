/* 
 * File:   timer.h
 * Author: skorra
 *
 * Created on April 13, 2023, 10:42 AM
 */

#ifndef TIMER_H
#define	TIMER_H

#include <xc.h>
#include <sys/attribs.h>
#include <stdbool.h>
#include "config.h"

#define TIMER_PRESCALE      256
#define TIMERS_COUNT        10

#ifdef	__cplusplus
extern "C" {
#endif
    
    typedef struct
    {
        uint32_t    startTime;
        uint32_t    overflow0;
        uint16_t    overflowsRequired;
        uint32_t    duration;
        uint32_t    endTime;
        void        (*CallbackFunc)(void);
    } Timer;
    
    typedef enum { TIMER_UNKNOWN, TIMER_RUNNING, TIMER_EXPIRED } TimerStatus;

void Timer_Init(void);
Timer * Timer_Set(Timer *  timer, uint32_t time);
TimerStatus Timer_GetStatus(Timer *  timer);

#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */