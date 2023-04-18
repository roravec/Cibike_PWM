#include "i2c_display.h"
#include "config.h"

uint8_t RS, i2c_add, BackLight_State = LCD_BACKLIGHT;

uint8_t firstLine[32] = {};

void LCD_Init() 
{
    i2c_add = LCD_ADDRESS;
    LCD_IO_Expander_Write(0x00);
    Delay_ms(30);
    LCD_CMD(0x03);
    Delay_ms(5);
    LCD_CMD(0x03);
    Delay_ms(5);
    LCD_CMD(0x03);
    Delay_ms(5);
    LCD_CMD(LCD_RETURN_HOME);
    Delay_ms(5);
    LCD_CMD(0x20 | (LCD_TYPE << 2));
    Delay_ms(50);
    LCD_CMD(LCD_TURN_ON);
    Delay_ms(50);
    LCD_CMD(LCD_CLEAR);
    Delay_ms(50);
    LCD_CMD(LCD_ENTRY_MODE_SET | LCD_RETURN_HOME);
    Delay_ms(50);
  
//    for (uint8_t i=0; i<255 ;i++)
//        LCD_Write_Char(0x55);
//    LCD_Set_Cursor(1, 1);
//    LCD_Write_String("  Welcome To ");
//    LCD_Set_Cursor(2, 1);  
//    LCD_Write_String("  MicroDigiSoft ");
}

void LCD_IO_Expander_Write(uint8_t Data) 
{
    I2C_Start(i2c_add);
    I2C_Write(Data | BackLight_State, 0);
    I2C_Stop();
}

void LCD_Write_4Bit(uint8_t Nibble) 
{
    // Get The RS Value To LSB OF Data  
    Nibble |= RS;
    LCD_IO_Expander_Write(Nibble | 0x04);
    LCD_IO_Expander_Write(Nibble & 0xFB);
    Delay_us(50);
}

void LCD_CMD(uint8_t CMD) 
{
    RS = 0; // Command Register Select
    LCD_Write_4Bit(CMD & 0xF0);
    LCD_Write_4Bit((CMD << 4) & 0xF0);
}

void LCD_Write_Char(uint8_t Data)
{
    RS = 1;  // Data Register Select
    LCD_Write_4Bit(Data & 0xF0);
    LCD_Write_4Bit((Data << 4) & 0xF0);
}

void LCD_Write_String(uint8_t* Str)
{
    for(uint16_t i=0; Str[i]!='\0'; i++)
       LCD_Write_Char(Str[i]); 
}

void LCD_Set_Cursor(uint8_t ROW, uint8_t COL) 
{    
    switch(ROW) 
    {
        case 2:
            LCD_CMD(0xC0 + COL-1);
            break;
        case 3:
            LCD_CMD(0x94 + COL-1);
            break;
        case 4:
            LCD_CMD(0xD4 + COL-1);
            break;
        // Case 1  
        default:
            LCD_CMD(0x80 + COL-1);
    }
}

void LCD_Backlight() 
{
    BackLight_State = LCD_BACKLIGHT;
    LCD_IO_Expander_Write(0);
}

void LCD_NoBacklight() 
{
    BackLight_State = LCD_NOBACKLIGHT;
    LCD_IO_Expander_Write(0);
}

void LCD_SL()
{
    LCD_CMD(0x18);
    Delay_us(40);
}

void LCD_SR()
{
    LCD_CMD(0x1C);
    Delay_us(40);
}

void LCD_Clear()
{
    LCD_CMD(0x01); 
    Delay_us(40);
}
