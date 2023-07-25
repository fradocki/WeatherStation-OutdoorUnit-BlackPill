/*
 * adc.h
 *
 *      Author: Filip
 */

#ifndef INC_FR_ADC_H_
#define INC_FR_ADC_H_

#include "main.h"
#include "fr_global.h"

extern ADC_HandleTypeDef hadc1;

void ADC_select_CH4(void);
void ADC_select_CH6(void);

#endif /* INC_FR_ADC_H_ */
