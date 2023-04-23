/* 
 * File:   M24256E.h
 * Author: orras
 *
 * Created on April 17, 2023, 8:56 PM
 */

#ifndef M24256E_H
#define	M24256E_H

#include "i2c.h"
#include "peripherals.h"

#define M24256E_ADDRESS_W   0xA0
#define M24256E_ADDRESS_R   0xA1
#define M24256E_PAGE_SIZE   64


#ifdef	__cplusplus
extern "C" {
#endif

void M24256E_Init(void);
uint8_t M24256E_ReadByte(uint16_t address);
void M24256E_ReadPage(uint16_t address, uint8_t * data, uint8_t len);
void M24256E_WriteByte(uint16_t address, uint8_t data);
void M24256E_WritePage(uint16_t address, uint8_t * data, uint8_t len);

void M24256E_WLock(void);
void M24256E_WUnlock(void);

#ifdef	__cplusplus
}
#endif

#endif	/* M24256E_H */

