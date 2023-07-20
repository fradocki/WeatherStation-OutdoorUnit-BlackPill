/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
struct kolejka{
	uint16_t etap;
	uint16_t krok;
};

struct polecenie{
	uint16_t time_init;
	char msg[60];
	uint16_t time_response;
	char rsp[20];
	struct kolejka powrot;
	uint16_t time_exit;
	uint8_t iter;
	uint8_t terminate;
};

enum tryb{
	HOTSPOT,
	ROUTER_MODE,
	WIFI_CHANGE,
	RESET_MODE_TO_HOTSPOT,
	RESET_MODE_TO_ROUTER,
	RESET_TO_WIFI_CHANGE
};

enum communication_menage{
	AT_COMMAND_IDLE_WAITING,
	AT_COMMAND_SENDING,
	AT_COMMAND_FIND_RESPONSE,
	AT_COMMAND_WAIT_AND_EXIT
};
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_PC13_Pin GPIO_PIN_13
#define LED_PC13_GPIO_Port GPIOC
#define ILED_Pin GPIO_PIN_0
#define ILED_GPIO_Port GPIOB
#define ESP_RESET_Pin GPIO_PIN_12
#define ESP_RESET_GPIO_Port GPIOB
#define ESP_ENABLE_Pin GPIO_PIN_8
#define ESP_ENABLE_GPIO_Port GPIOA
#define ESP_ENABLEB4_Pin GPIO_PIN_4
#define ESP_ENABLEB4_GPIO_Port GPIOB
#define ESP_RESETB6_Pin GPIO_PIN_6
#define ESP_RESETB6_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define RxBuf_SIZE   1000
#define RxMainBuf_SIZE 1000
#define USART_FLAG_IDLE 0x00000010U
#define COV_RATIO                       0.2            //ug/mmm / mv
#define NO_DUST_VOLTAGE                 1000            //mv
#define SYS_VOLTAGE                     3300
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
