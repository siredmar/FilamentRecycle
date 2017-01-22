/* ********
 * *****************************************************************************************
 * \file      HC595.c
 *
 *          This module offers functionality to drive a shift register queue of 74HC595 chips. The 
 *          values to put out on the IC pins are stored in an array and have to be explicitly set 
 *          by calling IC74HC595_updateOutput(). All configuration is done in the header file. The 
 *          pin numbering starts at 0 and ends at ((HC595_NUMBER_OF_ICS * 8) - 1). The ICs are counted
 *          from 0 to HC595_NUMBER_OF_ICS.
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
 * \author    Armin Schlegel (knallerbse@gmail.com)
 *
 * \date      22.01.2017
 *
 * \version   1.1   working
 *
 * \copyright http://creativecommons.org/licenses/by-nc-sa/3.0/
 **************************************************************************************************/
 
/* ------------------------------------ INCLUDES ------------------------------------------------ */
#include "HC595.h"


/* ------------------------------------ VARIABLES DECLARATIONS ---------------------------------- */

/* driver buffer to store state of the chips */
static uint8 IcBuffer[HC595_NUMBER_OF_ICS];


/* ------------------------------------ GLOBAL FUNCTIONS ---------------------------------------- */

/* *********************************************************************************************//**
 * Initializes the three needed pins of the MCU to output and drives them to low level.
 **************************************************************************************************/
void HC595_Init(void)
{
    uint8 IcCnt = 0;
    for(IcCnt = 0; IcCnt < HC595_NUMBER_OF_ICS; IcCnt++)
    {
        IcBuffer[IcCnt] = 0;
    }

	/* reset all output pins of the shift registers */
	HC595_UpdateOutput();
}


/* *********************************************************************************************//**
 * Reads out the buffer array and set the outputs of the shift registers to the corresponding 
 * values.
 **************************************************************************************************/
void HC595_UpdateOutput(void)
{
    uint8 i, j;           // loop counter
    
    /* pull the output register clock low; done here because of timing constraints 
     * at higher mcu frequencies 
     */
    Gpio_WriteChannel(HC595_PIN_RCK, GPIO_LOW);

    for(i = HC595_NUMBER_OF_ICS; i > 0 ; i--)
    {
        for(j = 0; j < 8; j++)
//        for(j = 7; j >= 0; j--)
        {
            /* set bit value to the ouput pin */
            Gpio_WriteChannel(HC595_PIN_SI, GET_BIT(IcBuffer[i-1], j));

            /* pull the shift register clock pin high to store bit value from serial-in pin */
            Gpio_WriteChannel(HC595_PIN_SCK, GPIO_HIGH);
            
            /* clear the bit value output pin */
            Gpio_WriteChannel(HC595_PIN_SI, GPIO_LOW);
            
            /* pull the shift register clock pin low */
            Gpio_WriteChannel(HC595_PIN_SCK, GPIO_LOW);
        }
    }
    
    /* transfer the state of the shift register to the output register; pin will held up high 
     * because of timing constraints at higher mcu frequencies
     */
    Gpio_WriteChannel(HC595_PIN_RCK, GPIO_HIGH);
}


/* *********************************************************************************************//**
 * Set the complete buffer array to low level. To assign the values to the physical output pins of
 * the registers additionally IC74HC595_updateOutput(void) has to be called.
 **************************************************************************************************/
void HC595_ClearAll(void)
{
    for(uint8 i = HC595_NUMBER_OF_ICS; i > 0; i--)
    {
        IcBuffer[i-1] = 0x00;
    }
}


/* *********************************************************************************************//**
 * Set the complete buffer array to low level. To assign the values to the physical output pins of
 * the registers additionally IC74HC595_updateOutput(void) has to be called.
 *************************************************************************************************/
void HC595_SetAll(void)
{
    for(uint8 i = HC595_NUMBER_OF_ICS; i > 0 ; i--)
    {
        IcBuffer[i-1] = 0xFF;
    }
}


/* *********************************************************************************************//**
 * This function save a desired pin value to the corresponding data field in the local array buffer.
 * To set the physical output pin to the new state the function IC74HC595_updateOutput() has to be 
 * called additionally.
 * 
 * \param pin   number of the pin to edit [pin = 0...((HC595_NUMBER_OF_ICS * 8) - 1)]
 * \param val   value of the pin  [0 = OFF, 1 = ON]
 **************************************************************************************************/
void HC595_SetBit(const uint8 pin, const uint8 val)
{
    uint8 icNum, pinNum;

    /* evaluate the right IC and the corresponding pin */
    icNum  = (pin >> 3);
    pinNum = (pin & 0x07);
    
    /* first clear the old value then set the new one */
    IcBuffer[icNum] &=  ~(1 << pinNum);
    IcBuffer[icNum] |= (val << pinNum);
}


/* *********************************************************************************************//**
 * Saves a complete register set of data to the local array buffer. To set the physical output pins 
 * to the new state the function IC74HC595_updateOutput() has to be called additionally.
 *
 * \param icNumber  number of the shift register [icNumber = 0...(HC595_NUMBER_OF_ICS - 1); 0 = MSB, n = LSB]
 * \param byteVal   state of the pins in a consecutive byte [0 = OFF, 1 = ON]
 **************************************************************************************************/
void HC595_SetIc(const uint8 icNumber, const uint8 byteVal)
{
    IcBuffer[icNumber] = byteVal;
}


/* *********************************************************************************************//**
 * Returns the value of the shift register array pin given as parameter.
 *
 * \param  pin  number of the pin to read out [pin = 0 ... ((HC595_NUMBER_OF_ICS * 8) - 1)]
 * \retval retVal   current value of the pin buffer  [0 = OFF, 1 = ON]
 **************************************************************************************************/
uint8 HC595_GetBit(const uint8 pin)
{
    uint8 retVal, pinNum;
    
    pinNum = (pin & 0x07);
    
    retVal = ((IcBuffer[pin >> 3] & (1 << pinNum)) >> pinNum);
    
    return(retVal);
}


/* *********************************************************************************************//**
 * Returns the current status of the corresponding IC buffer in the array. The returned values may 
 * differ from the state of the physical output pins.
 *
 * \param  icNumber  number of the requested IC value [icNumber = 0 ... (HC595_NUMBER_OF_ICS - 1)]
 * \returns current value of the IC buffer
 **************************************************************************************************/
uint8 HC595_GetIc(const uint8 icNumber)
{
    return(IcBuffer[icNumber]);
}
