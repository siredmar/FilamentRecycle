/*
 * Led.h
 *
 *  Created on: Jun 21, 2017
 *      Author: armin
 */

#ifndef LED_H_
#define LED_H_
#include "Std_Types.h"
#include "Gpio.h"

#define LED_BLINK_FAST_LIMIT (2u)
#define LED_BLINK_SLOW_LIMIT (5u)
#define LED_GPIO_PIN (GPIO_CHANNEL_PD7)

typedef enum
{
    LED_STATE_OFF = 0u,
    LED_STATE_ON,
    LED_STATE_BLINKING_FAST,
    LED_STATE_BLINKING_SLOW,
} Led_StateType;

typedef enum
{
    LED_BLINKING_ON = 0u,
    LED_BLINKING_OFF
} Led_BlinkingStateType;

typedef struct
{
    Led_StateType           State_e;
    Led_BlinkingStateType   BlinkingState_e;
    uint8                   BlinkCounter_ui8;
    uint8                   BlinkLimit_ui8;
} Led_DataType;

void Led_Init(void);
void Led_Handler(void);
void Led_SetState(Led_StateType);

#endif /* LED_H_ */
