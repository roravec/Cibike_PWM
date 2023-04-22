#include "timer.h"

static volatile uint32_t overflow0 = 0;

void Timer_Init(void)
{
    T4CONbits.ON = 0;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    TMR4 = 0;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    //PR1 = 0xFFFF;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    T4CONbits.TGATE = 0;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    //T2CONbits.TSYNC = 0;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    //T2CONbits.TSYNC = 0;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    IFS0bits.T4IF = 0; // reset interrupt flag
    IPC4bits.T4IP = 5; // priority 3
    IPC4bits.T4IS = 2; // subpriority 1
    IEC0bits.T4IE = 1; // enable interrupt
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    T4CONbits.TCKPS = 0b111; // 1:1
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    T4CONbits.T32 = 1; // 32bit
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    T4CONbits.ON = 1;
}

Timer * Timer_Set(Timer *  timer, uint32_t time)
{
    // 93750 ticks per second, 24MHz, 256 prescaler
    time = time * ((PCLK/TIMER_PRESCALE) / 1000); // time is in ms
    if (timer == 0) return 0;
    timer->duration = time;
    timer->overflow0 = overflow0;
    timer->startTime = TMR4;
    timer->endTime = TMR4 + time;
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
    if (timer->endTime < TMR4 && timer->overflowsRequired == 0) // timer expired
        return TIMER_EXPIRED;
    else if (timer->endTime < TMR4 && timer->overflowsRequired > 0) // timer expired, needs overflow
    {
        if (overflow0 - timer->overflow0 == timer->overflowsRequired) // overflown
            return TIMER_EXPIRED;
    }
    return TIMER_RUNNING;
}

void __ISR(_TIMER_4_VECTOR,ipl5auto) Timer1Handler(void)
{
    overflow0++;
    IFS0bits.T4IF = 0; // reset interrupt flag
}