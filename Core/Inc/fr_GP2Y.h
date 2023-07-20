/*
 * GP2Y1010AU0F.h
 *
 *  Created on: 3 maj 2023
 *      Author: Filip
 */

#ifndef INC_FR_GP2Y_H_
#define INC_FR_GP2Y_H_

#include <main.h>
#include "math.h"
#include "fr_global.h"
#include "fr_adc.h"

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim2;

int GP2Y_measure();
void delay(uint32_t time);

#endif /* INC_FR_GP2Y_H_ */
