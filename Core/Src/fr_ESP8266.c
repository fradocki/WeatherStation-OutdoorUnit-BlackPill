/*
 * ESP8266.c
 *      Author: Filip
 */


#include <fr_ESP8266.h>

uint32_t seconds = 0;
uint16_t buffer_index = 0, timeout = 0, messageHandlerFlag = 0;
uint16_t indx, kopiuj_buf=0;
uint16_t indy;



void ESP_RESET()
{

	HAL_GPIO_WritePin(ESP_ENABLE_GPIO_Port, ESP_ENABLE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ESP_RESET_GPIO_Port, ESP_RESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(ESP_ENABLE_GPIO_Port, ESP_ENABLE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ESP_RESET_GPIO_Port, ESP_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(200);

}

void ESP_Server_Init()
{

	HAL_Delay(500);
	ESP_RESET();
	HAL_Delay(500);

}

void transfer_bufor(){

	uint16_t i=0;
	  		  	while(((ogon +i)!=glowa)&&(ogon+i)<1000){

	  		  		if(flaga_szukania==1){

	  		  			int a = (ogon+i+1+1000)%1000;
	  		  			int b = (ogon+i+2+1000)%1000;

						if( ((MainBuf[ogon+i])==*(tab_polecenie[obecny.etap].rsp)) || ((MainBuf[ogon+i])=='#') ){

								if((MainBuf[a])==*(tab_polecenie[obecny.etap].rsp+1)){
									if((MainBuf[b])==*(tab_polecenie[obecny.etap].rsp+2)){
										flaga_szukania=2;
									}
								}

								if((MainBuf[a])=='W'){
									if((MainBuf[b])=='F'){
										wifi_set=1;
										obecny.etap=6;
										obecny.krok=0;
									}
								}

						}
	  		  		}

	  		  		if(wifi_set){
						if(kopiuj_buf){
							if((MainBuf[ogon+i]!='"')&&(indx<32)&&(indy<3)){
							wifi_list[indy][indx]=MainBuf[ogon+i];
							indx++;
							}
							else{
								kopiuj_buf=0;
								if(indx>1){
									indy++;
								}
								indx=0;
							}
						}
						szukaj_wifi(i);
	  		  		}
	  		  		i++;
	  		  	}

	  		  	HAL_UART_Transmit_DMA(&huart2, (MainBuf+ogon), i);
				ogon+=i;

}

void szukaj_wifi(uint16_t i){

	uint16_t ind,indmin4,indmin5;

	ind=ogon+i;
	indmin4=(ind-4+1000)%1000;
	indmin5=(ind-5+1000)%1000;

	if((MainBuf[indmin5]=='P')&&(MainBuf[indmin4]=='I')&&(MainBuf[ind]=='"')){
		kopiuj_buf=1;
	}
	if((MainBuf[indmin5]=='S')&&(MainBuf[indmin4]=='I')&&(MainBuf[ind]=='"')){
		kopiuj_buf=1;
	}

}

void ChooseNextStep(){
	if(tryb_wifi==RESET_MODE_TO_HOTSPOT){
		tryb_wifi=HOTSPOT;
		obecny.etap=0;
		obecny.krok=0;
	}
	else if(tryb_wifi==RESET_MODE_TO_ROUTER){
		tryb_wifi=ROUTER_MODE;
		obecny.etap=0;
		obecny.krok=0;
	}
	else if(tryb_wifi==RESET_TO_WIFI_CHANGE){
		tryb_wifi=WIFI_CHANGE;
		obecny.etap=0;
		obecny.krok=0;
	}
	else if(tryb_wifi==WIFI_CHANGE){
		save_ssid(wifi_list[0], wifi_list[1], wifi_list[2]);
		sprintf(wifi_ssid[0],wifi_list[0]);
		sprintf(wifi_ssid[1],wifi_list[1]);
		sprintf(wifi_ssid[2],wifi_list[2]);
		obecny.etap=0;
		obecny.krok=0;
		wifi_set=0;
		tryb_wifi=RESET_MODE_TO_ROUTER;
	}
	else{
		if(wifi_set){
			tryb_wifi=RESET_TO_WIFI_CHANGE;
			obecny.etap=0;
			obecny.krok=0;
		}
		else{
			obecny.etap=0;
			obecny.krok=0;
			PomiarFlag=0;
		}
	}
}

void send_msg(){

	switch(obecny.krok){

		case AT_COMMAND_IDLE_WAITING:
				if(i1==0){
					__HAL_TIM_SET_COUNTER(&htim4,0);
					i1=1;
				}
				if(TIM4->CNT > tab_polecenie[obecny.etap].time_init){
					if(obecny.etap==4){
						measure_battery();
						BME280_Measure();
						dust= GP2Y_measure();
					}
					obecny.krok++;
					i1=0;
				}
		break;

		case AT_COMMAND_SENDING:
					if((USART1->SR) & (USART_SR_TC))
					{
					if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*)tab_polecenie[obecny.etap].msg, strlen(tab_polecenie[obecny.etap].msg))==HAL_OK){
						obecny.krok++;
						flaga_szukania=1;
					}
					else{
						obecny.etap=tab_polecenie[obecny.etap].powrot.etap;
						obecny.krok=tab_polecenie[obecny.krok].powrot.krok;
					}
					}
		break;

		case AT_COMMAND_FIND_RESPONSE:
				if(i1==0){
					__HAL_TIM_SET_COUNTER(&htim4,0);
					i1=1;
				}
				if(flaga_szukania==2){
					obecny.krok++;
					flaga_szukania=0;
				}
				else if(TIM4->CNT > tab_polecenie[obecny.etap].time_response){
					if( ++iter_wifi_msg > tab_polecenie[obecny.etap].iter ){
						iter_wifi_msg=0;
						reset_iter++;
						obecny.etap=tab_polecenie[obecny.etap].powrot.etap;
						obecny.krok=tab_polecenie[obecny.krok].powrot.krok;
						if( (tryb_wifi==ROUTER_MODE) && (reset_iter>25) ){
							tryb_wifi=RESET_MODE_TO_HOTSPOT;
							obecny.etap=0;
							obecny.krok=0;
						}
						else if( (tryb_wifi==ROUTER_MODE) && (reset_iter>10)  ){
							tryb_wifi=RESET_MODE_TO_ROUTER;
							obecny.etap=0;
							obecny.krok=0;
							reset_iter=0;
						}
						else if( (tryb_wifi==HOTSPOT) && (reset_iter>10) ){
							tryb_wifi=RESET_MODE_TO_HOTSPOT;
							obecny.etap=0;
							obecny.krok=0;
							reset_iter=0;
						}
						else if( (tryb_wifi==WIFI_CHANGE) && (reset_iter>10) ){
							tryb_wifi=RESET_MODE_TO_HOTSPOT;
							obecny.etap=0;
							obecny.krok=0;
							reset_iter=0;
							wifi_set=0;
						}
						else{
							obecny.etap=tab_polecenie[obecny.etap].powrot.etap;
							obecny.krok=tab_polecenie[obecny.krok].powrot.krok;
						}
					}
					else{
						obecny.krok=tab_polecenie[obecny.krok].powrot.krok=0;
					}
					flaga_szukania=0;
					i1=0;
				}
		break;

		case AT_COMMAND_WAIT_AND_EXIT:
				if(i1==0){
					__HAL_TIM_SET_COUNTER(&htim4,0);
					i1=1;
				}
				if(TIM4->CNT > tab_polecenie[obecny.etap].time_exit){
					if(tab_polecenie[obecny.etap].terminate==0){
						obecny.krok=0;
						obecny.etap++;
					}
					else{
						ChooseNextStep();
						}
				}
		break;
		}

}

