/* File:   config.h
 * Author: Rastislav Oravec
 * Created on Jan 26 2022
 * Version: 1.0 (1/31/2022)
 * 
 * Description: General use config file
 */ 

#ifndef _CONFIG_H    /* Guard against multiple inclusion */
#define _CONFIG_H

#include <xc.h>
#include "hwinterface.h"

//#define FCY         30000000UL
//#define PCLK        15000000UL
//#define FCY         48000000UL // CPU frequency
//#define PCLK        48000000UL // peripheral frequency
#define FCY         48000000UL // CPU frequency
#define PCLK        24000000UL // peripheral frequency
#define I2C_SPEED   400000      
#define _DI()		__builtin_disable_interrupts()
#define _EI()		__builtin_enable_interrupts()

#define _SUPPRESS_PLIB_WARNING
#define SYS_FREQ    PCLK

/* Function prototypes */
void Init_Config(void);
void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
void Delay(uint32_t del);
void InitOscillator(void);
void InitInterrupts(void);
void InitGPIOs(void);
/***********************/

#endif /* _CONFIG_H */