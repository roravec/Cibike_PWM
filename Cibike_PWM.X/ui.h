/* 
 * File:   ui.h
 * Author: orras
 *
 * Created on April 17, 2023, 8:52 PM
 */

#ifndef UI_H
#define	UI_H

#include "i2c_display.h"
#include <stdbool.h>
#include <stdio.h>

#define DISPLAY_LINE_LIMIT  16
#define DISPLAY_LINES       2


#ifdef	__cplusplus
extern "C" {
#endif
    
    typedef enum {LINE_1=0, LINE_2} UI_Line;

    void UI_Init(void);
    void UI_Clear();
    void UI_WriteLine(UI_Line line, uint8_t * str);
    void UI_PrintStatus(uint8_t activeLine, uint16_t freq, uint8_t channel, uint8_t value);
    void UI_PrintChannelDuty(uint8_t channel, uint8_t value);
    void UI_PrintFreq(uint16_t freq);


#ifdef	__cplusplus
}
#endif

#endif	/* UI_H */


//1234567890123456
//99 99 99 99 99 9
//