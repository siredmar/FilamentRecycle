/*
 * rotaryEncoder.c
 *
 *  Created on: Jul 18, 2015
 *      Author: armin
 */


#include "RotaryEncoder.h"
#include "Gpio.h"

volatile RotaryEncoder_DataType RotaryEncoder_Data_s;


void RotaryEncoder_Init(void)
{
    RotaryEncoder_Data_s.SamplingAllowed = TRUE;
    RotaryEncoder_Data_s.Encoder_s = ROTARY_ENCODER_NO_EVENT;
    RotaryEncoder_Data_s.Button_s.Event_e = ROTARY_ENCODER_BTN_EVENT_RELEASE;
    RotaryEncoder_Data_s.Button_s.State_e = ROTARY_ENCODER_BTN_NOT_PRESSED;
}

/* returns change in encoder state (-1,0,1) */
RotaryEncoder_DataType RotaryEncoder_Handler(void)
{
    RotaryEncoder_DataType Ret;
    static uint8 DebounceCounter = 0;

    static sint8 enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
    static uint8 old_AB = 0;
    uint8 TmpPort = 0;
    uint8 Port = 0;
    uint8 A, B;
    /**/
    old_AB <<= 2;                   //remember previous state
    TmpPort = Gpio_ReadPort(GPIO_PORTD);
    A = GET_BIT(TmpPort, 4);
    B = GET_BIT(TmpPort, 6);
    if(A)
        SET_BIT(Port, 0);
    if(B)
        SET_BIT(Port, 1);
    Port &= 0x03;

    old_AB |= ( Port & 0x03 );  //add current state
    Ret.Encoder_s = RotaryEncoder_Data_s.Encoder_s;
    if(RotaryEncoder_Data_s.SamplingAllowed == TRUE)
    {
        switch(enc_states[(old_AB & 0x0f)])
        {
            case -1:
                Ret.Encoder_s = ROTARY_ENCODER_EVENT_RIGHT;
                break;
            case 1:
                Ret.Encoder_s = ROTARY_ENCODER_EVENT_LEFT;
                break;
            case 0:
            default:
                Ret.Encoder_s = ROTARY_ENCODER_NO_EVENT;
                break;
        }
        RotaryEncoder_Data_s.Encoder_s = Ret.Encoder_s;
        RotaryEncoder_Data_s.SamplingAllowed = FALSE;
    }
    Ret.Button_s.State_e = (RotaryEncoder_ButtonStateType)Gpio_ReadChannel(ROTARY_ENCODER_BUTTON_PIN);
    return Ret;
}

RotaryEncoder_RotEventType RotaryEncoder_ReadEvent(void)
{
    RotaryEncoder_RotEventType Ret = ROTARY_ENCODER_NO_EVENT;
    if(RotaryEncoder_Data_s.SamplingAllowed == FALSE)
    {
        Ret = RotaryEncoder_Data_s.Encoder_s;
        RotaryEncoder_Data_s.Encoder_s = ROTARY_ENCODER_NO_EVENT;
        RotaryEncoder_Data_s.SamplingAllowed = TRUE;
    }
    return Ret;
}
