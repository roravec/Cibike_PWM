#include "timer.h"

static volatile uint32_t overflow0 = 0;

//void Timer_Init(void)
//{
//    T1CONbits.ON = 0;
//    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
//    TMR1 = 0;
//    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
//    PR1 = 24000;
//    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
//    T1CONbits.TGATE = 0;
//    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
//    T1CONbits.TSYNC = 0;
//    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
//    T1CONbits.TSYNC = 0;
//    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
//    IFS0bits.T1IF = 0; // reset interrupt flag
//    IEC0bits.T1IE = 1; // enable interrupt
//    IPC1bits.IC1IP = 4; // priority 3
//    IPC1bits.IC1IS = 1; // subpriority 1
//    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
//    T1CONbits.TCKPS = 0; // 1:1
//    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
//    T1CONbits.ON = 1;
//}
//
//Timer * Timer_Set(Timer *  timer, uint16_t time)
//{
//    if (timer == 0) return 0;
//    timer->duration = time;
//    timer->overflow0 = overflow0;
//    timer->startTime = TMR1;
//    timer->endTime = TMR1 + time;
//    if (timer->startTime > timer->endTime) // overflow
//        timer->overflowsRequired = 1;
//    else
//        timer->overflowsRequired = 0;
//    return timer;
//}
//TimerStatus Timer_GetStatus(Timer *  timer)
//{
//    if (timer == 0) return TIMER_UNKNOWN;
//    if (timer->duration == 0) return TIMER_EXPIRED;
//    if (timer->endTime < TMR1 && timer->overflowsRequired == 0) // timer expired
//        return TIMER_EXPIRED;
//    else if (timer->endTime < TMR1 && timer->overflowsRequired > 0) // timer expired, needs overflow
//    {
//        if (timer->overflow0 != overflow0) // overflown
//            return TIMER_EXPIRED;
//    }
//    return TIMER_RUNNING;
//}

//void __ISR(_TIMER_1_VECTOR,ipl4auto) Timer1Handler(void)
//{
//    overflow0++;
//    IFS0bits.T1IF = 0; // reset interrupt flag
//}

void Timer_Init(void)
{
    T1CONbits.ON = 0;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    TMR2 = 0;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    PR2 = 24000;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    T2CONbits.TGATE = 0;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    T2CONbits.TCS = 0;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    IFS0bits.T2IF = 0; // reset interrupt flag
    IEC0bits.T2IE = 1; // enable interrupt
    IPC2bits.IC2IP = 4; // priority 3
    IPC2bits.IC2IS = 1; // subpriority 1
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    T2CONbits.TCKPS = 0; // 1:1
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    T2CONbits.ON = 1;
}

Timer * Timer_Set(Timer *  timer, uint16_t time)
{
    if (timer == 0) return 0;
    timer->duration = time;
    timer->overflow0 = overflow0;
    timer->startTime = TMR2;
    timer->endTime = TMR2 + time;
    if (timer->startTime > timer->endTime) // overflow
        timer->overflowsRequired = 1;
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
        if (timer->overflow0 != overflow0) // overflown
            return TIMER_EXPIRED;
    }
    return TIMER_RUNNING;
}

void __ISR(_TIMER_2_VECTOR,ipl4auto) Timer2Handler(void)
{
    overflow0++;
    IFS0bits.T2IF = 0; // reset interrupt flag
}