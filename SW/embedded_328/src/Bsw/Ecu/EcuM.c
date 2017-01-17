#include "Std_Types.h"
#include "string.h"
#include "Uart.h"
#include "Gpio.h"
#include "stdio.h"
#include "EcuM.h"
#include "Uart.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include "Timer.h"

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(b) (((b) * 1000L) / (F_CPU / 1000L))
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))
// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)
// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)


volatile uint32 BaseTimer_ui32 = 0;
volatile uint32 TimestampSeconds_ui32 = 0;
volatile uint8  Flag1ms = 0;
volatile uint8  Flag10ms = 0;
volatile uint8  Flag100ms = 0;
volatile uint8  Flag500ms = 0;
volatile uint8  Flag1000ms = 0;

static void EcuM_HandleResetSources(void);
static void EcuM_DisableWatchdog(void);
static void EcuM_InitModules(void);
static void EcuM_InitWatchdog(void);
static void EcuM_InitBaseTimer(void);
static void EcuM_Init(void);

static volatile EcuM_DataType EcuM_Data_s;
#ifdef WDG_ENABLE
uint16 test;
static void EcuM_InitWatchdog(void)
{

}

static void EcuM_TriggerEnableWatchdog(void)
{

}
#endif
static void EcuM_DisableWatchdog(void)
{
    /* TODO: Porting alert */
}

#ifdef SLEEP_MODE
EcuM_McuSleepingType EcuM_GetMcuSleepingState(void)
{
    return  EcuM_Data_s.CurrentSleepState_e;
}

void EcuM_SetMcuSleepingState(EcuM_McuSleepingType state)
{
    EcuM_Data_s.CurrentSleepState_e = state;
}
#endif

static void EcuM_HandleResetSources(void)
{
    switch(EcuM_Data_s.ResetSource_e)
    {
        case RESET_SOFT_RESET:
            /* Write issue a software reset */

            break;
        case RESET_SOFT_RESET_WITH_POR:
            /* Write issue a software reset */

            break;
        default:
            break;
    }
}

void EcuM_SetResetSource(Reset_ResetSourceType Source)
{
    EcuM_Data_s.ResetSource_e = Source;
}

static void EcuM_InitModules(void)
{
#ifndef WDG_ENABLE
    EcuM_DisableWatchdog();
#endif
    Gpio_Init();
    Caliper_Init();
    Uart_Init();
}

static void EcuM_InitBaseTimer(void)
{
    sei();
    SET_BIT(TCCR0A, WGM01);
    SET_BIT(TCCR0A, WGM00);
    SET_BIT(TCCR0B, CS01);
    SET_BIT(TCCR0B, CS00);
    SET_BIT(TIMSK0, TOIE0);
}

static void EcuM_Init(void)
{
    EcuM_Data_s.ResetSource_e = RESET_NO_RESET;
    EcuM_Data_s.CurrentSleepState_e = ECUM_MCU_WAKE;

    EcuM_InitBaseTimer();
}

void EcuM_InitSystem(void)
{
    EcuM_Init();
    EcuM_InitModules();
#ifdef WDG_ENABLE
    EcuM_InitWatchdog();
#endif
    Uart_WriteString(UART_HWUNIT_0, "Started...\r\n");
    sei(); /* Enable the interrupts */
}

void EcuM_Handler(void)
{
    SoftTimer_DataType Timer;

    if(Flag1ms)
    {
        Flag1ms = 0;
    }

    if(Flag10ms)
    {
        Flag10ms = 0;
#ifdef WDG_ENABLE
        EcuM_TriggerEnableWatchdog();
#endif
    }

    if(Flag100ms)
    {
        Flag100ms = 0;
    }

    if(Flag500ms)
    {
        Flag500ms = 0;
        SoftTimer_Start(&Timer);
        Caliper_Handler();
        Dbg_ReadVariableInteger("Time usage [ms]: ", SoftTimer_GetElapsedMs(&Timer));

        Caliper_PrintOutput();

    }

    if(Flag1000ms)
    {
        Flag1000ms = 0;
    }
}


volatile uint32 EcuM_Timer0OverflowCount = 0;
volatile uint32 EcuM_TimestampMilliseconds = 0;
static uint8 EcuM_Timer0Fract = 0;

SIGNAL(TIMER0_OVF_vect)
{
    uint32 m = EcuM_TimestampMilliseconds;
    uint8 f = EcuM_Timer0Fract;

    m += MILLIS_INC;
    f += FRACT_INC;
    if (f >= FRACT_MAX) {
        f -= FRACT_MAX;
        m += 1;
    }

    EcuM_TimestampMilliseconds = m;
    EcuM_Timer0Fract = f;
    EcuM_Timer0OverflowCount++;

    if(EcuM_TimestampMilliseconds % 1000 == 0)
    {
        TimestampSeconds_ui32++;
        Flag1000ms = TRUE;
    }

    if(EcuM_TimestampMilliseconds % 100 == 0)
    {
        Flag100ms = TRUE;
    }

    if(EcuM_TimestampMilliseconds % 500 == 0)
    {
        Flag500ms = TRUE;
    }

    if(EcuM_TimestampMilliseconds % 10 == 0)
    {
        Flag10ms = TRUE;
    }
}


uint32 EcuM_GetMillis()
{
    uint32 m;
    uint8 oldSREG = SREG;

    cli();
    m = EcuM_TimestampMilliseconds;
    SREG = oldSREG;

    return m;
}

uint32 EcuM_GetMicros() {
    uint32 m;
    uint8 oldSREG = SREG, t;

    cli();
    m = EcuM_Timer0OverflowCount;
    t = TCNT0;


    if ((TIFR0 & _BV(TOV0)) && (t < 255))
        m++;

    SREG = oldSREG;

    return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}

void EcuM_DelayMs(uint32 ms)
{
    uint16 start = (uint16)EcuM_GetMicros();

    while (ms > 0) {
        if (((uint16)EcuM_GetMicros() - start) >= 1000) {
            ms--;
            start += 1000;
        }
    }
}

