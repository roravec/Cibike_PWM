#include "M24256E.h"

void M24256E_Init(void)
{
    M24256E_WLock(); // lock memory
}
uint8_t M24256E_ReadByte(uint16_t address)
{
    M24256E_WUnlock();
    I2C_Start(M24256E_ADDRESS_W, 1);
    I2C_Write(address>>8, 1);
    I2C_Write(address, 1);
    I2C_Stop();
    I2C_Start(M24256E_ADDRESS_R, 1);
    //I2C_Restart(M24256E_ADDRESS_R, 1);
    uint8_t data = 0;
    data = I2C_Read(0);
    I2C_Stop();
    M24256E_WLock();
    return data;
}
void M24256E_ReadPage(uint16_t address, uint8_t * data, uint8_t len)
{
    if (data == 0) return; // invalid pointer
    if (len == 0 || len > M24256E_PAGE_SIZE) // array length check
        len = M24256E_PAGE_SIZE;
    M24256E_WUnlock();
    I2C_Start(M24256E_ADDRESS_W, 1);
    I2C_Write(address>>8, 1);
    I2C_Write(address, 1);
    I2C_Stop();
    I2C_Start(M24256E_ADDRESS_R, 1);
//    I2C_Restart(M24256E_ADDRESS_R, 1);
    for (uint8_t i=0; i < len; i++)
    {
        bool ack = 0;
        if (i == (len-1)) // last data byte, no ack
            ack = 0;
        data[i] = I2C_Read(ack);
    }
    I2C_Stop();
    M24256E_WLock();
}
void M24256E_WriteByte(uint16_t address, uint8_t data)
{
    M24256E_WUnlock();
    I2C_Start(M24256E_ADDRESS_W, 1);
    I2C_Write(address>>8, 1);
    I2C_Write(address, 1);
    I2C_Write(data, 1);
    I2C_Stop();
    M24256E_WLock();
}
void M24256E_WritePage(uint16_t address, uint8_t * data, uint8_t len)
{
    M24256E_WUnlock();
    I2C_Start(M24256E_ADDRESS_W, 1);
    I2C_Write(address>>8, 1);
    I2C_Write(address, 1);
    for (uint8_t i=0; i < len; i++)
    {
        bool ack = 0;
        if (i == len-1) // last data byte, no ack
            ack = 0;
        uint8_t dataW = data[i];
        I2C_Write(data[i], ack);
    }
    I2C_Stop();
    Delay_ms(5); // wait for write completition
    M24256E_WLock();
}

void M24256E_WLock(void)
{
    Delay_ms(10);
    //Peripherals_SetMemoryEnable(1); // lock memory
}
void M24256E_WUnlock(void)
{
    Peripherals_SetMemoryEnable(0); // unlock memory
    Delay_ms(10);
}