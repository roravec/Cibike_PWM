#include "manager.h"

Timer displayRefreshTimer;
uint8_t currentChannel = 6;
ManagerMode currentMode = DUTY_MODE;
bool displayUpdated = 0;
bool resetApplied = 0;

static uint8_t memoryData[MANAGER_USED_MEMORY_BYTES];
uint16_t displayTimeout = 1000;

void Manager_Init(void)
{
    Delay_ms(100);
    Timer_Init();
    Peripherals_Init();
    UI_Init();
    Delay_ms(100);
    UI_WriteLine(LINE_1, "Cibike PWM");
    UI_WriteLine(LINE_2, "Initializing...");
    UI_WriteLine(LINE_2, "TI function...");
    Manager_StartTI();
    Delay_ms(100);
    UI_WriteLine(LINE_2, "PWM manager...");
    PCA9685_Init();
    Delay_ms(100);
    UI_WriteLine(LINE_2, "Memory...");
    M24256E_Init();
    Delay_ms(100);
    UI_WriteLine(LINE_2, "Reading mem...");
    Manager_InitValues();
    Peripherals_SetPwmGenEnable(0);
    UI_WriteLine(LINE_1, "Cibike PWM");
    UI_WriteLine(LINE_2, "Initialized.");
    Delay_ms(500);
    Manager_UpdateDisplay(1);
}

static uint32_t loopCounter = 0;
void Manager_Loop(void)
{
    loopCounter++;
    Manager_CheckMode();

    if (Timer_GetStatus(&displayRefreshTimer) != TIMER_RUNNING && !displayUpdated)
    {
        Manager_UpdateDisplay(0);
        displayUpdated = 1;
        Manager_ApplyPWMValues();
    }
}

