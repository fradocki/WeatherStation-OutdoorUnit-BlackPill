/*
 * global.c
 *
 *  Created on: Jun 22, 2023
 *      Author: Filip
 */
#include "fr_global.h"

int dust=0;;
uint8_t iter_wifi_msg=0;
uint32_t tryb_wifi=0;
char str_temp[12];
char str_cis[12];
char str_wil[12];
float Temperature, Pressure, Humidity;
volatile uint8_t RxBuf[RxBuf_SIZE]="\0";
volatile uint8_t MainBuf[RxMainBuf_SIZE];
uint16_t ogon = 0;
uint16_t glowa = 0;
extern DMA_HandleTypeDef hdma_usart1_rx;
char msg[100];
char outputString[100];
char response[50];
char print[50];
char cipsend[50];
char cipclose[50];
uint8_t PomiarFlag=1;
uint16_t i1=0, flaga_szukania=0;

RTC_TimeTypeDef time_get= {0};
RTC_DateTypeDef data_get= {0};

uint8_t wifi_set=0;
uint8_t reset_iter=0;
char wifi_list[3][33]={'\0'};
char wifi_ssid[3][33]={'\0'};

double bat_lvl;
char bat_lvl_lb[20];
uint32_t bat_lvl_int;
uint16_t AD_BAT;
uint32_t bat_lvl_pre;

struct polecenie tab_polecenie [12];
struct kolejka obecny={0,0};





