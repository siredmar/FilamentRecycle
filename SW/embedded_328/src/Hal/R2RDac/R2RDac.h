/*
 * R2RDac.h
 *
 *  Created on: Jan 22, 2017
 *      Author: armin
 */

#ifndef R2RDAC_H_
#define R2RDAC_H_
#include "Std_Types.h"

void R2RDac_Init(void);
void R2RDac_SetOutputVoltage(float32 OutputVoltage_f32);
void R2RDac_SetOutputPercentage(float32 OutputPercentage_f32);
#endif /* R2RDAC_H_ */
