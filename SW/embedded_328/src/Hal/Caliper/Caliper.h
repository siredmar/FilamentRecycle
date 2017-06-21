/*
 * Caliper.h
 *
 *  Created on: Jan 17, 2017
 *      Author: Armin Schlegel
 *      based on the work of Martin Thalheimer
 *      https://sites.google.com/site/marthalprojects/home/arduino/arduino-reads-digital-caliper
 */

#include "Std_Types.h"

#ifndef CALIPER_H_
#define CALIPER_H_

/* Selects whether caliper input pins are pure ADC pins used as GPIO inputs or real GPIO pins */
#define CALIPER_PINS_ADC

typedef enum
{
    CALIPER_STATUS_ERROR = 0u,
    CALIPER_STATUS_OK
} Caliper_StatusType;

typedef struct
{
    uint32 Timestamp_ui32;
    float32 Result_f32;
} Caliper_ResultType;

typedef struct
{
    Caliper_ResultType Measurement;
} Caliper_DataType;

Caliper_StatusType Caliper_Handler(void);
void Caliper_Init(void);
void Caliper_PrintOutput(void);

#endif /* CALIPER_H_ */
