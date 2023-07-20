/*
 * GP2Y1010AU0F.c
 *      Author: Filip
 */
#include <fr_GP2Y.h>

//mesure data from GP2Y1010AU0F sensor using PA6 pin
int GP2Y_measure(){
	float density, voltage;
	int adcvalue;

	//change ADC to PA6
	ADC_Select_CH6();

	//start measurment with ILED pin
	HAL_GPIO_WritePin(ILED_GPIO_Port, ILED_Pin, GPIO_PIN_SET);

	//delay to take measure
	delay(280);

	//adc measurement
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 1);
	adcvalue = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

	//end measurement
	HAL_GPIO_WritePin(ILED_GPIO_Port, ILED_Pin, GPIO_PIN_RESET);

	//calculate voltage from 1:10 voltage divider
 	voltage = (SYS_VOLTAGE / 4096.0) * adcvalue * 11;

 	//convert measrement to ppm/~(mg/m3)
	if (voltage >= NO_DUST_VOLTAGE)
    {
        voltage -= NO_DUST_VOLTAGE;
        if(voltage>3600){
        	voltage=3600;
        }
        density = voltage * COV_RATIO;
    }
    else{
    	density = 0;
    }

    return floor(density);
}


//us  delay function
void delay(uint32_t time){

	__HAL_TIM_SET_COUNTER(&htim2,0);
	while (TIM2->CNT < time){
	}

}
