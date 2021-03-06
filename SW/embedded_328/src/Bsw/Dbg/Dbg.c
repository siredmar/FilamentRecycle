/*
 * Dbg.c
 *
 *  Created on: Oct 24, 2015
 *      Author: armin
 */
#include "Std_Types.h"
#include "Uart.h"
#include "Dbg.h"
#include <avr/io.h>

#define DBG_ENABLE

static void Dbg_HexToString(uint8* str, uint8 len, uint8 val)
{
#ifdef DBG_ENABLE
    uint8 i;

  for(i=1; i<=len; i++)
  {
      uint8 byte = (uint8)(val & 0xF);

    if(byte < 0xA)
    {
      str[len-i] = (uint8)(byte + '0');
    }
    else
    {
      byte -= 0xA;
      str[len-i] = (uint8)(byte + 'A');
    }
    val >>= 4;
  }

  str[i-1] = '\0';
#endif
}


void Dbg_ReadRegister(uint8 UartHwUnit, uint8 str[], uint8 address)
{
#ifdef DBG_ENABLE
    uint8 registerString[8];
    uint8 registerValue = 0;
    if(address != NULL)
    {
        registerValue = (*((uint8*)address));
    }
    Dbg_HexToString(registerString, 2, registerValue);
    Uart_WriteString(UartHwUnit, "Dbg_ReadRegister: ");
    Uart_WriteString(UartHwUnit, str);
    Uart_WriteString(UartHwUnit, "0x");
    Uart_WriteString(UartHwUnit, registerString);
    Uart_WriteString(UartHwUnit, "\n\r");
#endif
}


void Dbg_ReadVariableInteger(uint8 str[], sint32 variable)
{
#ifdef DBG_ENABLE
    uint8 string[30];
    sprintf(string, "%10li", (sint32)variable);
    //itoa(variable, string, 10);
    Uart_WriteString(UART_HWUNIT_0, str);
    Uart_WriteString(UART_HWUNIT_0, string);
    Uart_WriteString(UART_HWUNIT_0, "\n\r");
#endif
}

void Dbg_ReadVariableIntegerUnsigned(uint8 str[], uint32 variable)
{
#ifdef DBG_ENABLE
    uint8 string[30];
    sprintf(string, "%10ld", (uint32)variable);
    //itoa(variable, string, 10);
    Uart_WriteString(UART_HWUNIT_0, str);
    Uart_WriteString(UART_HWUNIT_0, string);
    Uart_WriteString(UART_HWUNIT_0, "\n\r");
#endif
}

void Dbg_ReadVariableFloat(uint8 str[], float32 variable)
{
#ifdef DBG_ENABLE
    uint8 string[30];
    sprintf(string, " %.2f", (float32)variable);
    Uart_WriteString(UART_HWUNIT_0, str);
    Uart_WriteString(UART_HWUNIT_0, string);
    Uart_WriteString(UART_HWUNIT_0, "\n\r");
#endif
}
