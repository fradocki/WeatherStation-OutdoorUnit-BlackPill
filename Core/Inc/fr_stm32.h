/*
 * fr_stm32.h
 *
 *  Created on: Jun 21, 2023
 *      Author: Filip
 */

#ifndef INC_FR_STM32_H_
#define INC_FR_STM32_H_

#include <main.h>
#include "fr_global.h"



extern uint32_t tryb_wifi;
extern RTC_HandleTypeDef hrtc;
extern RTC_HandleTypeDef hrtc;
extern RTC_TimeTypeDef time_get;
extern RTC_DateTypeDef data_get;

void set_next_alarm(void);
void get_time();
void sleep();

#endif /* INC_FR_STM32_H_ */
