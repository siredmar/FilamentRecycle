#ifndef _TIMER_H_
#define _TIMER_H_

#include "Std_Types.h"

//*****************************************
// Defines
//*****************************************

//*****************************************
// Strukturen
//*****************************************

//*****************************************
// Funktionsprototypen
//*****************************************
typedef struct
{
    uint32 StartTime;
    uint32 ElapsedTimeUs;
} SoftTimer_DataType;

uint32 SoftTimer_GetElapsedMs(SoftTimer_DataType *Timer);
uint32 SoftTimer_GetElapsedS(SoftTimer_DataType *Timer);
void SoftTimer_Start(SoftTimer_DataType *Timer);
void SoftTimer_Clear(SoftTimer_DataType *Timer);

void SoftTimer_DelayUsBlocking(uint16 us);

#endif
