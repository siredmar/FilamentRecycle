/*
 * Caliper.c
 *
 *  Created on: Jan 17, 2017
 *      Author: Armin Schlegel
 *      based on the work of Martin Thalheimer
 *      https://sites.google.com/site/marthalprojects/home/arduino/arduino-reads-digital-caliper
 */

#include "Caliper.h"
#include <avr/interrupt.h>
#include "Gpio.h"
#include "Uart.h"
#include "Dbg.h"
#include "EcuM.h"

#ifdef CALIPER_PINS_ADC
#include "Adc.h"

#define CALIPER_ERROR_TIMEOUT (10000u)
#define CALIPER_CLK_PIN (ADC_CHANNEL_7)
#define CALIPER_DAT_PIN (ADC_CHANNEL_6)
#define CALIPER_ADC_GPIO_HIGH_LIMIT (768u)

Caliper_DataType Caliper_Data_s;
static void Caliper_DecodeData(void);

Gpio_PinState Caliper_CheckPinState(Adc_ChannelType_e ch)
{
    Gpio_PinState retVal = GPIO_LOW;
    Adc_SetChannel(ch);
    if(Adc_Read10bit() > CALIPER_ADC_GPIO_HIGH_LIMIT)
    {
        retVal = GPIO_HIGH;
    }
    else
    {
        retVal = GPIO_LOW;
    }
    return retVal;
}

#else
/* Configuration defines for clock and data pins */
#define CALIPER_CLK_PIN (GPIO_CHANNEL_PD6)
#define CALIPER_DAT_PIN (GPIO_CHANNEL_PD7)

Gpio_PinState Caliper_CheckPinState(Gpio_ChannelType ch)
{
    return Gpio_ReadChannel(ch);
}

#endif

void Caliper_Init(void)
{
    Caliper_Data_s.Measurement.Result_f32 = 0.0f;
    Caliper_Data_s.Measurement.Timestamp_ui32 = 0;
}

Caliper_StatusType Caliper_Handler(void)
{
    Caliper_StatusType Ret = CALIPER_STATUS_ERROR;
    uint32 OldTimestampMicroseconds = 0u;
    uint32 NewTimestampMicroseconds = 0u;
    sint32 Timediff = 0;
    //if clock is LOW wait until it turns to HIGH
    while (Caliper_CheckPinState(CALIPER_CLK_PIN) == GPIO_HIGH) { }
    OldTimestampMicroseconds = EcuM_GetMicros();
    //wait for the end of the HIGH pulse
    while (Caliper_CheckPinState(CALIPER_CLK_PIN) == GPIO_LOW) { }
    cli();
    NewTimestampMicroseconds = EcuM_GetMicros();
    Timediff = (NewTimestampMicroseconds-OldTimestampMicroseconds);

    /* if the HIGH pulse was longer than 500 micros we are at the start of a new bit sequence */
    if (Timediff > 500)
    {
        Caliper_DecodeData(); //decode the bit sequence
    }
    sei();
    return Ret;
}

void Caliper_PrintOutput(void)
{
    Dbg_ReadVariableFloat("\r\nWidth [mm]:\t\t", Caliper_Data_s.Measurement.Result_f32);
    //    Dbg_ReadVariableInteger("Timestamp [ms]: ", Caliper_Data_s.Measurement.Timestamp_ui32);
}

static void Caliper_DecodeData(void)
{
    sint32 value = 0;
    uint8 i;
    sint32 sign = 1;

    for(i = 0; i < 23; i++)
    {
        while(Caliper_CheckPinState(CALIPER_CLK_PIN) == GPIO_HIGH) {} //wait until clock returns to HIGH- the first bit is not needed
        while(Caliper_CheckPinState(CALIPER_CLK_PIN) == GPIO_LOW) {} //wait until clock returns to LOW
        if (Caliper_CheckPinState(CALIPER_DAT_PIN) == GPIO_LOW)
        {
            if (i<20)
            {
                value|= 1<<i;
            }
            if (i==20)
            {
                sign=-1;
            }
        }
        else
        {
            /* do nothing */
        }
    }
    Caliper_Data_s.Measurement.Result_f32 = (value * sign) / 100.00;
    Caliper_Data_s.Measurement.Timestamp_ui32 = EcuM_GetMillis();
}