void save_ssid(char * ssid, char * pass, char * ip){

	uint8_t ssid_len= strlen(ssid);
	uint8_t pass_len= strlen(pass);
	uint8_t ip_len= strlen(ip);
	uint8_t mem_ssid[ssid_len+1];
	uint8_t mem_pass[pass_len+1];
	uint8_t mem_ip[ip_len+1];
	memcpy(mem_ssid, ssid, ssid_len);
	memcpy(mem_pass, pass, pass_len);
	memcpy(mem_ip, ip, ip_len);
	uint32_t store = (ssid_len<<16) | pass_len;
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR5, store);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR19, (uint32_t) ip_len);
	for(int i=0; i<((ssid_len+3)/4); i++){
		uint32_t temp = ((mem_ssid[4*i])<<24)|(mem_ssid[4*i+1]<<16)|(mem_ssid[4*i+2]<<8)|(mem_ssid[4*i+3]);
	    HAL_RTCEx_BKUPWrite(&hrtc, (0x06U+(0x01U*i)), temp);
	}
	for(int i=0; i<((pass_len+3)/4); i++){
		uint32_t temp = (mem_pass[4*i]<<24)|(mem_pass[4*i+1]<<16)|(mem_pass[4*i+2]<<8)|(mem_pass[4*i+3]);
	    HAL_RTCEx_BKUPWrite(&hrtc, (0x0CU+(0x01U*i)), temp);
	}
	for(int i=0; i<((ip_len+3)/4); i++){
		uint32_t temp = (mem_ip[4*i]<<24)|(mem_ip[4*i+1]<<16)|(mem_ip[4*i+2]<<8)|(mem_ip[4*i+3]);
	    HAL_RTCEx_BKUPWrite(&hrtc, (0x01U+(0x01U*i)), temp);
	}
	int i=1;

}
void read_ssid(){

	uint32_t store= HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR5);
	uint32_t ssid_len=store>>16;
	uint32_t pass_len=store&0xff;
	uint32_t ip_len= HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR19);
	for(int i=0; i<((ssid_len+3)/4); i++){
		uint32_t temp = HAL_RTCEx_BKUPRead(&hrtc, (0x06U+(0x01U*i)));
		wifi_ssid[0][4*i]=(temp>>24)&0xff;
		wifi_ssid[0][4*i+1]=(temp>>16)&0xff;
		wifi_ssid[0][4*i+2]=(temp>>8)&0xff;
		wifi_ssid[0][4*i+3]=temp&0xff;
	}
	for(int i=0; i<((pass_len+3)/4); i++){
		uint32_t temp = HAL_RTCEx_BKUPRead(&hrtc, (0x0CU+(0x01U*i)));
		wifi_ssid[1][4*i]=(temp>>24)&0xff;
		wifi_ssid[1][4*i+1]=(temp>>16)&0xff;
		wifi_ssid[1][4*i+2]=(temp>>8)&0xff;
		wifi_ssid[1][4*i+3]=temp&0xff;
	}
	for(int i=0; i<((ip_len+3)/4); i++){
		uint32_t temp = HAL_RTCEx_BKUPRead(&hrtc, (0x01U+(0x01U*i)));
		wifi_ssid[2][4*i]=(temp>>24)&0xff;
		wifi_ssid[2][4*i+1]=(temp>>16)&0xff;
		wifi_ssid[2][4*i+2]=(temp>>8)&0xff;
		wifi_ssid[2][4*i+3]=temp&0xff;
	}
	for(int i=ssid_len;i<33;i++){
		wifi_ssid[0][i]='\0';
	}
	for(int i=pass_len;i<33;i++){
		wifi_ssid[1][i]='\0';
	}
	for(int i=ip_len;i<33;i++){
		wifi_ssid[2][i]='\0';
	}

}
