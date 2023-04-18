#include "timer.h"

static volatile uint32_t overflow0 = 0;

void Timer_Init(void)
{
    T2CONbits.ON = 0;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    TMR2 = 0;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    //PR1 = 0xFFFF;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    T2CONbits.TGATE = 0;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    //T2CONbits.TSYNC = 0;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    //T2CONbits.TSYNC = 0;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    IFS0bits.T2IF = 0; // reset interrupt flag
    IPC2bits.T2IP = 5; // priority 3
    IPC2bits.T2IS = 2; // subpriority 1
    IEC0bits.T2IE = 1; // enable interrupt
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    T2CONbits.TCKPS = 0b111; // 1:1
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    T2CONbits.T32 = 1; // 32bit
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    T2CONbits.ON = 1;
}

Timer * Timer_Set(Timer *  timer, uint32_t time)
{
    // 93750 ticks per second, 24MHz, 256 prescaler
    time = time * ((PCLK/TIMER_PRESCALE) / 1000); // time is in ms
    if (timer == 0) return 0;
    timer->duration = time;
    timer->overflow0 = overflow0;
    timer->startTime = TMR2;
    timer->endTime = TMR2 + time;
    if (timer->startTime > timer->endTime) // overflow
        timer->overflowsRequired = 0xFFFFFFFF / time;
    else
        timer->overflowsRequired = 0;
    return timer;
}
TimerStatus Timer_GetStatus(Timer *  timer)
{
    if (timer == 0) return TIMER_UNKNOWN;
    if (timer->duration == 0) return TIMER_EXPIRED;
    if (timer->endTime < TMR2 && timer->overflowsRequired == 0) // timer expired
        return TIMER_EXPIRED;
    else if (timer->endTime < TMR2 && timer->overflowsRequired > 0) // timer expired, needs overflow
    {
        if (overflow0 - timer->overflow0 == timer->overflowsRequired) // overflown
            return TIMER_EXPIRED;
    }
    return TIMER_RUNNING;
}

void __ISR(_TIMER_2_VECTOR,ipl5auto) Timer1Handler(void)
{
    overflow0++;
    IFS0bits.T2IF = 0; // reset interrupt flag
}