/*
 * adc.c
 *
 *  Created on: Jun 13, 2023
 *      Author: Filip
 */

#include <fr_adc.h>

void ADC_Select_CH4(void)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	  // Configure for the selected ADC
	  sConfig.Channel = ADC_CHANNEL_4;
	  sConfig.Rank = 1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

void ADC_Select_CH6(void)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	  // Configure for the selected ADC
	  sConfig.Channel = ADC_CHANNEL_6;
	  sConfig.Rank = 1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}



