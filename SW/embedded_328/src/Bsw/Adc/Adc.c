/* *************************************************************************************************
 * file:        adc.c
 *
 *          The adc module.
 *
 * author:      Armin Schlegel, Mr. L.
 * date:        09.10.2014
 * version:     0.3   worky, testing
 *
 * file history:
 *          09.10.2014  A. Schlegel file created, basic version
 *          14.10.2014  Mr. L.      strutural improvements, nicify layout, add comments
 *
 * notes:
 *      bit7  bit6  bit5  bit4  bit3  bit2  bit1  bit0   register
 *      -----+-----+-----+-----+-----+-----+-----+-----+---------+
 *      ADEN  ADSC  ADATE ADIF  ADIE  ADPS2 ADPS1 ADPS0| ADCSRA
 *      –     ACME  –     –     –     ADTS2 ADTS1 ADTS0| ADCSRB
 *      REFS1 REFS0 ADLAR MUX4  MUX3  MUX2  MUX1  MUX0 | ADMUX
 *
 *
 * todo: ADC: think about how to prescale adc right automatically
 * todo: ADC: what about averaging in ISR? freaky or geeky?
 *
 * copyright:   http://creativecommons.org/licenses/by-nc-sa/3.0/
 **************************************************************************************************/
/* ------------------------------------ INCLUDES ------------------------------------------------ */
#include "Adc.h"
#include "Adc_Lcfg.h"
#include <avr/interrupt.h>

/* ------------------------------------ DEFINES ------------------------------------------------- */

/* ------------------------------------ TYPE DEFINITIONS ---------------------------------------- */

/* ------------------------------------ GLOBAL VARIABLES ---------------------------------------- */

/* ------------------------------------ PRIVATE VARIABLES --------------------------------------- */

static Adc_ConfigType  *AdcConfig;
static Adc_DataType     Adc_Data_s;

static volatile const Adc_RegisterAddressType Adc_RegisterAdresses_as =
{
        (uint8*)  ADC_ADCL_ADDRESS,
        (uint8*)  ADC_ADCH_ADDRESS,
        (uint16*) ADC_ADCW_ADDRESS,
        (uint8*)  ADC_ADCSRA_ADDRESS,
        (uint8*)  ADC_ADCSRB_ADDRESS,
        (uint8*)  ADC_ADMUX_ADDRESS,
        (uint8*)  ADC_DIDR0_ADDRESS
};


/* ------------------------------------ PROTOTYPES ---------------------------------------------- */
/* ------------------------------------ GLOBAL FUNCTIONS ---------------------------------------- */

void Adc_Init(void)
{
    uint8 ChannelCnt = 0;

    Adc_Data_s.CurrentAdcChannel_e = 0;

    for(ChannelCnt = 0; ChannelCnt < ADC_NUMBER_OF_CHANNELS; ChannelCnt++)
    {
        Adc_Data_s.CurrentAdcValue10Bit_ui16[ChannelCnt] = 0;
    }

    AdcConfig = (const Adc_ConfigType*)Adc_GetLcfgData();

    /* enable ADC and set prescaler */
    *(Adc_RegisterAdresses_as.Adc_ControlAndStatusRegisterA_pui8) = \
            (AdcConfig->enableState_e      << ADC_ADEN) | \
            (AdcConfig->prescalerControl_e << ADC_ADPS0);

    /* selecting voltage reference, result alignment and ADC channel */
    *(Adc_RegisterAdresses_as.Adc_MuxRegister_pui8) =  \
            (AdcConfig->referenceControl_e << ADC_REFS0) | \
            (AdcConfig->defaultChannel_e   << ADC_MUX0);

    /* wait for some ADC clock cycles to take the settings */

/* ---------------------------------------------------------------------------------------------- */
    /* dummy read out to discard the invalid first conversion value */
    *(Adc_RegisterAdresses_as.Adc_ControlAndStatusRegisterA_pui8) |= (1 << ADC_ADSC);
    while (!(*(Adc_RegisterAdresses_as.Adc_ControlAndStatusRegisterA_pui8) & (1 << ADC_ADIF)));
    *(Adc_RegisterAdresses_as.Adc_ControlAndStatusRegisterA_pui8) |= (1 << ADC_ADIF);
/* ---------------------------------------------------------------------------------------------- */

    /* set the trigger sources if needed */
    if (AdcConfig->triggerControl_e != ADC_TRIGGER_SINGLE_SHOT)
    {
        *(Adc_RegisterAdresses_as.Adc_ControlAndStatusRegisterB_pui8)  = (AdcConfig->triggerControl_e << ADC_ADTS0);
        *(Adc_RegisterAdresses_as.Adc_ControlAndStatusRegisterA_pui8) |= (1 << ADC_ADATE);
    }

    /* configure adc interrupt */
    *(Adc_RegisterAdresses_as.Adc_ControlAndStatusRegisterA_pui8) |=  (AdcConfig->interruptState_e << ADC_ADIE);
}

void Adc_DisableDigitalInput(const Adc_ChannelType_e channels)
{
    /* disable digital system of given port pin */
    *(Adc_RegisterAdresses_as.Adc_DigitalInputDisableRegister_pui8) = channels;
    AdcConfig->digitalInputDisable_e = channels;
}

