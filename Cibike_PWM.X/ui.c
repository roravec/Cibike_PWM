#include "ui.h"

static uint8_t Line[DISPLAY_LINES][DISPLAY_LINE_LIMIT+2];

void UI_Init(void)
{
    for (uint8_t j=0; j<DISPLAY_LINES ;j++)
    {
        for (uint8_t i=0; i<DISPLAY_LINE_LIMIT+2 ;i++)
            Line[j][i] = 0x00;
    }
    LCD_Init();
    UI_Clear();
}

void UI_Clear()
{
    LCD_Clear();
}

void UI_WriteLine(UI_Line line, uint8_t * str)
{
    if (line >= DISPLAY_LINE_LIMIT) return; // out of array range
    LCD_Set_Cursor(line+1, 1);
    bool copyEnable = 1;
    UI_ClearLine(line);
    for (uint8_t i=0; i < DISPLAY_LINE_LIMIT ;i++)
    {
        if (str[i] < 32 || str[i] > 126) copyEnable = 0;
        if (copyEnable)
        {
            Line[line][i] = str[i];
        }
        else Line[line][i] = ' ';
        LCD_Write_Char(Line[line][i]);
    }
}

void UI_PrintStatus(uint8_t activeLine, uint16_t freq, uint8_t channel, uint8_t value)
{
    uint8_t actLine1 = ' ';
    uint8_t actLine2 = ' ';
    if (!activeLine) actLine1 = '>';
    else actLine2 = '>';
    
    uint8_t str2[DISPLAY_LINE_LIMIT];
    sprintf(str2, "%cFreq:   %4dHz", actLine1,freq);
    UI_WriteLine(LINE_1, str2);
    
    uint8_t str1[DISPLAY_LINE_LIMIT];
    sprintf(str1, "%cDuty [%2d]:%3d%", actLine2,channel, value);
    UI_WriteLine(LINE_2, str1);
}

void UI_PrintChannelDuty(uint8_t channel, uint8_t value)
{
    uint8_t str[DISPLAY_LINE_LIMIT];
    sprintf(str, "Duty [%2d]:%3d%", channel, value);
    UI_WriteLine(LINE_2, str);
}

void UI_PrintFreq(uint16_t freq)
{
    uint8_t str[DISPLAY_LINE_LIMIT];
    sprintf(str, "Freq: %4dHz", freq);
    UI_WriteLine(LINE_2, str);
}

void UI_ClearLine(uint8_t line)
{
    if (line >= DISPLAY_LINES) return;
    for (uint8_t i=0; i<DISPLAY_LINE_LIMIT+2 ;i++)
            Line[line][i] = 0x00;
}