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

#define MANAGER_DEFAULT_DUTY            50
#define MANAGER_DEFAULT_FREQ            100

#define MANAGER_USED_MEMORY_BYTES       19
#define MANAGER_MEMORY_START_ADDR       0x00
#define MANAGER_MEM_CHANNEL_OFFSET      3  // MEM_DUTY_CH0 index

#ifdef	__cplusplus
extern "C" {
#endif
    typedef enum { UNKNOWN_MODE, DUTY_MODE, FREQ_MODE } ManagerMode;
    typedef enum { MEM_NO_WRITE_YET=0, 
            MEM_FREQ_0, 
            MEM_FREQ_1, 
            MEM_DUTY_CH0,
            MEM_DUTY_CH1,
            MEM_DUTY_CH2,
            MEM_DUTY_CH3,
            MEM_DUTY_CH4,
            MEM_DUTY_CH5,
            MEM_DUTY_CH6,
            MEM_DUTY_CH7,
            MEM_DUTY_CH8,
            MEM_DUTY_CH9,
            MEM_DUTY_CH10,
            MEM_DUTY_CH11,
            MEM_DUTY_CH12,
            MEM_DUTY_CH13,
            MEM_DUTY_CH14,
            MEM_DUTY_CH15
    } MemoryDataID;
    
    void Manager_Init(void);
    void Manager_Loop(void);
    void Manager_CheckMode(void);
    void Manager_DutyMode(void);
    void Manager_FreqMode(void);
    void Manager_UpdateDisplay(bool fullUpdate);
    void Manager_ApplyPWMValues(void);
    uint8_t Manager_GetNiceChNum(uint8_t chNum);
    void Manager_InitValues(void);
    
    void Manager_MemoryReadAll(void);
    uint8_t Manager_MemoryReadByte(MemoryDataID dataID);
    void Manager_MemorySetAll(void);
    void Manager_MemorySetByte(MemoryDataID dataID, uint8_t data);

    void Manager_SetChDuty(uint8_t channel, uint8_t duty);
    void Manager_SetFreq(uint16_t freq);
    void Manager_SetMode(ManagerMode mode);
    uint8_t Manager_GetChDuty(uint8_t channel);
    uint16_t Manager_GetFreq(void);
    ManagerMode Manager_GetMode(void);
    uint8_t Manager_GetCurrentChannel(void);
    void Manager_SetCurrentChannel(uint8_t channel);
    void Manager_Reset(void);
    void Manager_SetDefaultValues(void);
    void Manager_ScheduleDataUpdate(void);
    
    void Manager_StartTI(void);
    void Manager_StopTI(void);


#ifdef	__cplusplus
}
#endif

#endif	/* MANAGER_H */