uint8_t previousMode = 99;
void Manager_CheckMode(void)
{
    uint8_t rotPos = Peripherals_GetRotaryPosition();
    if (rotPos > ROTARY_CHANNELS) return;
    if (rotPos != Per_GetActualChannel())
    {
        Per_SetActualChannel(rotPos);
        Manager_SetCurrentChannel(rotPos);
        Manager_UpdateDisplay(0);
    }
    if (previousMode != Peripherals_GetFreqDuty()) 
    {
        previousMode = Peripherals_GetFreqDuty();
        if (Peripherals_GetFreqDuty())
            currentMode = DUTY_MODE;
        else
            currentMode = FREQ_MODE;
        Manager_UpdateDisplay(0);
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
    
    if (Peripherals_GetReset() && !Peripherals_GetLock())
    {
        Manager_Reset();
    }
}

void Manager_DutyMode(void)
{
    if (Peripherals_GetLock()) 
        return; // locked
    RotaryMove rotMove = Peripherals_CheckRotMove();
    if (rotMove == CW_MOVEMENT)
    {
        uint8_t currentChannelDuty = Manager_GetChDuty(Manager_GetCurrentChannel());
        if (currentChannelDuty < 100)
        {
            Manager_SetChDuty(Manager_GetCurrentChannel(), ++currentChannelDuty);
            Manager_ScheduleDataUpdate();
            UI_PrintChannelDuty(Manager_GetChDuty(Manager_GetCurrentChannel()));
        }
    }
    if (rotMove == CCW_MOVEMENT)
    {
        uint8_t currentChannelDuty = Manager_GetChDuty(Manager_GetCurrentChannel());
        if (currentChannelDuty > 0)
        {
            Manager_SetChDuty(Manager_GetCurrentChannel(), --currentChannelDuty);
            Manager_ScheduleDataUpdate();
            UI_PrintChannelDuty(Manager_GetChDuty(Manager_GetCurrentChannel()));
        }
    }
}
void Manager_FreqMode(void)
{
    if (Peripherals_GetLock()) 
        return; // locked
    RotaryMove rotMove = Peripherals_CheckRotMove();
    if (rotMove == CW_MOVEMENT)
    {
        uint8_t actualFreq = Manager_GetFreq();
        if (actualFreq < PCA9685_MAX_FREQ)
        {
            Manager_SetFreq(++actualFreq);
            Manager_ScheduleDataUpdate();
            UI_PrintFreq(Manager_GetFreq());
        }
    }
    if (rotMove == CCW_MOVEMENT)
    {
        uint8_t actualFreq = Manager_GetFreq();
        if (Manager_GetFreq() > PCA9685_MIN_FREQ)
        {
            Manager_SetFreq(--actualFreq);
            Manager_ScheduleDataUpdate();
            UI_PrintFreq(Manager_GetFreq());
        }
    }
}

void Manager_UpdateDisplay(bool fullUpdate)
{
    uint8_t actualFreq = Manager_GetFreq();
    uint8_t currentChannelDuty = Manager_GetChDuty(Manager_GetCurrentChannel());
    switch (currentMode)
    {
        case UNKNOWN_MODE: break;
        case DUTY_MODE: 
            if (fullUpdate)
                UI_PrintStatus(1, actualFreq, Manager_GetNiceChNum(Manager_GetCurrentChannel()), currentChannelDuty, Peripherals_GetLock());
            else
                UI_UpdateStatus(1, actualFreq, Manager_GetNiceChNum(Manager_GetCurrentChannel()), currentChannelDuty, Peripherals_GetLock());
            break;
        case FREQ_MODE: 
            if (fullUpdate)
                UI_PrintStatus(0, actualFreq, Manager_GetNiceChNum(Manager_GetCurrentChannel()), currentChannelDuty, Peripherals_GetLock());
            else
                UI_UpdateStatus(0, actualFreq, Manager_GetNiceChNum(Manager_GetCurrentChannel()), currentChannelDuty, Peripherals_GetLock());
            break;
    }
}

uint16_t lastUpdatedFreq = 0;
void Manager_ApplyPWMValues()
{
    UI_PrintUpdateStart();
    uint8_t actualFreq = Manager_GetFreq();
    uint8_t currentChannelDuty = Manager_GetChDuty(Manager_GetCurrentChannel());
    if (lastUpdatedFreq != actualFreq) // set only if new frequency is different than actual
    {
        lastUpdatedFreq = actualFreq;
        PCA9685_SetFreq(actualFreq);
    }
    PCA9685_SetChannelDuty(PCA9685_GetChannelByNum(Manager_GetNiceChNum(Manager_GetCurrentChannel())), 
            currentChannelDuty);
    Manager_MemorySetAll();
    UI_PrintUpdateEnd();
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

void Manager_InitValues(void)
{
    Manager_MemoryReadAll();
//    UI_PrintMemData(memoryData, MANAGER_USED_MEMORY_BYTES);
//    Delay_ms(5000);
    if (memoryData[MEM_NO_WRITE_YET] != 1) // no data in memory, set default values
    {
        Manager_SetDefaultValues();
        memoryData[MEM_NO_WRITE_YET] = 1;   // initial data applied
    }
    Manager_ApplyPWMValues();
    //Manager_MemoryReadAll();
    //UI_PrintMemData(memoryData, MANAGER_USED_MEMORY_BYTES);
    //Delay_ms(5000);
}

void Manager_MemoryReadAll(void)
{
    M24256E_ReadPage(MANAGER_MEMORY_START_ADDR, memoryData, MANAGER_USED_MEMORY_BYTES);
}

uint8_t Manager_MemoryReadByte(MemoryDataID dataID)
{
    return M24256E_ReadByte(MANAGER_MEMORY_START_ADDR+dataID);
}

void Manager_MemorySetAll(void)
{
    M24256E_WritePage(MANAGER_MEMORY_START_ADDR, memoryData, MANAGER_USED_MEMORY_BYTES);
}

void Manager_MemorySetByte(MemoryDataID dataID, uint8_t data)
{
    M24256E_WriteByte(MANAGER_MEMORY_START_ADDR+dataID, data);
}

void Manager_SetChDuty(uint8_t channel, uint8_t duty)
{
    if (channel >= ROTARY_CHANNELS) return; // invalid channel
    memoryData[channel+MEM_DUTY_CH0] = duty;
}
void Manager_SetFreq(uint16_t freq)
{
    memoryData[MEM_FREQ_0] = (uint8_t)freq;
    memoryData[MEM_FREQ_1] = (uint8_t)freq << 8;
}
void Manager_SetMode(ManagerMode mode)
{
    currentMode = mode;
}
uint8_t Manager_GetChDuty(uint8_t channel)
{
    if (channel >= ROTARY_CHANNELS) return 0; // invalid channel
    return memoryData[channel+MEM_DUTY_CH0];
}
uint16_t Manager_GetFreq(void)
{
    return memoryData[MEM_FREQ_0] | (memoryData[MEM_FREQ_1] << 8);
}
ManagerMode Manager_GetMode(void)
{
    return currentMode;
}

uint8_t Manager_GetCurrentChannel(void)
{
    return currentChannel;
}

void Manager_SetCurrentChannel(uint8_t channel)
{
    currentChannel = channel;
}

void Manager_Reset(void)
{
    if (!resetApplied)
    {
        Manager_SetDefaultValues();
        Manager_ScheduleDataUpdate();
    }
}
void Manager_SetDefaultValues(void)
{
    Manager_SetFreq(MANAGER_DEFAULT_FREQ);
    for (uint8_t i=0;i<ROTARY_CHANNELS;i++)
        Manager_SetChDuty(i, MANAGER_DEFAULT_DUTY);
    Manager_ApplyPWMValues();
    UI_WriteLine(LINE_1, "Default values");
    UI_WriteLine(LINE_2, "applied.");
    Delay_ms(1000);
    Manager_UpdateDisplay(1);
}

void Manager_ScheduleDataUpdate(void)
{
    Timer_Set(&displayRefreshTimer, displayTimeout);
    displayUpdated = 0;
    resetApplied = 0;
}

void Manager_StartTI(void)
{
    Peripherals_StartTI(500);
}
void Manager_StopTI(void)
{
    
}