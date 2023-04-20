#include "manager.h"

Timer displayRefreshTimer;
uint8_t actualChannel = 6;
uint8_t actualChannelDuties[ROTARY_CHANNELS];
uint16_t actualFreq = 100;
uint16_t lastUpdatedFreq = 0;
ManagerMode currentMode = DUTY_MODE;
bool displayUpdated = 0;

uint16_t displayTimeout = 500;

void Manager_Init(void)
{
    Delay_ms(100);
    Timer_Init();
    Peripherals_Init();
    UI_Init();
    Delay_ms(100);
    UI_WriteLine(LINE_1, "Cibike PWM");
    UI_WriteLine(LINE_2, "Initializing...");
    //Timer_Set(&displayRefreshTimer, 10000);
    UI_WriteLine(LINE_2, "PWM manager...");
    PCA9685_Init();
    Delay_ms(100);
    UI_WriteLine(LINE_2, "Memory...");
    M24256E_Init();
    Delay_ms(100);
    UI_WriteLine(LINE_2, "Reading mem...");
    Manager_ApplyStartValues();
    Peripherals_SetPwmGenEnable(0);
    UI_WriteLine(LINE_2, "Initialized.");
    Delay_ms(500);
}

static uint32_t loopCounter = 0;
uint8_t previousMode = 99;
void Manager_Loop(void)
{
    loopCounter++;
    uint8_t rotPos = Peripherals_GetRotaryPosition();
    if (rotPos > ROTARY_CHANNELS) return;
    if (rotPos != Per_GetActualChannel())
    {
        Per_SetActualChannel(rotPos);
        actualChannel = rotPos;
        Manager_UpdateDisplay();
    }
    if (previousMode != Peripherals_GetFreqDuty()) 
    {
        previousMode = Peripherals_GetFreqDuty();
        if (Peripherals_GetFreqDuty())
            currentMode = DUTY_MODE;
        else
            currentMode = FREQ_MODE;
        Manager_UpdateDisplay();
    }

    switch (currentMode)
    {
        case UNKNOWN_MODE: break;
        case DUTY_MODE: 
            Manager_DutyMode();
            break;
        case FREQ_MODE: 
            Manager_FreqMode();
            break;
    }

    if (Timer_GetStatus(&displayRefreshTimer) != TIMER_RUNNING && !displayUpdated)
    {
        Manager_UpdateDisplay();
        displayUpdated = 1;
        Manager_ApplyPWMValues();
    }
}

void Manager_DutyMode(void)
{
    if (Peripherals_GetLock()) return; // locked
    RotaryMove rotMove = Peripherals_CheckRotMove();
    if (rotMove == CW_MOVEMENT)
    {
        if (actualChannelDuties[actualChannel] < 100)
        {
            actualChannelDuties[actualChannel]++;
            Timer_Set(&displayRefreshTimer, displayTimeout);
            displayUpdated = 0;
        }
        //Manager_UpdateDisplay();
    }
    if (rotMove == CCW_MOVEMENT)
    {
        if (actualChannelDuties[actualChannel] > 0)
        {
            Timer_Set(&displayRefreshTimer, displayTimeout);
            actualChannelDuties[actualChannel]--;
            displayUpdated = 0;
        }
        //Manager_UpdateDisplay();
    }
    //PCA9685_SetChannelDuty(actualChannel, actualChannelDuties[actualChannel]);
}
void Manager_FreqMode(void)
{
    if (Peripherals_GetLock()) return; // locked
    RotaryMove rotMove = Peripherals_CheckRotMove();
    if (rotMove == CW_MOVEMENT)
    {
        if (actualFreq < PCA9685_MAX_FREQ)
        {
            actualFreq++;
            Timer_Set(&displayRefreshTimer, displayTimeout);
            displayUpdated = 0;
        }
        //Manager_UpdateDisplay();
    }
    if (rotMove == CCW_MOVEMENT)
    {
        if (actualFreq > PCA9685_MIN_FREQ)
        {
            actualFreq--;
            Timer_Set(&displayRefreshTimer, displayTimeout);
            displayUpdated = 0;
        }
        //Manager_UpdateDisplay();
    }
}

void Manager_UpdateDisplay()
{
    switch (currentMode)
    {
        case UNKNOWN_MODE: break;
        case DUTY_MODE: 
            UI_PrintStatus(1, actualFreq, Manager_GetNiceChNum(actualChannel), actualChannelDuties[actualChannel]);
            //UI_PrintChannelDuty(Manager_GetNiceChNum(actualChannel), actualChannelDuties[actualChannel]);
            break;
        case FREQ_MODE: 
            UI_PrintStatus(0, actualFreq, Manager_GetNiceChNum(actualChannel), actualChannelDuties[actualChannel]);
            //UI_PrintFreq(100);
            break;
    }
}

void Manager_ApplyPWMValues()
{
    if (lastUpdatedFreq != actualFreq) // set only if new frequency is different than actual
    {
        lastUpdatedFreq = actualFreq;
        PCA9685_SetFreq(actualFreq);
    }
    PCA9685_SetChannelDuty(PCA9685_GetChannelByNum(Manager_GetNiceChNum(actualChannel)), 
            actualChannelDuties[actualChannel]);
}
// channels order: 9 8 10 11 3 2 0 1 5 4 6 7 15 14 12 13
uint8_t Manager_GetNiceChNum(uint8_t chNum)
{
    switch (chNum)
    {
        case 9:     return 0;
        case 8:     return 1;
        case 10:    return 2;
        case 11:    return 3;
        case 3:     return 4;
        case 2:     return 5;
        case 0:     return 6;
        case 1:     return 7;
        case 5:     return 8;
        case 4:     return 9;
        case 6:     return 10;
        case 7:     return 11;
        case 15:    return 12;
        case 14:    return 13;
        case 12:    return 14;
        case 13:    return 15;
        default:    return 0;
    }
}

void Manager_ApplyStartValues(void)
{
    actualFreq = 100;
    for (uint8_t i=0;i<ROTARY_CHANNELS;i++)
        actualChannelDuties[i] = 50;
    Manager_ApplyPWMValues();
}