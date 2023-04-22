#include "pwm.h"

/* File:   pwm.c
 * Author: skorra
 * Created on July 27, 2022, 7:59 AM
 * Version: 
 * 
 * Description: 
 */

#include "pwm.h"

static uint8_t preScale = 64;
static uint32_t pwmClockFreq = PWM_CLOCK_FREQ;

static uint32_t actualFqPeriod = 0;
static uint32_t actualDutyPeriod = 0;

void PWM_Init()
{
    T2CONbits.ON = 0;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    TMR2 = 0;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    //PR1 = 0xFFFF;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    T2CONbits.TGATE = 0;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    //T2CONbits.TSYNC = 0;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    //T2CONbits.TSYNC = 0;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    T2CONbits.TCKPS = 0b111; // 1:1
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    T2CONbits.T32 = 1; // 32bit
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    T2CONbits.ON = 1;
    
    
    OC1CON = 0;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    OC1CONbits.OC32 = 1;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    OC1CONbits.OCM = 0b110; // pwm mode
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    OC1CONbits.OCTSEL = 0; // timer2 is input
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    PR2 = 90000;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    OC1R = 1;
    OC1RS = PR2/2;
    asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
    OC1CONbits.ON = 1; // turn on
}

void PWM_Set(float pwmFreq, uint8_t dutyPercent)
{
    // make correction
    if (pwmFreq < 501)
        pwmFreq = pwmFreq * 0.99;
    else if (pwmFreq >= 501 && pwmFreq < 800){}
    else
        pwmFreq = pwmFreq * 1.01;
    uint32_t topP = (pwmClockFreq + (preScale * pwmFreq) / 2) ;
    uint32_t bottomP = (preScale * pwmFreq) - 1;
    uint32_t period = topP / bottomP;
    // With CCP1RA = 0, duty cycle ratio is CCP1RB / (CCP1PR + 1)
    float dutyCalcF = 0;
    dutyCalcF = ((float)period / (float)100);
    float dutyCalc = (float)dutyCalcF * (float)dutyPercent;
    PWM_SetDirect(period, dutyCalc);
}

void PWM_SetDuty(uint8_t dutyPercent)
{
    float dutyCalcF = 0;
    dutyCalcF = ((float)actualFqPeriod / (float)100);
    float dutyCalc = (float)dutyCalcF * (float)dutyPercent;
    PWM_SetDirect(actualFqPeriod, dutyCalc);
}

void PWM_SetDirect(uint32_t period, uint32_t duty)
{
    actualFqPeriod = period;
    actualDutyPeriod = duty;
    //CCP1PR = period; // 62499
    PR1 = period; // set pwm frequency
    OC1R = duty; // set duty
}