#include "Heating.h"
#include "Adc.h"

#define HEATING_NTC_ADC_CHANNEL (ADC_CHANNEL_0)
#define HEATING_NTC_RESISTOR (100000)
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25
// resistance at 25 degrees C
#define THERMISTORNOMINAL 100000

static Heating_DataType Heating_Data_s;

void Heating_Init(void)
{
    Heating_Data_s.ActualTemperature_f32 = 0.0f;
    Heating_Data_s.TargetTemperature_f32 = 0.0f;
    Heating_Data_s.TemperatureAdcValue_ui16;
}

void Heating_Handler(void)
{
    Adc_SetChannel(ADC_CHANNEL_0);
    Heating_Data_s.TemperatureAdcValue_ui16 = Adc_Read10bitAverage();
    Heating_Data_s.TemperatureResistorValue_f32 = 1023 / (float32)Heating_Data_s.TemperatureAdcValue_ui16 - 1;
    Heating_Data_s.TemperatureResistorValue_f32 = HEATING_NTC_RESISTOR / Heating_Data_s.TemperatureResistorValue_f32;
    Heating_Data_s.ActualTemperature_f32 = Heating_Data_s.TemperatureResistorValue_f32 / THERMISTORNOMINAL;     // (R/Ro)
    Heating_Data_s.ActualTemperature_f32 = log(Heating_Data_s.ActualTemperature_f32);                  // ln(R/Ro)
    Heating_Data_s.ActualTemperature_f32 /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
    Heating_Data_s.ActualTemperature_f32 += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
    Heating_Data_s.ActualTemperature_f32 = 1.0 / Heating_Data_s.ActualTemperature_f32;                 // Invert
    Heating_Data_s.ActualTemperature_f32 -= 273.15;                         // convert to C

    Heating_OutputValue();
}

void Heating_OutputValue(void)
{
    Dbg_ReadVariableIntegerUnsigned("Adc: ", (uint32)(Heating_Data_s.TemperatureAdcValue_ui16 & 0xFFFF));
    Dbg_ReadVariableFloat("Res: ", Heating_Data_s.TemperatureResistorValue_f32);
    Dbg_ReadVariableFloat("Cel: ", Heating_Data_s.ActualTemperature_f32);
}

void Heating_SetTemperature(float32 Temp_f32)
{

}

float32 Heating_GetTemperature(void)
{
    return Heating_Data_s.ActualTemperature_f32;
}




