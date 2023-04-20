/* 
 * File:   hwinterface.h
 * Author: skorra
 *
 * Created on March 29, 2023, 2:03 PM
 */

#ifndef HWINTERFACE_H
#define	HWINTERFACE_H

#include <xc.h>

#define HW_ROTARY_PLUS_TRIS     TRISBbits.TRISB12  
#define HW_ROTARY_PLUS_PORT     PORTBbits.RB12
#define HW_ROTARY_PLUS_LAT      LATBbits.LATB12
#define HW_ROTARY_MINUS_TRIS    TRISBbits.TRISB11 
#define HW_ROTARY_MINUS_PORT    PORTBbits.RB11
#define HW_ROTARY_MINUS_LAT     LATBbits.LATB11

#define HW_POS1_TRIS            TRISFbits.TRISF4
#define HW_POS1_PORT            PORTFbits.RF4
#define HW_POS1_LAT             LATFbits.LATF4
#define HW_POS2_TRIS            TRISBbits.TRISB13 
#define HW_POS2_PORT            PORTBbits.RB13
#define HW_POS2_LAT             LATBbits.LATB13
#define HW_POS3_TRIS            TRISBbits.TRISB14 
#define HW_POS3_PORT            PORTBbits.RB14
#define HW_POS3_LAT             LATBbits.LATB14
#define HW_POS4_TRIS            TRISBbits.TRISB15 
#define HW_POS4_PORT            PORTBbits.RB15
#define HW_POS4_LAT             LATBbits.LATB15

#define HW_LOCK_TRIS            TRISBbits.TRISB6
#define HW_LOCK_PORT            PORTBbits.RB6
#define HW_LOCK_LAT             LATBbits.LATB6
#define HW_RESET_TRIS           TRISBbits.TRISB9
#define HW_RESET_PORT           PORTBbits.RB9
#define HW_RESET_LAT            LATBbits.LATB9

#define HW_FREQ_DUTY_TRIS       TRISBbits.TRISB7
#define HW_FREQ_DUTY_PORT       PORTBbits.RB7
#define HW_FREQ_DUTY_LAT        LATBbits.LATB7

#define HW_MEMORY_ENABLE_TRIS   TRISCbits.TRISC13
#define HW_MEMORY_ENABLE_PORT   PORTCbits.RC13
#define HW_MEMORY_ENABLE_LAT    LATCbits.LATC13
#define HW_PWMCON_ENABLE_TRIS   TRISDbits.TRISD0
#define HW_PWMCON_ENABLE_PORT   PORTDbits.RD0
#define HW_PWMCON_ENABLE_LAT    LATDbits.LATD0

#define HW_TI_OUT_TRIS          TRISDbits.TRISD1
#define HW_TI_OUT_PORT          PORTDbits.RD1
#define HW_TI_OUT_LAT           LATDbits.LATD1


#ifdef	__cplusplus
extern "C" {
#endif

    void HW_Init(void);

#ifdef	__cplusplus
}
#endif

#endif	/* HWINTERFACE_H */

