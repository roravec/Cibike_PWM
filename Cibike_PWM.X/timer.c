#include "timer.h"

uint16_t timerWatchers[TIMERS_COUNT];

void Timer_Init(void)
{
    T1CONbits.ON = 0;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    T1CONbits.TCKPS = 0; // 1:1
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    T1CONbits.ON = 1;
}

void Timer_SetWatch(uint8_t timerId, uint16_t time)
{
    
}
bool Timer_GetTimerStatus(uint8_t timerId)
{
    
}