#include "peripherals.h"

uint16_t pwmTimerLastVal = 0; // last timerVal of TI toggle
RotaryMove rotaryDetectedMove = NO_MOVEMENT;
bool rotA = 0;
bool rotB = 0;
uint16_t rotConfirmationsCW = 0;
uint16_t rotConfirmationsCCW = 0;
bool movementDetected = 0;

void        Peripherals_Init(void)
{
    
}

uint8_t     Peripherals_GetRotaryPosition(void)
{
    uint8_t position = 0;
    position = (HW_POS4_PORT << 3) | (HW_POS3_PORT << 2) | (HW_POS2_PORT << 1) | (HW_POS1_PORT);
    return position;
}

RotaryMove  Peripherals_CheckRotMove(void)
{
    rotA = HW_ROTARY_PLUS_PORT;
    rotB = HW_ROTARY_MINUS_PORT;
    if (rotA && !rotB)
    {
        rotConfirmationsCCW = 0;
        rotConfirmationsCW++;
        movementDetected = 0;
    }
    else if (rotB && !rotA) // moving CCW
    {
        rotConfirmationsCW = 0;
        rotConfirmationsCCW++;
        movementDetected = 0;
    }
    else if (rotA && rotB && !movementDetected) // movement ended, both pins should be high
    {
        rotConfirmationsCW = 0;
        rotConfirmationsCCW = 0;
        if (rotConfirmationsCW >= ROTARY_DIR_CINFIRMATIONS_NEEDED)
        {
            rotaryDetectedMove = CW_MOVEMENT;
        }
        else if (rotConfirmationsCCW >= ROTARY_DIR_CINFIRMATIONS_NEEDED)
        {
            rotaryDetectedMove = CCW_MOVEMENT;
        }
        movementDetected = 1;
        return rotaryDetectedMove;
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
    return (bool)HW_RESET_PORT;
}

void        Peripherals_ToggleTI(uint16_t timerVal)
{
    HW_TI_OUT_LAT = ~HW_TI_OUT_PORT;
}