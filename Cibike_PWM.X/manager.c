#include "manager.h"

Timer displayTimer;

void Manager_Init(void)
{
    Delay_ms(100);
    Timer_Init();
    UI_Init();
    Delay_ms(100);
    //UI_WriteLine(LINE_1, "Cibike PWM");
    //UI_WriteLine(LINE_2, "Initializing...");
    //Timer_Set(&displayTimer, 10000);
    PCA9685_Init();
    M24256E_Init();
    //UI_WriteLine(LINE_2, "Initialized");
}

void Manager_Loop(void)
{
//    if (Timer_GetStatus(&displayTimer) == TIMER_EXPIRED)
//    {
//        UI_WriteLine(LINE_2, "Timer expired");
//    }
    
}