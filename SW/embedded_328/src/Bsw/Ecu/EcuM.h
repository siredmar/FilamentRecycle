/*
 * EcuM.h
 *
 *  Created on: 04.05.2016
 *      Author: ASchl
 */

#ifndef ECUM_H_
#define ECUM_H_

typedef enum
{
    ECUM_MCU_WAKE
} EcuM_McuSleepingType;


typedef enum
{
    RESET_NO_RESET = (0u),
    RESET_SOFT_RESET_WITH_POR,
    RESET_SOFT_RESET
} Reset_ResetSourceType;

typedef struct
{
    Reset_ResetSourceType ResetSource_e;
    EcuM_McuSleepingType  CurrentSleepState_e;
} EcuM_DataType;

EcuM_McuSleepingType EcuM_GetMcuSleepingState(void);
void EcuM_SetMcuSleepingState(EcuM_McuSleepingType state);
void EcuM_SetResetSource(Reset_ResetSourceType Source);
void EcuM_InitSystem(void);
void EcuM_Handler(void);
uint32 EcuM_GetMillis(void);
uint32 EcuM_GetMicros(void);
#endif /* ECUM_H_ */
