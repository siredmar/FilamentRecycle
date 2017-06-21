// Ansteuerung eines HD44780 kompatiblen LCD im 4-Bit-Interfacemodus
// http://www.mikrocontroller.net/articles/HD44780
// http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/LCD-Ansteuerung
//
// Die Pinbelegung ist über defines in lcd-routines.h einstellbar

#include <avr/io.h>
#include <util/delay.h>
#include "Lcd.h"
#include "Gpio.h"

static void Lcd_Enable(void)
{
    Gpio_WriteChannel(LCD_EN, GPIO_LOW);
    _delay_us(LCD_ENABLE_US);
    Gpio_WriteChannel(LCD_EN, GPIO_HIGH);
    _delay_us(LCD_ENABLE_US);
    Gpio_WriteChannel(LCD_EN, GPIO_LOW);
    _delay_us(LCD_ENABLE_US);
}

static void Lcd_SetMode(Lcd_CommandDataType Mode)
{
    if(Mode == LCD_COMMAND)
    {
        Gpio_WriteChannel(LCD_RS, GPIO_LOW);
    }
    else
    {
        Gpio_WriteChannel(LCD_RS, GPIO_HIGH);
    }
}

static void Lcd_WriteData(uint8 data)
{
    Gpio_WriteChannel(LCD_D7, (Gpio_PinState)GET_BIT(data, 7));
    Gpio_WriteChannel(LCD_D6, (Gpio_PinState)GET_BIT(data, 6));
    Gpio_WriteChannel(LCD_D5, (Gpio_PinState)GET_BIT(data, 5));
    Gpio_WriteChannel(LCD_D4, (Gpio_PinState)GET_BIT(data, 4));
    Lcd_Enable();
    _delay_us(LCD_WRITEDATA_US);

    Gpio_WriteChannel(LCD_D7, (Gpio_PinState)GET_BIT(data, 3));
    Gpio_WriteChannel(LCD_D6, (Gpio_PinState)GET_BIT(data, 2));
    Gpio_WriteChannel(LCD_D5, (Gpio_PinState)GET_BIT(data, 1));
    Gpio_WriteChannel(LCD_D4, (Gpio_PinState)GET_BIT(data, 0));
    Lcd_Enable();
    _delay_us(LCD_WRITEDATA_US);
}

static void Lcd_CommandNibble(uint8 data)
{
    Lcd_SetMode(LCD_COMMAND);
    Gpio_WriteChannel(LCD_D7, (Gpio_PinState)GET_BIT(data, 3));
    Gpio_WriteChannel(LCD_D6, (Gpio_PinState)GET_BIT(data, 2));
    Gpio_WriteChannel(LCD_D5, (Gpio_PinState)GET_BIT(data, 1));
    Gpio_WriteChannel(LCD_D4, (Gpio_PinState)GET_BIT(data, 0));
    Lcd_Enable();
}


void Lcd_Init(void)
{
    _delay_ms(LCD_BOOTUP_MS);

    Lcd_CommandNibble(0x03);    /* 0x3 -> 0x0011 */
    _delay_ms(LCD_SOFT_RESET_MS1);

    Lcd_CommandNibble(0x03);    /* 0x3 -> 0x0011 */
    _delay_ms(LCD_SOFT_RESET_MS2);

    Lcd_CommandNibble(0x03);    /* 0x3 -> 0x0011 */
    _delay_ms(LCD_SOFT_RESET_MS3);

    Lcd_CommandNibble(0b0010); // Function Set
    _delay_ms(LCD_COMMAND_US);

    Lcd_CommandNibble(0b0010); // Function Set
    Lcd_CommandNibble(0b1100); // nfxx: n=1: 2 line, f=1: display on
    _delay_ms(LCD_COMMAND_US);

    Lcd_CommandNibble(0b0000); // Display On/Off Control
    Lcd_CommandNibble(0b1100); // 1dcb: d=1: dsp on, c=0: cursor off, b=0: blink off
    _delay_ms(LCD_COMMAND_US);

    Lcd_CommandNibble(0b0000); // Display Clear
    Lcd_CommandNibble(0b0001);
    _delay_ms(LCD_COMMAND_US);

    Lcd_CommandNibble(0b0000); // Entry Mode Set
    Lcd_CommandNibble(0b0010); // 01is: i=1: increment, s=0: shift off
    _delay_ms(LCD_COMMAND_US);

    Lcd_Clear();
    Lcd_Home();
}

void Lcd_Data(uint8 data)
{
    Lcd_SetMode(LCD_DATA);

    Lcd_WriteData(data);

    _delay_us(LCD_WRITEDATA_US);
}

void Lcd_Command(uint8 data)
{
    Lcd_SetMode(LCD_COMMAND);

    Lcd_WriteData(data);

    _delay_us(LCD_COMMAND_US);
}

void Lcd_Clear(void)
{
    Lcd_Command(LCD_CLEAR_DISPLAY);
    _delay_ms(LCD_CLEAR_DISPLAY_MS);
}

void Lcd_Home(void)
{
    Lcd_Command(LCD_CURSOR_HOME);
    _delay_ms(LCD_CURSOR_HOME_MS);
}

void Lcd_SetCursor(uint8 x, uint8 y)
{
    uint8 data;

    switch (y)
    {
        case 0:
            data = LCD_SET_DDADR + LCD_DDADR_LINE1 + x;
            break;

        case 1:
            data = LCD_SET_DDADR + LCD_DDADR_LINE2 + x;
            break;

        case 2:
            data = LCD_SET_DDADR + LCD_DDADR_LINE3 + x;
            break;

        case 3:
            data = LCD_SET_DDADR + LCD_DDADR_LINE4 + x;
            break;

        default:
            return;
    }

    Lcd_Command(data);
}

void Lcd_String(const char *data)
{
    while(*data != '\0')
        Lcd_Data(*data++);
}

void Lcd_StringAtPosition(const char *data, uint8 x, uint8 y)
{
    Lcd_SetCursor(x, y);
    while(*data != '\0')
        Lcd_Data(*data++);
}

void Lcd_PrintFloat(float32 variable, uint8 x, uint8 y)
{
    uint8 string[20];
    sprintf(string, "%.2f", (float32)variable);
    Lcd_StringAtPosition(string, x, y);
}


void Lcd_GenerateChar(uint8 startadresse, const uint8 *data)
{
    Lcd_Command(LCD_SET_CGADR | (startadresse << 3));

    for (uint8 i = 0; i < 8; i++)
    {
        Lcd_Data(data[i]);
    }
    Lcd_Command(LCD_SET_DDADR);
}
