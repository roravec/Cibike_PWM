/* 
 * File:   PCA9685.h
 * Author: skorra
 *
 * Created on March 29, 2023, 1:30 PM
 */

#ifndef PCA9685_H
#define	PCA9685_H

#include <xc.h>
#include <stdbool.h>
#include "i2c.h"
#include "config.h"

/* Registers *****************************/
//#define PCA9685_ADDRESS_W           0xE0
//#define PCA9685_ADDRESS_R           0xE1
#define PCA9685_ADDRESS_W           0x80
#define PCA9685_ADDRESS_R           0x81

#define PCA9685_REG_MODE1           0x00
#define PCA9685_REG_MODE2           0x01
#define PCA9685_REG_SUBADR1         0x02
#define PCA9685_REG_SUBADR2         0x03
#define PCA9685_REG_SUBADR3         0x04
#define PCA9685_REG_ALLCALLADR      0x05

#define PCA9685_REG_LED0_ON_L       0x06
#define PCA9685_REG_LED0_ON_H       0x07
#define PCA9685_REG_LED0_OFF_L      0x08
#define PCA9685_REG_LED0_OFF_H      0x09

#define PCA9685_REG_LED1_ON_L       0x0A
#define PCA9685_REG_LED1_ON_H       0x0B
#define PCA9685_REG_LED1_OFF_L      0x0C
#define PCA9685_REG_LED1_OFF_H      0x0D

#define PCA9685_REG_LED2_ON_L       0x0E
#define PCA9685_REG_LED2_ON_H       0x0F
#define PCA9685_REG_LED2_OFF_L      0x10
#define PCA9685_REG_LED2_OFF_H      0x11

#define PCA9685_REG_LED3_ON_L       0x12
#define PCA9685_REG_LED3_ON_H       0x13
#define PCA9685_REG_LED3_OFF_L      0x14
#define PCA9685_REG_LED3_OFF_H      0x15

#define PCA9685_REG_LED4_ON_L       0x16
#define PCA9685_REG_LED4_ON_H       0x17
#define PCA9685_REG_LED4_OFF_L      0x18
#define PCA9685_REG_LED4_OFF_H      0x19

#define PCA9685_REG_LED5_ON_L       0x1A
#define PCA9685_REG_LED5_ON_H       0x1B
#define PCA9685_REG_LED5_OFF_L      0x1C
#define PCA9685_REG_LED5_OFF_H      0x1D

#define PCA9685_REG_LED6_ON_L       0x1E
#define PCA9685_REG_LED6_ON_H       0x1F
#define PCA9685_REG_LED6_OFF_L      0x20
#define PCA9685_REG_LED6_OFF_H      0x21

#define PCA9685_REG_LED7_ON_L       0x22
#define PCA9685_REG_LED7_ON_H       0x23
#define PCA9685_REG_LED7_OFF_L      0x24
#define PCA9685_REG_LED7_OFF_H      0x25

#define PCA9685_REG_LED8_ON_L       0x26
#define PCA9685_REG_LED8_ON_H       0x27
#define PCA9685_REG_LED8_OFF_L      0x28
#define PCA9685_REG_LED8_OFF_H      0x29

#define PCA9685_REG_LED9_ON_L       0x2A
#define PCA9685_REG_LED9_ON_H       0x2B
#define PCA9685_REG_LED9_OFF_L      0x2C
#define PCA9685_REG_LED9_OFF_H      0x2D

#define PCA9685_REG_LED10_ON_L      0x2E
#define PCA9685_REG_LED10_ON_H      0x2F
#define PCA9685_REG_LED10_OFF_L     0x30
#define PCA9685_REG_LED10_OFF_H     0x31

#define PCA9685_REG_LED11_ON_L      0x32
#define PCA9685_REG_LED11_ON_H      0x33
#define PCA9685_REG_LED11_OFF_L     0x34
#define PCA9685_REG_LED11_OFF_H     0x35

#define PCA9685_REG_LED12_ON_L      0x36
#define PCA9685_REG_LED12_ON_H      0x37
#define PCA9685_REG_LED12_OFF_L     0x38
#define PCA9685_REG_LED12_OFF_H     0x39

#define PCA9685_REG_LED13_ON_L      0x3A
#define PCA9685_REG_LED13_ON_H      0x3B
#define PCA9685_REG_LED13_OFF_L     0x3C
#define PCA9685_REG_LED13_OFF_H     0x3D

#define PCA9685_REG_LED14_ON_L      0x3E
#define PCA9685_REG_LED14_ON_H      0x3F
#define PCA9685_REG_LED14_OFF_L     0x40
#define PCA9685_REG_LED14_OFF_H     0x41

#define PCA9685_REG_LED15_ON_L      0x42
#define PCA9685_REG_LED15_ON_H      0x43
#define PCA9685_REG_LED15_OFF_L     0x44
#define PCA9685_REG_LED15_OFF_H     0x45

#define PCA9685_REG_ALL_LED_ON_L    0xFA
#define PCA9685_REG_ALL_LED_ON_H    0xFB
#define PCA9685_REG_ALL_LED_OFF_L   0xFC
#define PCA9685_REG_ALL_LED_OFF_H   0xFD
#define PCA9685_REG_PRE_SCALE       0xFE
#define PCA9685_REG_TESTMODE        0xFF

/***************************************/

#define PCA9685_NUMBER_OF_CHANNELS  16
#define PCA9685_CLOCK               25000000UL
#define PCA9685_MAXIMUM_TIME        4095
#define PCA9685_PRESCALE_MIN        3
#define PCA9685_PRESCALE_MAX        0xFF
#define PCA9685_DEFAULT_PRESCALE    30
#define PCA9685_DEFAULT_DUTY        0
#define PCA9685_MAX_FREQ            1500
#define PCA9685_MIN_FREQ            25


#define PCA9685_CHANNEL_OFF_OFFSET  2

typedef enum {
            CHANNEL_0 = 0x06,
            CHANNEL_1 = 0x0A,
            CHANNEL_2 = 0x0E,
            CHANNEL_3 = 0x12,
            CHANNEL_4 = 0x16,
            CHANNEL_5 = 0x1A,
            CHANNEL_6 = 0x1E,
            CHANNEL_7 = 0x22,
            CHANNEL_8 = 0x26,
            CHANNEL_9 = 0x2A,
            CHANNEL_10 = 0x2E,
            CHANNEL_11 = 0x32,
            CHANNEL_12 = 0x36,
            CHANNEL_13 = 0x3A,
            CHANNEL_14 = 0x3E,
            CHANNEL_15 = 0x42
            } PCA9685_Channel;


#ifdef	__cplusplus
extern "C" {
#endif

    void PCA9685_Init(void);
    void PCA9685_SetMode1(bool allcall, bool sub3, bool sub2, bool sub1, bool sleep, bool extclk, bool restart);
    void PCA9685_SetMode2(uint8_t outne, bool outdrv, bool och, bool invrt);
    void PCA9685_SendCommand(uint8_t registerAddress, uint8_t value);
    void PCA9685_SetChannel(PCA9685_Channel channel, uint16_t valueON, uint16_t valueOFF);
    void PCA9685_SetPreScale(uint8_t prescale);
    void PCA9685_SetFreq(uint16_t freq);
    void PCA9685_SetChannelDuty(PCA9685_Channel channel, uint8_t duty);
    void PCA9685_SetAllChannels(uint16_t valueON, uint16_t valueOFF);
    PCA9685_Channel PCA9685_GetChannelByNum(uint8_t chnum);

#ifdef	__cplusplus
}
#endif

#endif	/* PCA9685_H */

