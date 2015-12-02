#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_adc.h"


void InitTempSens()
{

	ADC_InitTypeDef ADC_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;

	ADC_DeInit();

	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	 ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	 ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div8;
	 ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	 ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	 ADC_CommonInit(&ADC_CommonInitStruct);

	 ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	 ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	 ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	 ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	 ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	 ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	 ADC_InitStruct.ADC_NbrOfConversion = 1;
	 ADC_Init(ADC1, &ADC_InitStruct);

	  // ADC1 Configuration, ADC_Channel_TempSensor is actual channel 16
	 ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_144Cycles);

	  // Enable internal temperature sensor
	 ADC_TempSensorVrefintCmd(ENABLE);

	  // Enable ADC conversion
	 ADC_Cmd(ADC1, ENABLE);

}

float GetTemp()
{
	   float TemperatureValue = 0;
	  // ADC Conversion to read temperature sensor
	  // Temperature (in °C) = ((Vsense – V25) / Avg_Slope) + 25
	  // Vense = Voltage Reading From Temperature Sensor
	  // V25 = Voltage at 25°C, for STM32F407 = 0.76V
	  // Avg_Slope = 2.5mV/°C
	  // This data can be found in the STM32F407VF Data Sheet

	   ADC_SoftwareStartConv(ADC1); //Start the conversion
	  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
	   ; //Processing the conversion
	  TemperatureValue = ADC_GetConversionValue(ADC1); //Return the converted data
	  TemperatureValue *= 3300;
	  TemperatureValue /= 0xfff; //Reading in mV
	  TemperatureValue /= 1000.0; //Reading in Volts
	  TemperatureValue -= 0.760; // Subtract the reference voltage at 25°C
	  TemperatureValue /= .0025; // Divide by slope 2.5mV
	  TemperatureValue -= 15.0; // subtract the 15°C

	  return TemperatureValue;
}
