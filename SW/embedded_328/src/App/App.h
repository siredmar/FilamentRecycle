/*
 * App.h
 *
 *  Created on: Jun 21, 2017
 *      Author: armin
 */

#ifndef APP_H_
#define APP_H_

#include "Std_Types.h"
#include "RotaryEncoder.h"

typedef struct
{
    float32 HeatingPercentage_f32;
    float32 Temperature_f32;
    RotaryEncoder_EncodedValuesType RotaryEncoder_s;
} App_DataType;

void App_Init(void);
void App_Handler(void);
void App_StatusScreen(void);

#endif /* APP_H_ */
