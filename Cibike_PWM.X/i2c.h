/* File:   i2c.h
 * Author: Rastislav Oravec
 * Created on Jan 26 2022
 * Version: 1.0 (1/31/2022)
 * 
 * Description: PIC32 I2C communication library
 */ 

#ifndef _I2C_H    /* Guard against multiple inclusion */
#define _I2C_H

#include <xc.h>

void I2C_Init(uint32_t sys_frequency, uint32_t frequency);
void I2C_WaitForIdle(void);
void I2C_Start(uint8_t addr, uint8_t wait_ack);
void I2C_Stop();
void I2C_Restart(uint8_t addr, uint8_t wait_ack);
void I2C_PinRestart();
void I2C_Ack(void);
void I2C_Nack(void); // Acknowledge Data bit
void I2C_Wait(uint8_t wait_ack);
// address is I2C slave address, set wait_ack to 1 to wait for ACK bit or anything else to skip ACK checking  
void I2C_Write(uint8_t address, uint8_t wait_ack);
// value is the value of the data we want to send, set ack_nack to 0 to send an ACK or anything else to send a NACK  
void I2C_ReadRef(uint8_t *value, uint8_t ack_nack);
uint8_t I2C_Read(uint8_t ack_nack);

#endif /* _I2C_H */