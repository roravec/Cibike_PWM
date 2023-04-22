#include "M24256E.h"

void M24256E_Init(void)
{
    Peripherals_SetMemoryEnable(1); // lock memory
}
uint8_t M24256E_ReadByte(uint16_t address)
{
    I2C_Start(M24256E_ADDRESS_W);
    I2C_Write(address>>8, 1);
    I2C_Write(address, 1);
    //I2C_Stop();
    I2C_Restart(M24256E_ADDRESS_R);
    //I2C_Start(M24256E_ADDRESS_R);
    uint8_t data = 0;
    data = I2C_Read(0);
    I2C_Stop();
}
void M24256E_ReadPage(uint16_t address, uint8_t * data, uint8_t len)
{
    if (data == 0) return; // invalid pointer
    if (len == 0 || len > M24256E_PAGE_SIZE) // array length check
        len = M24256E_PAGE_SIZE;
    I2C_Start(M24256E_ADDRESS_W);
    I2C_Write(address>>8, 1);
    I2C_Write(address, 1);
    //I2C_Stop();
    //I2C_Start(M24256E_ADDRESS_R);
    I2C_Restart(M24256E_ADDRESS_R);
    for (uint8_t i=0; i < M24256E_PAGE_SIZE; i++)
    {
        bool ack = 1;
        if (i==M24256E_PAGE_SIZE-1) // last data byte, no ack
            ack = 0;
        data[i] = I2C_Read(ack);
    }
    I2C_Stop();
}
void M24256E_WriteByte(uint16_t address, uint8_t data)
{
    Peripherals_SetMemoryEnable(0); // unlock memory
    I2C_Start(M24256E_ADDRESS_W);
    I2C_Write(address>>8, 1);
    I2C_Write(address, 1);
    I2C_Write(data, 1);
    I2C_Stop();
    Peripherals_SetMemoryEnable(1); // lock memory
}
void M24256E_WritePage(uint16_t address, uint8_t * data, uint8_t len)
{
    Peripherals_SetMemoryEnable(0); // unlock memory
    I2C_Start(M24256E_ADDRESS_W);
    I2C_Write(address>>8, 1);
    I2C_Write(address, 1);
    for (uint8_t i=0; i < M24256E_PAGE_SIZE; i++)
    {
        bool ack = 1;
        if (i==M24256E_PAGE_SIZE-1) // last data byte, no ack
            ack = 1;
        I2C_Write(data[i], ack);
    }
    I2C_Stop();
    Peripherals_SetMemoryEnable(1); // lock memory
}