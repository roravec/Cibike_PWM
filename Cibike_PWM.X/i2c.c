/* File:   i2c.c
 * Author: Rastislav Oravec
 * Created on Jan 26 2022
 * Version: 1.0 (1/31/2022)
 * 
 * Description: PIC32 I2C communication library
 */ 
#include "i2c.h"

// I2C_init() initialises I2C1 at at frequency of [frequency]Hz  
void I2C_Init(uint32_t sys_frequency, uint32_t frequency)
{
    double BRG;
    I2C1CON = 0;            // Turn off I2C2 module
    I2C1CONbits.DISSLW = 1; // Disable slew rate for 100kHz
//    TRISAbits.TRISA2 = 0;
//    TRISAbits.TRISA3 = 0;
//    PORTAbits.RA2 = 1;      // SCL
//    PORTAbits.RA3 = 1;      // SDA

    BRG = (1 / (2 * frequency)) - 0.000000104;
    BRG *= (sys_frequency / 2) - 2;    
    //BRG = 0x00F6;

    I2C1BRG = (uint32_t)BRG;     // Set baud rate
//    I2C2BRG = 0x95;     // Set baud rate
    I2C1CONbits.ON = 1;     // Turn on I2C1 module
}
// I2C_restart() sends a repeated start/restart condition
// currently not in use
void I2C_PinRestart()
{
    I2C_WaitForIdle();
    I2C1CONbits.RSEN = 1;
    if (I2C1STATbits.I2COV) 
    { 
        I2C1STATbits.I2COV  = 0; 
        I2C1STATbits.RBF    = 0; 
        I2C1CONbits.SCLREL  = 1; 
    }
    I2C1CONbits.ON = 1;     // Turn on I2C1 module
    while (I2C1CONbits.RSEN == 1);
}
// I2C_wait_for_idle() waits until the I2C peripheral is no longer doing anything  
void I2C_WaitForIdle(void)
{ 
    while((I2C1CON & 0x1F) /*|| (I2C2STAT & 0x04)*/); // Acknowledge sequence not in progress
                                // Receive sequence not in progress
                                // Stop condition not in progress
                                // Repeated Start condition not in progress
                                // Start condition not in progress
    while(I2C1STATbits.TRSTAT); // Bit = 0 ? Master transmit is not in progress
}

// I2C_start() sends a start condition  
void I2C_Start(uint8_t addr)
{
    I2C_WaitForIdle();
    I2C1CONbits.SEN = 1;
    while (I2C1CONbits.SEN == 1);
    if (addr != 0)
        I2C_Write(addr, 0);
}

// I2C_stop() sends a stop condition  
void I2C_Stop()
{
    I2C_WaitForIdle();
    I2C1CONbits.PEN = 1;
}

// I2C_restart() sends a repeated start/restart condition
void I2C_Restart(uint8_t addr)
{
    I2C_WaitForIdle();
    I2C2CONbits.RSEN = 1;
    while (I2C1CONbits.RSEN == 1);
    if (addr != 0)
        I2C_Write(addr, 0);
}

// I2C_ack() sends an ACK condition
void I2C_Ack(void)
{
    I2C_WaitForIdle();
    I2C1CONbits.ACKDT = 0; // Set hardware to send ACK bit
    I2C1CONbits.ACKEN = 1; // Send ACK bit, will be automatically cleared by hardware when sent  
    while(I2C1CONbits.ACKEN); // Wait until ACKEN bit is cleared, meaning ACK bit has been sent
}

// I2C_nack() sends a NACK condition
void I2C_Nack(void) // Acknowledge Data bit
{
    I2C_WaitForIdle();
    I2C1CONbits.ACKDT = 1; // Set hardware to send NACK bit
    I2C1CONbits.ACKEN = 1; // Send NACK bit, will be automatically cleared by hardware when sent  
    while(I2C1CONbits.ACKEN); // Wait until ACKEN bit is cleared, meaning NACK bit has been sent
}

// set wait_ack to 1 to wait for ACK bit or anything else to skip ACK checking 
void I2C_Wait(uint8_t wait_ack)
{
    if (wait_ack) while (I2C1STATbits.ACKSTAT == 1);
}

// address is I2C slave address, set wait_ack to 1 to wait for ACK bit or anything else to skip ACK checking  
void I2C_Write(uint8_t address, uint8_t wait_ack)
{
    I2C_WaitForIdle();                  // Wait until I2C bus is idle
    I2C1TRN = address/* | 0*/;              // Send slave address with Read/Write bit cleared
    while (I2C1STATbits.TBF == 1);      // Wait until transmit buffer is empty
    //I2C_WaitForIdle();                // Wait until I2C bus is idle
    I2C_Wait(wait_ack); // Wait until ACK is received
    //I2C_Wait();
}

// value is the value of the data we want to send, set ack_nack to 0 to send an ACK or anything else to send a NACK  
void I2C_ReadRef(uint8_t *value, uint8_t ack_nack)
{
    I2C1CONbits.RCEN = 1;               // Receive enable
    while (I2C1CONbits.RCEN);           // Wait until RCEN is cleared (automatic)  
    while (!I2C1STATbits.RBF);          // Wait until Receive Buffer is Full (RBF flag)  
    *value = I2C1RCV;                   // Retrieve value from I2C1RCV

    if (!ack_nack)                      // Do we need to send an ACK or a NACK?  
        I2C_Ack();                      // Send ACK  
    else
        I2C_Nack();                     // Send NACK  
}

// set ack_nack to 0 to send an ACK or anything else to send a NACK  
uint8_t I2C_Read(uint8_t ack_nack)
{
    uint8_t data;
    I2C_WaitForIdle();
    I2C1CONbits.RCEN = 1;               // Receive enable
    while (I2C1CONbits.RCEN);           // Wait until RCEN is cleared (automatic)  
    while (!I2C1STATbits.RBF);           // Wait until Receive Buffer is Full (RBF flag)       
    data = I2C1RCV;                     // Retrieve value
    I2C_WaitForIdle();
    if (!ack_nack)                      // Do we need to send an ACK or a NACK?  
        I2C_Ack();                      // Send ACK  
    else
        I2C_Nack();   
    return data;
}