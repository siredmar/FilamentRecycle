/*
 * App.c
 *
 *  Created on: Jun 21, 2017
 *      Author: armin
 */

#include <string.h>
#include "App.h"
#include "Lcd.h"
#include "RotaryEncoder.h"
#include "Caliper.h"
#include "Heating.h"

App_DataType App_Data_s;
extern Caliper_DataType Caliper_Data_s;
extern Heating_DataType Heating_Data_s;

void App_Init(void)
{
    App_Data_s.HeatingPercentage_f32 = 0.0;
    App_Data_s.Temperature_f32 = 0.0;
    App_Data_s.RotaryEncoder_s.State_s = ROTARY_ENCODER_BTN_NOT_PRESSED;
    App_Data_s.RotaryEncoder_s.Value_si8 = 0;
}

void App_Handler(void)
{
    App_StatusScreen();
}

void App_StatusScreen(void)
{
    uint8 string[30];
    sprintf(string, "Rot: %i  ", (sint8)App_Data_s.RotaryEncoder_s.Value_si8);
    Lcd_StringAtPosition(string, 0, 1);

    if(App_Data_s.RotaryEncoder_s.State_s == ROTARY_ENCODER_BTN_PRESSED)
    {
        Lcd_StringAtPosition("Pressed ", 0, 11);
    }
    else
    {
        Lcd_StringAtPosition("Released", 0, 11);
    }


    Lcd_StringAtPosition("d: ", 0, 3);
    Lcd_PrintFloat(Caliper_Data_s.Measurement.Result_f32, 2, 3);
    Lcd_StringAtPosition("mm", 7, 3);

    Lcd_StringAtPosition("T:", 10, 3);
    Lcd_PrintFloat(Heating_Data_s.ActualTemperature_f32, 12, 3);
    Lcd_Data((char)223);
    Lcd_Data('C');
    if(Heating_Data_s.ActualTemperature_f32 < 100.0)
    {
        Lcd_Data(' ');
    }
}
