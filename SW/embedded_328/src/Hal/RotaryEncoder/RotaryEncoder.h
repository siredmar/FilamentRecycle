#ifndef ROTARYENCODER_H_
#define ROTARYENCODER_H_

#include <avr/io.h>
#include "Std_Types.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Gpio.h"

//#define ROTARY_ENCODER_1
#define ROTARY_ENCODER_2
//#define ROTARY_ENCODER_4
#define ROTARY_ENCODER_BUTTON

#define ROTARY_ENCODER_PHASE_A_PIN (GPIO_CHANNEL_PD4)
#define ROTARY_ENCODER_PHASE_B_PIN (GPIO_CHANNEL_PD6)


#ifdef ROTARY_ENCODER_BUTTON
#define ROTARY_ENCODER_BUTTON_PIN   (GPIO_CHANNEL_PD5)
#endif

typedef enum
{
    ROTARY_ENCODER_NO_EVENT = 0,
    ROTARY_ENCODER_EVENT_LEFT,
    ROTARY_ENCODER_EVENT_RIGHT,
} RotaryEncoder_RotEventType;

typedef enum
{
    ROTARY_ENCODER_BTN_PRESSED = 0,
    ROTARY_ENCODER_BTN_NOT_PRESSED,
} RotaryEncoder_ButtonStateType;

typedef enum
{
    ROTARY_ENCODER_BTN_EVENT_RELEASE = 0,
    ROTARY_ENCODER_BTN_EVENT_PRESS
} RotaryEncoder_ButtonEventType;

typedef struct
{
    RotaryEncoder_ButtonStateType State_e;
    RotaryEncoder_ButtonEventType Event_e;
} RotaryEncoder_ButtonDataType;

typedef struct
{
    RotaryEncoder_ButtonDataType    Button_s;
    RotaryEncoder_RotEventType      Encoder_s;
    bool                            SamplingAllowed;
} RotaryEncoder_DataType;

typedef struct
{
    sint8                           Value_si8;
    RotaryEncoder_ButtonStateType   State_s;
} RotaryEncoder_EncodedValuesType;

void RotaryEncoder_Init(void);
RotaryEncoder_DataType RotaryEncoder_Handler(void);

#endif /* ROTARYENCODER_H_ */
