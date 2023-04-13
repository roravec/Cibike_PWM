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


#define HW_ROTARY_PLUS_TRIS     TRISBbits.TRISB6  
#define HW_ROTARY_PLUS_PORT     PORTBbits.RB6
#define HW_ROTARY_PLUS_LAT      LATBbits.LATB6
#define HW_ROTARY_MINUS_TRIS    TRISBbits.TRISB7 
#define HW_ROTARY_MINUS_PORT    PORTBbits.RB7
#define HW_ROTARY_MINUS_LAT     LATBbits.LATB7

#define HW_LOCK_TRIS            TRISBbits.TRISB5 
#define HW_LOCK_PORT            PORTBbits.RB5
#define HW_LOCK_LAT             LATBbits.LATB5
#define HW_RESET_TRIS           TRISBbits.TRISB11
#define HW_RESET_PORT           PORTBbits.RB11
#define HW_RESET_LAT            LATBbits.LATB11

#define HW_FREQ_DUTY_TRIS       TRISBbits.TRISB9
#define HW_FREQ_DUTY_PORT       PORTBbits.RB9
#define HW_FREQ_DUTY_LAT        LATBbits.LATB9

#define HW_MEMORY_ENABLE_TRIS   TRISCbits.TRISC13
#define HW_MEMORY_ENABLE_PORT   PORTCbits.RC13
#define HW_MEMORY_ENABLE_LAT    LATCbits.LATC13
#define HW_PWMCON_ENABLE_TRIS   TRISDbits.TRISD0
#define HW_PWMCON_ENABLE_PORT   PORTDbits.RD0
#define HW_PWMCON_ENABLE_LAT    LATDbits.LATD0

#define HW_TI_OUT_TRIS          TRISDbits.TRISD1
#define HW_TI_OUT_PORT          PORTDbits.RD1
#define HW_TI_OUT_LAT           LATDbits.LATD1