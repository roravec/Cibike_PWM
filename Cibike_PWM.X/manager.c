#include "manager.h"

Timer displayTimer;

void Manager_Init(void)
{
    Timer_Init();
    UI_Init();
    UI_WriteLine(LINE_1, "Cibike PWM");
    UI_WriteLine(LINE_2, "Initializing...");
    Timer_Set(&displayTimer, 65000);
    PCA9685_Init();
    M24256E_Init();
    //UI_WriteLine(LINE_2, "Initialized");
}

void Manager_Loop(void)
{
    if (Timer_GetStatus(&displayTimer) == TIMER_EXPIRED)
    {
        UI_WriteLine(LINE_2, "Timer expired");
    }
}
