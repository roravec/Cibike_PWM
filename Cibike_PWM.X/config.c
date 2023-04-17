/* File:   config.c
 * Author: Rastislav Oravec
 * Created on Jan 26 2022
 * Version: 1.0 (1/31/2022)
 * 
 * Description: General use config file
 */ 
#include "config.h"

void Init_Config(void)
{
    InitOscillator();
    InitInterrupts();
    HW_Init();
}

uint32_t __coretimer = 0; // used to calculate core ticks
void Delay_us(uint32_t us)
{
    // Convert microseconds us into how many clock ticks it will take
    us *= FCY / 1000000 / 2; // Core Timer updates every 2 ticks
    _CP0_SET_COUNT(0); // Set Core Timer count to 0
    __coretimer = _CP0_GET_COUNT();

    while (us > _CP0_GET_COUNT()); // Wait until Core Timer count reaches the number we calculated earlier
}
void Delay_ms(uint32_t ms)
{
    Delay_us(ms * 1000);
}
void Delay_ticks(uint32_t del)
{
    for(uint32_t time = 0; time < del; time++){
        asm("nop");
    }
}

void InitOscillator(void)
{
    //PLL output divider set to 1
    OSCCONbits.PLLODIV2 = 0;
    OSCCONbits.PLLODIV1 = 0;
    OSCCONbits.PLLODIV0 = 0;
    
    /* From documentation:
        111 = PLL output divided by 256
        110 = PLL output divided by 64
        101 = PLL output divided by 32
        100 = PLL output divided by 16
        011 = PLL output divided by 8
        010 = PLL output divided by 4
        001 = PLL output divided by 2
        000 = PLL output divided by 1
     */
}

void InitInterrupts(void)
{
    INTCONbits.MVEC = 1;
    asm volatile("ei");
    __builtin_enable_interrupts();
}

void InitGPIOs()
{
}