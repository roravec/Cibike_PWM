/* 
 * File:   peripherals.h
 * Author: skorra
 *
 * Created on April 13, 2023, 1:06 PM
 */

#ifndef PERIPHERALS_H
#define	PERIPHERALS_H

#define ROTARY_DIR_CINFIRMATIONS_NEEDED     3

#include "hwinterface.h"
#include <stdbool.h>

typedef enum {NO_MOVEMENT, CW_MOVEMENT, CCW_MOVEMENT, UNSTABLE_MOVEMENT} RotaryMove; // used fort rotary switch/encoder

#ifdef	__cplusplus
extern "C" {
#endif

    void        Peripherals_Init(void);
    uint8_t     Peripherals_GetRotaryPosition(void);
    RotaryMove  Peripherals_CheckRotMove(void);
    void        Peripherals_SetMemoryEnable(bool);
    void        Peripherals_SetPwmGenEnable(bool);
    bool        Peripherals_GetReset(void);
    bool        Peripherals_GetLock(void);
    bool        Peripherals_GetFreqDuty(void);
    void        Peripherals_ToggleTI(uint16_t timerVal);

#ifdef	__cplusplus
}
#endif

#endif	/* PERIPHERALS_H */

