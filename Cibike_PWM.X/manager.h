/* 
 * File:   manager.h
 * Author: orras
 *
 * Created on April 17, 2023, 9:02 PM
 */

#ifndef MANAGER_H
#define	MANAGER_H

#include "config.h"
#include "ui.h"
#include "M24256E.h"
#include "PCA9685.h"
#include "peripherals.h"
#include "timer.h"

#ifdef	__cplusplus
extern "C" {
#endif
    typedef enum { UNKNOWN_MODE, DUTY_MODE, FREQ_MODE } ManagerMode;
    
    void Manager_Init(void);
    void Manager_Loop(void);
    void Manager_DutyMode(void);
    void Manager_FreqMode(void);
    void Manager_UpdateDisplay(void);
    void Manager_ApplyPWMValues(void);
    uint8_t Manager_GetNiceChNum(uint8_t chNum);
    void Manager_ApplyStartValues(void);



#ifdef	__cplusplus
}
#endif

#endif	/* MANAGER_H */