void Adc_SetChannel(const Adc_ChannelType_e channel)
{
    uint8 autoTriggerFlag = 0;
    Adc_Data_s.CurrentAdcChannel_e = channel;

    /* disable auto trigger if running */
    if (*(Adc_RegisterAdresses_as.Adc_ControlAndStatusRegisterA_pui8) & (1 << ADC_ADATE))
    {
        autoTriggerFlag = 1;
        *(Adc_RegisterAdresses_as.Adc_ControlAndStatusRegisterA_pui8) &= ~(1 << ADC_ADATE);
    }

    /* wait if a conversion is in progress */
    while(*(Adc_RegisterAdresses_as.Adc_ControlAndStatusRegisterA_pui8) & (1 << ADC_ADSC));

    /* save channel in local config */
    AdcConfig->defaultChannel_e = (Adc_ChannelType_e) (0x07 & channel);

    /* clear and set channel in register */
    *(Adc_RegisterAdresses_as.Adc_MuxRegister_pui8) &= 0xE0;
    *(Adc_RegisterAdresses_as.Adc_MuxRegister_pui8) |= (AdcConfig->defaultChannel_e << ADC_MUX0);

    /* enable auto trigger if previously set */
    if (autoTriggerFlag != 0) {
        *(Adc_RegisterAdresses_as.Adc_ControlAndStatusRegisterA_pui8) |= (1 << ADC_ADATE);
    }
}

uint8 Adc_Read8bit(void)
{
    /* start conversion */
    *(Adc_RegisterAdresses_as.Adc_ControlAndStatusRegisterA_pui8) |= (1 << ADC_ADSC);

    if(AdcConfig->interruptState_e == ADC_INTERRUPT_DISABLED)
    {
        /* wait for end of conversion, fetch adc value and clear the flag */
        while (!(*(Adc_RegisterAdresses_as.Adc_ControlAndStatusRegisterA_pui8) & (1 << ADC_ADIF)));
        Adc_Data_s.CurrentAdcValue10Bit_ui16[Adc_Data_s.CurrentAdcChannel_e] = (*(volatile uint16 *)(Adc_RegisterAdresses_as.Adc_DataRegisterWord_pui16)) & 0x3FF;
        *(Adc_RegisterAdresses_as.Adc_ControlAndStatusRegisterA_pui8) |= (1 << ADC_ADIF);
    }
    else
    {
        /* NOTE: interrupts and callback function must be configured correctly! */
    }

    return (uint8)(Adc_Data_s.CurrentAdcValue10Bit_ui16[Adc_Data_s.CurrentAdcChannel_e] >> 2);
}

uint16 Adc_Read10bit(void)
{
    /* start conversion */
    *(Adc_RegisterAdresses_as.Adc_ControlAndStatusRegisterA_pui8) |= (1 << ADC_ADSC);

    if(AdcConfig->interruptState_e == ADC_INTERRUPT_DISABLED)
    {
        /* wait for end of conversion, fetch adc value and clear the flag */
        while (!(*(Adc_RegisterAdresses_as.Adc_ControlAndStatusRegisterA_pui8) & (1 << ADC_ADIF)));
        Adc_Data_s.CurrentAdcValue10Bit_ui16[Adc_Data_s.CurrentAdcChannel_e] = (*(volatile uint16 *)(Adc_RegisterAdresses_as.Adc_DataRegisterWord_pui16)) & 0x3FF;
        *(Adc_RegisterAdresses_as.Adc_ControlAndStatusRegisterA_pui8) |= (1 << ADC_ADIF);
    }
    else
    {
        /* NOTE: interrupts and callback function must be configured correctly! */
    }

    return Adc_Data_s.CurrentAdcValue10Bit_ui16[Adc_Data_s.CurrentAdcChannel_e];
}

uint16 Adc_Read8bitAverage(void)
{
   uint16 avResult_ui16 = 0;
   uint8 averages_ui8;

   averages_ui8 = AdcConfig->averageControl_e;

   for(uint8 avCnt_ui8 = 0; avCnt_ui8 <= (1 << averages_ui8); avCnt_ui8++)
   {
       avResult_ui16 += Adc_Read8bit();
   }

   avResult_ui16 = (uint16) (avResult_ui16 >> averages_ui8);

   return avResult_ui16;
}

uint16 Adc_Read10bitAverage(void)
{
   uint16 avResult_ui16 = 0;
   uint8 averages_ui8;

   averages_ui8 = AdcConfig->averageControl_e;

   for(uint8 avCnt_ui8 = 0; avCnt_ui8 <= (1 << averages_ui8); avCnt_ui8++)
   {
       avResult_ui16 += Adc_Read10bit();
   }

   avResult_ui16 = (uint16) (avResult_ui16 >> averages_ui8);

   return avResult_ui16;
}


/* ------------------------------------ PRIVATE FUNCTIONS --------------------------------------- */

uint8 Adc_GetResult8bit(void)
{
    return ((Adc_Data_s.CurrentAdcValue10Bit_ui16[Adc_Data_s.CurrentAdcChannel_e] >> 2) & 0xFF);
}

uint16 Adc_GetResult10bit(void)
{
    return Adc_Data_s.CurrentAdcValue10Bit_ui16[Adc_Data_s.CurrentAdcChannel_e];
}


/* ------------------------------------ INTERRUPT SERVICE ROUTINES ------------------------------ */

ISR(ADC_vect)
{
   uint16 adcResult_ui16 = 0;

   Adc_Data_s.CurrentAdcValue10Bit_ui16[Adc_Data_s.CurrentAdcChannel_e] = (*(volatile uint16 *)(Adc_RegisterAdresses_as.Adc_DataRegisterWord_pui16)) & 0x3FF;
   if(AdcConfig->callbackFunc_pv != ADC_CALLBACK_NULL_PTR)
   {
      AdcConfig->callbackFunc_pv(Adc_Data_s.CurrentAdcValue10Bit_ui16[Adc_Data_s.CurrentAdcChannel_e]);
   }
   else
   {
      /* do nothing */
   }
}
/* ************************************ E O F *************************************************** */
