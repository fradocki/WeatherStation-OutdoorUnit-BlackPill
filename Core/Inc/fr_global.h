/*
 * global.h
 *
 *  Created on: Jun 22, 2023
 *      Author: Filip
 */
#include <main.h>

extern int dust;;
extern uint8_t iter_wifi_msg;
extern uint32_t tryb_wifi;
extern char str_temp[12];
extern char str_cis[12];
extern char str_wil[12];
extern float Temperature, Pressure, Humidity;
extern volatile uint8_t RxBuf[RxBuf_SIZE];
extern volatile uint8_t MainBuf[RxMainBuf_SIZE];
extern uint16_t ogon;
extern uint16_t glowa;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern char msg[100];
extern char outputString[100];
extern char response[50];
extern char print[50];
extern char cipsend[50];
extern char cipclose[50];
extern uint8_t PomiarFlag;
extern uint16_t i1, flaga_szukania;

extern RTC_TimeTypeDef time_get;
extern RTC_DateTypeDef data_get;

extern uint8_t wifi_set;
extern uint8_t reset_iter;
extern char wifi_list[3][33];
extern char wifi_ssid[3][33];

extern double bat_lvl;
extern char bat_lvl_lb[20];
extern uint32_t bat_lvl_int;
extern uint16_t AD_BAT;
extern uint32_t bat_lvl_pre;



extern ADC_HandleTypeDef hadc1;

extern I2C_HandleTypeDef hi2c1;

extern RTC_HandleTypeDef hrtc;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart2_tx;

extern struct polecenie tab_polecenie [12];
extern struct kolejka obecny;
