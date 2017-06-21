#include "Heating.h"
#include "Adc.h"
#include <avr/io.h>
#include <avr/pgmspace.h>

#define HEATING_NTC_ADC_CHANNEL (ADC_CHANNEL_0)
// resistance at 25 degrees C
#define THERMISTORNOMINAL 126000.0
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 21.0
// how many samples to take and TempCelsius, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950.0
// the value of the 'other' resistor
#define SERIESRESISTOR 4700.0

Heating_DataType Heating_Data_s;

void Heating_Init(void)
{
    Heating_Data_s.ActualTemperature_f32 = 0.0f;
    Heating_Data_s.TargetTemperature_f32 = 0.0f;
    Heating_Data_s.TemperatureAdcValue_ui16;
}

void Heating_Handler(void)
{
    Adc_SetChannel(HEATING_NTC_ADC_CHANNEL);
    Heating_Data_s.TemperatureAdcValue_ui16 = Adc_Read10bit();
    Heating_CalculateTemperature();
//    Heating_OutputValue();
}

void Heating_OutputValue(void)
{
//    Dbg_ReadVariableIntegerUnsigned("Adc: ", (uint32)(Heating_Data_s.TemperatureAdcValue_ui16 & 0xFFFF));
//    Dbg_ReadVariableFloat("Res: ", Heating_Data_s.TemperatureResistorValue_f32);
    Dbg_ReadVariableFloat("Temperature [C]:\t", Heating_Data_s.ActualTemperature_f32);
//    Lcd_PrintFloat(Heating_Data_s.ActualTemperature_f32, 12, 3);
}

void Heating_SetTemperature(float32 Temp_f32)
{

}

float32 Heating_GetTemperature(void)
{
    return Heating_Data_s.ActualTemperature_f32;
}

void Heating_CalculateTemperature(void)
{
     float TempCelsius = 0.0;
     float steinhart = 0.0;
     // convert the value to resistance
     TempCelsius = 1023.0 / (float)(Heating_Data_s.TemperatureAdcValue_ui16) - 1.0;
     TempCelsius = SERIESRESISTOR / TempCelsius;

     steinhart = TempCelsius / THERMISTORNOMINAL;     // (R/Ro)
     steinhart = log(steinhart);                  // ln(R/Ro)
     steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
     steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
     steinhart = 1.0 / steinhart;                 // Invert
     steinhart -= 273.15;                         // convert to C
     Heating_Data_s.ActualTemperature_f32 = steinhart;
}
