#include "Timer.h"
#include "EcuM.h"

#define TIMER_SYSTEM_CLK_MHZ (16)

static uint32 SoftTimer_MeasureElapsedTime(const uint32 *st);
static void SoftTimer_StartCyclicTimer(uint32 *st);

static uint32 SoftTimer_MeasureElapsedTime(const uint32 *st)
{
    return (EcuM_GetMicros() - *st);
}

static void SoftTimer_StartCyclicTimer(uint32 *st)
{
    *st = EcuM_GetMicros();
}

uint32 SoftTimer_GetElapsedUs(SoftTimer_DataType *Timer)
{
    Timer->ElapsedTimeUs = SoftTimer_MeasureElapsedTime(&Timer->StartTime);
    return (uint32)Timer->ElapsedTimeUs;
}

uint32 SoftTimer_GetElapsedMs(SoftTimer_DataType *Timer)
{
    return SoftTimer_GetElapsedUs(Timer) / 1000u;
}

uint32 SoftTimer_GetElapsedS(SoftTimer_DataType *Timer)
{
    return SoftTimer_GetElapsedMs(Timer) / 1000u;
}

void SoftTimer_Start(SoftTimer_DataType *Timer)
{
    SoftTimer_StartCyclicTimer(&Timer->StartTime);
}

void SoftTimer_Clear(SoftTimer_DataType *Timer)
{
    Timer->StartTime = 0u;
    Timer->ElapsedTimeUs = 0u;
}

void SoftTimer_DelayUsBlocking(uint16 us)
{
    uint16 Cnt = 0u;
    for(Cnt = 0; Cnt < us*TIMER_SYSTEM_CLK_MHZ; Cnt++)
    {
        __asm("nop");
    }
}

