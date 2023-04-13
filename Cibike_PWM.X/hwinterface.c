#include "hwinterface.h"

void HW_Init(void)
{
    HW_ROTARY_PLUS_TRIS = 1;
    HW_ROTARY_MINUS_TRIS = 1;
    
    HW_POS1_TRIS = 1;
    HW_POS2_TRIS = 1;
    HW_POS3_TRIS = 1;
    HW_POS4_TRIS = 1;
    
    HW_LOCK_TRIS = 1;
    HW_RESET_TRIS = 1;
    HW_FREQ_DUTY_TRIS = 1;
    
    HW_MEMORY_ENABLE_TRIS = 0;
    HW_PWMCON_ENABLE_TRIS = 0;
    
    HW_TI_OUT_TRIS = 0;
}