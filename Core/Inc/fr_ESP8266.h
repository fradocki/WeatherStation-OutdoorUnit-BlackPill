/*
 * ESP8266.h
 *
 *      Author: Filip
 */

#ifndef INC_FR_ESP8266_H_
#define INC_FR_ESP8266_H_

#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fr_global.h"
#include "fr_battery.h"
#include "fr_BME280.h"

#define WiFi_Credentials	"AT+CWJAP=\"NORMAN\",\"uiyuiy121\"\r\n"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern uint32_t seconds;
extern uint8_t buffer[1000];
extern uint16_t buffer_index, timeout, messageHandlerFlag;

void esp_reset();
void esp_server_init();
void transfer_buffer();
void search_wifi(uint16_t i, int ind);
void esp_clear_buffer();
void esp_send(char *, int);
void rx_clear_all();
void choose_next_step();
void send_msg();
void save_ssid(char * ssid, char * pass, char * ip);
void read_ssid();


#endif /* INC_FR_ESP8266_H_ */
