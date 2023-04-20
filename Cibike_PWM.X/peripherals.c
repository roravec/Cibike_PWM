#include "peripherals.h"

uint16_t pwmTimerLastVal = 0; // last timerVal of TI toggle
RotaryMove rotaryDetectedMove = NO_MOVEMENT;
bool rotA = 0;
uint8_t rotB = 0;
uint16_t rotConfirmationsCW = 0;
uint16_t rotConfirmationsCCW = 0;
bool movementDetected = 0;


uint8_t actChannel = 0;
uint8_t actChannelDuties[ROTARY_CHANNELS];

void Per_SetActualChannel(uint8_t channel)
{
    actChannel = channel;
}

uint8_t Per_GetActualChannel(void)
{
    return actChannel;
}

uint8_t Per_GetChannelDuty(uint8_t channel)
{
    return actChannelDuties[channel];
}

void        Peripherals_Init(void)
{
    for (uint8_t i=0;i<ROTARY_CHANNELS;i++)
        actChannelDuties[i] = 50;
    rotA = HW_ROTARY_PLUS_PORT;
    rotB = HW_ROTARY_MINUS_PORT;
    
    IPC8bits.CNIP = 5; // IPC6bits.CNIP = 4                 
    IPC8bits.CNIS = 1; // IPC6bits.CNIS = 0
    IFS1bits.CNBIF = 0; // IFS1bits.CNIF = 0               
    IEC1bits.CNBIE = 1; // IEC1bits.CNIE = 1
    CNCONBbits.ON = 1;  // CNCONbits.ON = 1 
}

uint8_t     Peripherals_GetRotaryPosition(void)
{
    uint8_t position = 0;
    position = (HW_POS4_PORT << 3) | (HW_POS3_PORT << 2) | (HW_POS2_PORT << 1) | (HW_POS1_PORT);
    return position;
}

//RotaryMove  Peripherals_CheckRotMove(void)
//{
//    rotA = HW_ROTARY_PLUS_PORT;
//    rotB = HW_ROTARY_MINUS_PORT;
//    if (rotA && !rotB)
//    {
//        rotConfirmationsCCW = 0;
//        rotConfirmationsCW++;
//        movementDetected = 0;
//    }
//    else if (rotB && !rotA) // moving CCW
//    {
//        rotConfirmationsCW = 0;
//        rotConfirmationsCCW++;
//        movementDetected = 0;
//    }
//    else if (rotA && rotB && !movementDetected) // movement ended, both pins should be high
//    {
//        rotConfirmationsCW = 0;
//        rotConfirmationsCCW = 0;
//        if (rotConfirmationsCW >= ROTARY_DIR_CINFIRMATIONS_NEEDED)
//        {
//            rotaryDetectedMove = CW_MOVEMENT;
//        }
//        else if (rotConfirmationsCCW >= ROTARY_DIR_CINFIRMATIONS_NEEDED)
//        {
//            rotaryDetectedMove = CCW_MOVEMENT;
//        }
//        movementDetected = 1;
//        return rotaryDetectedMove;
//    }
//    return NO_MOVEMENT;
//}
//RotaryMove  Peripherals_CheckRotMove(void)
//{
//    if (rotA != HW_ROTARY_PLUS_PORT)
//    {
//        rotConfirmationsCCW++;
//        rotA = HW_ROTARY_PLUS_PORT;
//        if (rotConfirmationsCCW==2)
//        {
//            rotConfirmationsCCW = 0;
//            rotA = HW_ROTARY_PLUS_PORT;
//            return CCW_MOVEMENT;
//        }
//    }
//    else if (rotB != HW_ROTARY_MINUS_PORT)
//    {
//        rotConfirmationsCW++;
//        rotB = HW_ROTARY_MINUS_PORT;
//        if (rotConfirmationsCW==2)
//        {
//            rotConfirmationsCW = 0;
//            rotB = HW_ROTARY_MINUS_PORT;
//            return CW_MOVEMENT;
//        }
//    }
//    return NO_MOVEMENT;
//}

RotaryMove  Peripherals_CheckRotMove(void)
{
    if (rotA != HW_ROTARY_PLUS_PORT && !movementDetected)
    {
        Delay_us(500);
        movementDetected = 1;
        if (HW_ROTARY_MINUS_PORT)
        {
            return CW_MOVEMENT;
        }
        else
        {
            return CCW_MOVEMENT;
        }
    }
    if (rotA == HW_ROTARY_PLUS_PORT)
    {
        movementDetected = 0; // reset
    }
    return NO_MOVEMENT;
}

void        Peripherals_SetMemoryEnable(bool value)
{
    HW_MEMORY_ENABLE_LAT = value;
}

void        Peripherals_SetPwmGenEnable(bool value)
{
    HW_PWMCON_ENABLE_LAT = value;
}

bool        Peripherals_GetReset(void)
{
    return (bool)HW_RESET_PORT;
}

bool        Peripherals_GetLock(void)
{
    return (bool)HW_RESET_PORT;
}

bool        Peripherals_GetFreqDuty(void)
{
    return (bool)HW_FREQ_DUTY_PORT;
}

void        Peripherals_ToggleTI(uint16_t timerVal)
{
    HW_TI_OUT_LAT = ~HW_TI_OUT_PORT;
}

void __ISR(33,ipl5auto) CNB_Intrr(void)
{
    if (!movementDetected && !HW_ROTARY_PLUS_PORT)
    {
        movementDetected = 1;
        if (HW_ROTARY_MINUS_PORT)
        {
            if (actChannelDuties[actChannel] > 0)
                actChannelDuties[actChannel]--;
        }
        else
        {
            if (actChannelDuties[actChannel] < 100)
                actChannelDuties[actChannel]++;
        }
    }
    if (movementDetected && HW_ROTARY_PLUS_PORT) // reset position
    {
        movementDetected = 0;
    }
    IFS1bits.CNBIF = 0;
}