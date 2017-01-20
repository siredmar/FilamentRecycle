/*
 * Heating.h
 *
 *  Created on: Jan 17, 2017
 *      Author: armin
 */

#ifndef HEATING_H_
#define HEATING_H_
#include "Std_Types.h"

typedef struct
{
    float32 TargetTemperature_f32;
    float32 ActualTemperature_f32;
    uint16  TemperatureAdcValue_ui16;
    float32 TemperatureResistorValue_f32;
} Heating_DataType;

void Heating_Init(void);
void Heating_Handler(void);
void Heating_OutputValue(void);

void Heating_SetTemperature(float32 Temp_f32);
float32 Heating_GetTemperature(void);
float32 Adc2Celsius(uint16 raw);

#endif /* HEATING_H_ */
