#include "ui.h"

static uint8_t Line[DISPLAY_LINES][DISPLAY_LINE_LIMIT];

void UI_Init(void)
{
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
    for (uint8_t i=0; i < DISPLAY_LINE_LIMIT ;i++)
    {
        if (str[i] == 0x00) copyEnable = 0;
        if (copyEnable) Line[line][i] = str[i];
        else Line[line][i] = ' ';
        LCD_Write_Char(Line[line][i]);
    }
}