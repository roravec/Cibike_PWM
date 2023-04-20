#include "PCA9685.h"

static bool Mode1_allcal =     1;
static bool Mode1_sub3 =       0;
static bool Mode1_sub2 =       0;
static bool Mode1_sub1 =       0;
static bool Mode1_sleep =      0;
static bool Mode1_extclk =     0;
static bool Mode1_ai =         1;
static bool Mode1_restart =    0;
static uint8_t Mode2_outne =   0;
static bool Mode2_outdrv =     1;
static bool Mode2_och =        0;
static bool Mode2_invrt =      1;

static uint8_t ChannelDuty[PCA9685_NUMBER_OF_CHANNELS] = { 10,20,30,40,50,60,70,80,90,10,20,30,40,50,60};
static uint8_t Prescale = PCA9685_DEFAULT_PRESCALE;

void PCA9685_SetDefaultValues(uint8_t prescale, uint8_t * channelDuties)
{
    Prescale = prescale;
    for (uint8_t i=0; i<PCA9685_NUMBER_OF_CHANNELS; i++)
        ChannelDuty[i] = channelDuties[i];
}

void PCA9685_Init(void)
{
    //PCA9685_SetPreScale(Prescale);
    PCA9685_SendCommand(PCA9685_REG_PRE_SCALE, Prescale);
    //Delay_ms(50);
    PCA9685_SetMode1(Mode1_allcal, Mode1_sub3, Mode1_sub2, Mode1_sub1, Mode1_sleep, Mode1_extclk, Mode1_restart);
    //Delay_ms(50);
    PCA9685_SetMode2(Mode2_outne, Mode2_outdrv, Mode2_och, Mode2_invrt);
    //Delay_ms(50);
}
void PCA9685_SetMode1(bool allcall, bool sub3, bool sub2, bool sub1, bool sleep, bool extclk, bool restart)
{
    Mode1_allcal = allcall;
    Mode1_sub3 = sub3;
    Mode1_sub2 = sub2;
    Mode1_sub1 = sub1;
    Mode1_sleep = sleep;
    Mode1_extclk = extclk;
    Mode1_restart = restart;
    
    uint8_t value = (uint8_t)allcall | (sub3<<1) | (sub2<<2) | (sub1<<3) | (sleep<<4) | (Mode1_ai<<5) | (extclk<<6) | (restart<<7);
    PCA9685_SendCommand(PCA9685_REG_MODE1, value);
}
void PCA9685_SetMode2(uint8_t outne, bool outdrv, bool och, bool invrt)
{
    Mode2_outne = outne;
    Mode2_outdrv = outdrv;
    Mode2_och = och;
    Mode2_invrt = invrt;
    
    uint8_t value = (uint8_t)outne | (outdrv<<2) | (och<<3) | (invrt<<4);
    PCA9685_SendCommand(PCA9685_REG_MODE2, value);
}
void PCA9685_SendCommand(uint8_t registerAddress, uint8_t value)
{
    I2C_Start(PCA9685_ADDRESS_W);
    I2C_Write(registerAddress, 1);
    I2C_Write(value, 1);
    I2C_Stop();
}

// ON count is DELAY
// OFF count is DELAY + LED_ON
// TOTAL is 4095
void PCA9685_SetChannel(PCA9685_Channel channel, uint16_t valueON, uint16_t valueOFF)
{
    I2C_Start(PCA9685_ADDRESS_W);
    I2C_Write(channel, 1);
    I2C_Write((valueON&0xFF), 1);
    I2C_Write((valueON&0xF00)>>8, 1);
    I2C_Write((valueOFF&0xFF), 1);
    I2C_Write((valueOFF&0xF00)>>8,1);
    I2C_Stop();
}
void PCA9685_SetPreScale(uint8_t iprescale)
{
    Prescale = iprescale;
    PCA9685_SetMode1(Mode1_allcal, Mode1_sub3, Mode1_sub2, Mode1_sub1, 1, Mode1_extclk, Mode1_restart);
    //Delay_ms(50);
    PCA9685_SendCommand(PCA9685_REG_PRE_SCALE, Prescale);
    //Delay_ms(50);
    PCA9685_SetMode1(Mode1_allcal, Mode1_sub3, Mode1_sub2, Mode1_sub1, 0, Mode1_extclk, Mode1_restart);
}
void PCA9685_SetFreq(uint16_t freq)
{
    uint8_t prescale = (PCA9685_CLOCK / (4096*freq)) - 1;
    if (prescale < 3) prescale = 3;
    PCA9685_SetPreScale(prescale);
}
void PCA9685_SetChannelDuty(PCA9685_Channel channel, uint8_t duty)
{
    if (duty > 25 && duty < 75) 
        duty = duty+1;
    uint16_t dutyTime = (PCA9685_MAXIMUM_TIME / 100) * (duty);
    if (duty == 0)
        dutyTime = 0;
    else if (duty == 100)
        dutyTime = PCA9685_MAXIMUM_TIME;
    PCA9685_SetChannel(channel, 0, PCA9685_MAXIMUM_TIME-dutyTime);
//    PCA9685_SetChannel(channel, 409, 1228);
//    for (uint8_t i=0;i<PCA9685_NUMBER_OF_CHANNELS;i++)
//        PCA9685_SetChannel(i, 409, 1228);
//    PCA9685_SetAllChannels(409, 1228);
}
void PCA9685_SetAllChannels(uint16_t valueON, uint16_t valueOFF)
{
    I2C_Start(PCA9685_ADDRESS_W);
    I2C_Write(PCA9685_REG_ALL_LED_ON_L,1);
    I2C_Write((valueON&0xFF), 1);
    I2C_Write((valueON&0xF00)>>8, 1);
    I2C_Write((valueOFF&0xFF), 1);
    I2C_Write((valueOFF&0xF00)>>8, 1);
    I2C_Stop();
}
PCA9685_Channel PCA9685_GetChannelByNum(uint8_t chnum)
{
    switch (chnum)
    {
        case 0: return CHANNEL_0;
        case 1: return CHANNEL_1;
        case 2: return CHANNEL_2;
        case 3: return CHANNEL_3;
        case 4: return CHANNEL_4;
        case 5: return CHANNEL_5;
        case 6: return CHANNEL_6;
        case 7: return CHANNEL_7;
        case 8: return CHANNEL_8;
        case 9: return CHANNEL_9;
        case 10: return CHANNEL_10;
        case 11: return CHANNEL_11;
        case 12: return CHANNEL_12;
        case 13: return CHANNEL_13;
        case 14: return CHANNEL_14;
        case 15: return CHANNEL_15;
        default: return CHANNEL_0;
    }
}