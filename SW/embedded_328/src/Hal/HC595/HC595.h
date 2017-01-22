/* *************************************************************************************************
 * \file      ic74hc595.h
 *
 *          This module offers functionality to drive a shift register queue of 74HC595 chips. The 
 *          values to put out on the IC pins are stored in an array and have to be explicitly set 
 *          by calling IC74HC595_updateOutput(). All configuration is done in the header file. The 
 *          pin numbering starts at 0 and ends at ((IC74HC595_ICS * 8) - 1). The ICs are counted 
 *          from 0 to IC74HC595_ICS.
 *          The module is designed to need as little MCU pins as possible. So the output enable and  
 *          also the clear line are always active respectively connected to VCC.
 *
 * \verbatim
 *                 LSB  <---------------------------->  MSB
 *
 *                 QH.....QA      QH.....QA      QH.....QA
 *
 *  +-------+ SI  +--------+ SI  +--------+ SI  +--------+
 *  |       |---->|  IC 0  +---->|  IC 1  +---->|  IC 2  |
 *  |       |     +--------+     +--------+     +--------+
 *  |       |        ^  ^           ^  ^           ^  ^
 *  | M C U | SCK    |  |           |  |           |  |
 *  |       |--------+--|-----------+--|-----------+  |
 *  |       | RCK       |              |              |
 *  |       |-----------+--------------+--------------+
 *  +-------+
 *
 * \endverbatim
 *
 * \ingroup   ic74hc595
 *
 * \author    Manuel Lederhofer (code@0x08f.de)
 *
 * \date      15.11.2013
 *
 * \version   1.0   working
 *
 * \copyright http://creativecommons.org/licenses/by-nc-sa/3.0/
 **************************************************************************************************/
#ifndef _HC595_H_
#define _HC595_H_
/* ============================================================================================== */

/* ------------------------------------ INCLUDES ------------------------------------------------ */
#include "Std_Types.h"
#include "Gpio.h"


/* ------------------------------------ DEFINES ------------------------------------------------- */

/* amount of cascaded ICs */
#define HC595_NUMBER_OF_ICS       1

/* port setup */
#define HC595_PIN_SCK   GPIO_CHANNEL_PC3
#define HC595_PIN_RCK   GPIO_CHANNEL_PC2
#define HC595_PIN_SI    GPIO_CHANNEL_PC1


/* ------------------------------------ PROTOTYPES ---------------------------------------------- */
void HC595_Init(void);
void HC595_UpdateOutput(void);
void HC595_ClearAll(void);
void HC595_SetAll(void);
void HC595_SetBit(const uint8 pin, const uint8 val);
void HC595_SetIc(const uint8 icNumber, const uint8 byteVal);

uint8 HC595_GetBit(const uint8 pin);
uint8 HC595_GetIc(const uint8 icNumber);


/* ************************************ E O F *************************************************** */
#endif /* _IC74HC595_H_ */
