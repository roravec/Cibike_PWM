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

#define TIMERS_COUNT        10

#ifdef	__cplusplus
extern "C" {
#endif
    
    typedef struct
    {
        uint16_t    startTime;
        uint32_t    overflow0;
        uint8_t     overflowsRequired;
        uint16_t    duration;
        uint16_t    endTime;
    } Timer;
    
    typedef enum { TIMER_UNKNOWN, TIMER_RUNNING, TIMER_EXPIRED } TimerStatus;

void Timer_Init(void);
Timer * Timer_Set(Timer *  timer, uint16_t time);
TimerStatus Timer_GetStatus(Timer *  timer);

#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */