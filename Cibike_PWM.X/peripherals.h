/* 
 * File:   peripherals.h
 * Author: skorra
 *
 * Created on April 13, 2023, 1:06 PM
 */

#ifndef PERIPHERALS_H
#define	PERIPHERALS_H

#include <sys/attribs.h>
#include "timer.h"
#include "pwm.h"

#define ROTARY_DIR_CINFIRMATIONS_NEEDED     1
#define ROTARY_CHANNELS                     16

#include "config.h"
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
    void        Peripherals_StartTI(uint16_t timerVal);
    void        Peripherals_ToggleTI(uint16_t timerVal);
    
    void Per_SetActualChannel(uint8_t channel);
    uint8_t Per_GetActualChannel(void);
    uint8_t Per_GetChannelDuty(uint8_t channel);

#ifdef	__cplusplus
}
#endif

#endif	/* PERIPHERALS_H */

