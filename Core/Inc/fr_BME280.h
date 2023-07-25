/*
 * fr_BME280.c
 *
 *      Author: Filip
 */

#ifndef INC_FR_BME280_H_
#define INC_FR_BME280_H_

#include "stm32f4xx_hal.h"
#include "fr_global.h"

// Oversampling definitions
#define OSRS_OFF    	0x00
#define OSRS_1      	0x01
#define OSRS_2      	0x02
#define OSRS_4      	0x03
#define OSRS_8      	0x04
#define OSRS_16     	0x05

// MODE Definitions
#define MODE_FORCED     0x01

// IIR Filter Coefficients
#define IIR_OFF     	0x00

// REGISTERS DEFINITIONS
#define ID_REG      	0xD0
#define RESET_REG  		0xE0
#define CTRL_HUM_REG    0xF2
#define STATUS_REG      0xF3
#define CTRL_MSG_REG   0xF4
#define CONFIG_REG      0xF5
#define PRESS_MSB_REG   0xF7

int BME280_config (uint8_t osrs_t, uint8_t osrs_p, uint8_t osrs_h, uint8_t filter);
void trim_read(void);
void BME280_wake_up(void);
void BME280_measure (void);

#endif /* INC_FR_BME280_H_ */
