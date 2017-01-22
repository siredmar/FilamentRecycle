#include "R2RDac.h"
#include "Gpio.h"

#define R2DAC_MODE_GPIO_PINS   (1)
#define R2DAC_MODE_HC595       (2)

#define R2RDAC_MODE R2DAC_MODE_HC595

#define R2RDAC_MAXVALUE (255.0f)
#define R2RDAC_SUPPLY_VOLTAGE (5.0f)
#if R2RDAC_MODE == R2DAC_MODE_GPIO_PINS

#define R2RDAC_DEPTH (8u)

static Gpio_ChannelType R2RPins[R2RDAC_DEPTH] =
{
        GPIO_CHANNEL_PD2,
        GPIO_CHANNEL_PD3,
        GPIO_CHANNEL_PD4,
        GPIO_CHANNEL_PD5,
        GPIO_CHANNEL_PC1,
        GPIO_CHANNEL_PC2,
        GPIO_CHANNEL_PC3,
        GPIO_CHANNEL_PC4
};
#else
#include "HC595.h"
#endif

static void R2RDac_SetOutputPins(uint8 Data_ui8);

void R2RDac_Init(void)
{

}

void R2RDac_SetOutputPercentage(float32 OutputPercentage_f32)
{
    float32 Data_f32 = 0.0f;
    Data_f32 = (OutputPercentage_f32 * R2RDAC_SUPPLY_VOLTAGE) / 100;
    R2RDac_SetOutputVoltage(Data_f32);
}

void R2RDac_SetOutputVoltage(float32 OutputVoltage_f32)
{
    uint8 Data_ui8 = 0u;
    Data_ui8 = (uint8)(((R2RDAC_MAXVALUE * OutputVoltage_f32)/R2RDAC_SUPPLY_VOLTAGE));
    R2RDac_SetOutputPins(Data_ui8);
}


static void R2RDac_SetOutputPins(uint8 Data_ui8)
{
#if R2RDAC_MODE == R2DAC_MODE_GPIO_PINS
    uint8 Index_ui8 = 0;
    for(Index_ui8 = 0; Index_ui8 < R2RDAC_DEPTH; Index_ui8++)
    {
        Gpio_WriteChannel(R2RPins[Index_ui8], GET_BIT(Data_ui8, Index_ui8));
    }
#else
    HC595_SetIc(0, ReverseByte(Data_ui8));
    HC595_UpdateOutput();
#endif
}
