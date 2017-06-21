/*
 * Led.c
 *
 *  Created on: Jun 21, 2017
 *      Author: armin
 */
#include "Led.h"

static void Led_PerformBlinking(void);
volatile Led_DataType Led_Data_s;

void Led_Init(void)
{
    Led_Data_s.BlinkCounter_ui8 = 0;
    Led_Data_s.BlinkLimit_ui8 = LED_STATE_BLINKING_SLOW;
    Led_Data_s.BlinkingState_e = LED_BLINKING_OFF;
    Led_Data_s.State_e = LED_STATE_OFF;

}

void Led_Handler(void)
{
    switch(Led_Data_s.State_e)
    {
        case LED_STATE_OFF:
            Gpio_WriteChannel(LED_GPIO_PIN, GPIO_LOW);
            break;
        case LED_STATE_ON:
            Gpio_WriteChannel(LED_GPIO_PIN, GPIO_HIGH);
            break;
        case LED_STATE_BLINKING_SLOW:
            Led_Data_s.BlinkLimit_ui8 = LED_BLINK_SLOW_LIMIT;
            Led_PerformBlinking();
            break;
        case LED_STATE_BLINKING_FAST:
            Led_Data_s.BlinkLimit_ui8 = LED_BLINK_FAST_LIMIT;
            Led_PerformBlinking();
            break;
        default:
            break;
    }
}

void Led_SetState(Led_StateType state)
{
    Led_Data_s.State_e = state;
}

static void Led_PerformBlinking(void)
{
    if(++Led_Data_s.BlinkCounter_ui8 > Led_Data_s.BlinkLimit_ui8)
    {
        Led_Data_s.BlinkCounter_ui8 = 0;
        if(Led_Data_s.BlinkingState_e == LED_BLINKING_OFF)
        {
            Led_Data_s.BlinkingState_e = LED_BLINKING_ON;
            Gpio_WriteChannel(LED_GPIO_PIN, GPIO_HIGH);
        }
        else if(Led_Data_s.BlinkingState_e == LED_BLINKING_ON)
        {
            Led_Data_s.BlinkingState_e = LED_BLINKING_OFF;
            Gpio_WriteChannel(LED_GPIO_PIN, GPIO_LOW);
        }
        else
        {

        }
    }
}
