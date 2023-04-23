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

void UI_PrintStatus(uint8_t activeLine, uint16_t freq, uint8_t channel, uint8_t value, bool locked)
{
    uint8_t cursorChar = '>';
    if (locked)
        cursorChar = '-';
    uint8_t actLine1 = ' ';
    uint8_t actLine2 = ' ';
    if (!activeLine) actLine1 = cursorChar;
    else actLine2 = cursorChar;
    
    uint8_t str2[DISPLAY_LINE_LIMIT+2];
    
    sprintf(str2, "%cFreq: %4dHz\0", actLine1,freq);
    UI_WriteLine(LINE_1, str2);
    
    uint8_t str1[DISPLAY_LINE_LIMIT+2];
    sprintf(str1, "%cDuty [%2d]:%3d%\0", actLine2,channel, value);
    UI_WriteLine(LINE_2, str1);
}
void UI_UpdateStatus(uint8_t activeLine, uint16_t freq, uint8_t channel, uint8_t value, bool locked)
{
    uint8_t cursorChar = '>';
    if (locked)
        cursorChar = '-';
    uint8_t actLine1 = ' ';
    uint8_t actLine2 = ' ';
    if (!activeLine) actLine1 = cursorChar;
    else actLine2 = cursorChar;
    
    uint8_t sfreq[DISPLAY_LINE_LIMIT+2];
    sprintf(sfreq, "%4dHz\0",freq);
    uint8_t sduty[DISPLAY_LINE_LIMIT+2];
    uint8_t schannel[DISPLAY_LINE_LIMIT+2];
    sprintf(schannel, "%2d\0", channel);
    sprintf(sduty, "%3d\0", value);
    
    LCD_Set_Cursor(1,1); // cursor 1
    LCD_Write_Char(actLine1);
    LCD_Set_Cursor(2,1); // cursor 2
    LCD_Write_Char(actLine2);
    LCD_Set_Cursor(1,8); // freq
    LCD_Write_String(sfreq);
    LCD_Set_Cursor(2,8); // channel
    LCD_Write_String(schannel);
    LCD_Set_Cursor(2,12); // duty
    LCD_Write_String(sduty);
}

void UI_PrintChannelDuty(uint8_t value)
{
    uint8_t sduty[DISPLAY_LINE_LIMIT+2];
    sprintf(sduty, "%3d\0", value);
    LCD_Set_Cursor(2,12); // duty
    LCD_Write_String(sduty);
}

void UI_PrintFreq(uint16_t freq)
{
    uint8_t sfreq[DISPLAY_LINE_LIMIT+2];
    sprintf(sfreq, "%4dHz\0",freq);
    
    LCD_Set_Cursor(1,8); // freq
    LCD_Write_String(sfreq);
}

void UI_PrintUpdateStart()
{
    LCD_Set_Cursor(1,16); // cursor 1
    LCD_Write_Char('>');
}
void UI_PrintUpdateEnd()
{
    LCD_Set_Cursor(1,16); // cursor 1
    LCD_Write_Char(' ');
}

void UI_ClearLine(uint8_t line)
{
    if (line >= DISPLAY_LINES) return;
    for (uint8_t i=0; i<DISPLAY_LINE_LIMIT ;i++)
            Line[line][i] = 0x00;
}

void UI_PrintMemData(uint8_t * data, uint8_t len)
{
    uint8_t str2[DISPLAY_LINE_LIMIT+2];
    sprintf(str2, "%3d-%3d-%3d-%3d\0", data[0],data[1],data[2],data[3]);
    UI_WriteLine(LINE_1, str2);
    
    uint8_t str1[DISPLAY_LINE_LIMIT+2];
    sprintf(str1, "%3d-%3d-%3d-%3d\0", data[4],data[5],data[6],data[7]);
    UI_WriteLine(LINE_2, str1);
}