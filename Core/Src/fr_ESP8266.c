/*
 * ESP8266.c
 *      Author: Filip
 */


#include <fr_ESP8266.h>

uint32_t seconds = 0;
uint16_t buffer_index = 0, timeout = 0, messageHandlerFlag = 0;
uint16_t indx, kopiuj_buf=0;
uint16_t indy;


// function to hard reset ESP8266 with GPIO pins
void ESP_RESET()
{

	HAL_GPIO_WritePin(ESP_ENABLE_GPIO_Port, ESP_ENABLE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ESP_RESET_GPIO_Port, ESP_RESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(ESP_ENABLE_GPIO_Port, ESP_ENABLE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ESP_RESET_GPIO_Port, ESP_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(200);

}

// function used to initialize ESP8266 through hard reset
void ESP_Server_Init()
{

	HAL_Delay(500);
	ESP_RESET();
	HAL_Delay(500);

}

/* function used to process and transfer data through MainBuf(Buffer with data copied from RxBuf-
   which was received from ESP8266), buffer is transfered to external UASRT2(PC) */
void transfer_bufor(){

	uint16_t i=0;
	while(((ogon +i)!=glowa)&&(ogon+i)<1000){

		//variables used to search trough MainBuf buffer
		int ind = ogon+i;
        int inda = ((ind-1)+1000)%1000;
        int indb = ((ind-2)+1000)%1000;

        //conditions used to check for positive confirmation of receive of the command
		if(flaga_szukania==1){
			if( (MainBuf[ind]) == *(tab_polecenie[obecny.etap].rsp+2) ){
				if((MainBuf[inda])==*(tab_polecenie[obecny.etap].rsp+1)){
					if((MainBuf[indb])==*(tab_polecenie[obecny.etap].rsp)){
						flaga_szukania=2;
					}
				}
			}
		}

		//conditions used to check for wifi change message
		if((MainBuf[ind])=='#'){
			if((MainBuf[inda])=='F'){
				if((MainBuf[indb])=='W'){
					wifi_set=1;
					obecny.etap=6;
					obecny.krok=0;
				}
			}
		}

		/* code used to search wifi SSID and password in message from ESP8266
		   or to search for IP address of a new server after network in router mode */
		if(wifi_set){
			if(kopiuj_buf){
				if((MainBuf[ind]!='"')&&(indx<32)&&(indy<3)){
				wifi_list[indy][indx]=MainBuf[ind];
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
			search_wifi(i, ind);
		}


		i++;
	}

	HAL_UART_Transmit_DMA(&huart2, (MainBuf+ogon), i);
	ogon+=i;

}

/* function to search wifi SSID and password in message from ESP8266
   or to search for IP address of a new server after network in router mode*/
void search_wifi(uint16_t i, int ind){

	int indmin4, indmin5;

	// calculate previous character position in circular buffer

	indmin4=(ind-4+1000)%1000;
	indmin5=(ind-5+1000)%1000;

	if((MainBuf[indmin5]=='P')&&(MainBuf[indmin4]=='I')&&(MainBuf[ind]=='"')){
		kopiuj_buf=1;
	}
	if((MainBuf[indmin5]=='S')&&(MainBuf[indmin4]=='I')&&(MainBuf[ind]=='"')){
		kopiuj_buf=1;
	}

}

/* The ChooseNextStep function is responsible for managing WiFi working mode.
   At the end of communication sequence, depending on system state variables it will
   maintain same state or switch into other. */

void ChooseNextStep(){

    /* If the system is in the state to reset WiFi to hotspot mode.
       Reset sequence is ended and system switch to hotspot mode. */
	if(tryb_wifi==RESET_MODE_TO_HOTSPOT){

        // update system mode to hotspot.
		tryb_wifi=HOTSPOT;
        // reset the current step and stage in the process.
		obecny.etap=0;
		obecny.krok=0;
	}

    /* If the system is in the state to reset WiFi to router mode.
       Reset sequence is ended and system switch to router mode. */
	else if(tryb_wifi==RESET_MODE_TO_ROUTER){

        // update system mode to router.
		tryb_wifi=ROUTER_MODE;
        // reset the current step and stage in the process
		obecny.etap=0;
		obecny.krok=0;
	}

    /* If the system is in the state to reset WiFi to WiFi change.
       Reset sequence is ended and system start sequence in order to change WiFi. */
	else if(tryb_wifi==RESET_TO_WIFI_CHANGE){

		// update system mode to change WiFi settings
		tryb_wifi=WIFI_CHANGE;
        // reset the current step and stage in the process
		obecny.etap=0;
		obecny.krok=0;

	}
	else if(tryb_wifi==WIFI_CHANGE){

        // update the IP settings, SSID, password and IP address from the read values
		save_ssid(wifi_list[0], wifi_list[1], wifi_list[2]);
		sprintf(wifi_ssid[0],wifi_list[0]);
		sprintf(wifi_ssid[1],wifi_list[1]);
		sprintf(wifi_ssid[2],wifi_list[2]);
        // reset the current step and stage in the process
		obecny.etap=0;
		obecny.krok=0;
        // reset the WiFi set flag
		wifi_set=0;
        // set the system mode to reset to router mode
		tryb_wifi=RESET_MODE_TO_ROUTER;

	}
	else{

        // if the wifi_set flag is set device switch to WiFi change mode through
		if(wifi_set){

            // set the system mode to reset and change wifi settings.
			tryb_wifi=RESET_TO_WIFI_CHANGE;
            // reset the current step and stage in the process.
			obecny.etap=0;
			obecny.krok=0;

		}
		else{

            // reset the current step and stage in the process.
			obecny.etap=0;
			obecny.krok=0;
            // set flag for measuring
			PomiarFlag=0;

		}
	}
}

void send_msg(){

	switch(obecny.krok){

		//step for initial delay given amount of time
		case AT_COMMAND_IDLE_WAITING:

            //start timer to measure initial delay time
			if(i1==0){
				__HAL_TIM_SET_COUNTER(&htim4,0);
				i1=1;
			}

			if(TIM4->CNT > tab_polecenie[obecny.etap].time_init){

				/* in this step measurements are taken not to halt
				   processor during communication sequence
				   measurements are taken before messages are send*/
				if(obecny.etap==0){
					measure_battery();
					BME280_Measure();
					dust= GP2Y_measure();
				}

				// if time delay elapsed switch to next step
				obecny.krok++;
				i1=0;
			}
		break;

		// step to send message to ESP8266 in DMA mode and check for response
		case AT_COMMAND_SENDING:

            // check if USART1 is not busy
			if((USART1->SR) & (USART_SR_TC))
			{

	            /* send message from current communication stage and start searching
	                 for response setting search_flag, switch to next step if communication
	                 was successful */
				if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*)tab_polecenie[obecny.etap].msg, strlen(tab_polecenie[obecny.etap].msg))==HAL_OK){
					obecny.krok++;
					flaga_szukania=1;
				}

                // if communication was not successful back to specified step and stage
				else{
					obecny.etap=tab_polecenie[obecny.etap].powrot.etap;
					obecny.krok=tab_polecenie[obecny.krok].powrot.krok;
				}
			}
		break;

		// stage to find response to current message
		case AT_COMMAND_FIND_RESPONSE:

            //start timer to measure maximum time for response
			if(i1==0){
				__HAL_TIM_SET_COUNTER(&htim4,0);
				i1=1;
			}

            /* check if response was found before time for response elapsed
               if so switch to next step */
			if(flaga_szukania==2){
				obecny.krok++;
				flaga_szukania=0;
			}

            // check if maximum time for response elapsed
			else if(TIM4->CNT > tab_polecenie[obecny.etap].time_response){

                // if message was not found iterate current message specified amount of times
				if( ++iter_wifi_msg > tab_polecenie[obecny.etap].iter ){

                    //reset current message iteration
					iter_wifi_msg=0;
                    //count reset iteration after
					reset_iter++;

                    /* if device is in router mode and reset sequence repeat
                       given amount of times switch to hotspot mode */
					if( (tryb_wifi==ROUTER_MODE) && (reset_iter>25) ){
						tryb_wifi=RESET_MODE_TO_HOTSPOT;
						obecny.etap=0;
						obecny.krok=0;
					}

                    /* if device is in router mode and reset sequence repeat
                       given amount of times reset to the same mode */
					else if( (tryb_wifi==ROUTER_MODE) && (reset_iter>10)  ){
						tryb_wifi=RESET_MODE_TO_ROUTER;
						obecny.etap=0;
						obecny.krok=0;
						reset_iter=0;
					}

                    /* if device is in hotspot mode and reset sequence repeat
                       given amount of times reset to the same mode */
					else if( (tryb_wifi==HOTSPOT) && (reset_iter>10) ){
						tryb_wifi=RESET_MODE_TO_HOTSPOT;
						obecny.etap=0;
						obecny.krok=0;
						reset_iter=0;
					}

                    /* if device is in router change mode and reset sequence repeat
                       given amount of times reset to the same mode */
					else if( (tryb_wifi==WIFI_CHANGE) && (reset_iter>10) ){
						tryb_wifi=RESET_MODE_TO_HOTSPOT;
						obecny.etap=0;
						obecny.krok=0;
						reset_iter=0;
						wifi_set=0;
					}

					// if communication was not successful back to specified step and stage
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

		/* last step for each message, delay specified amount of time and do one of below
		   -switch to next stage
		   -end communication if it was last message
		   -change working mode if conditions are meet
		   */
		case AT_COMMAND_WAIT_AND_EXIT:

			//start timer to measure exit delay time
			if(i1==0){
				__HAL_TIM_SET_COUNTER(&htim4,0);
				i1=1;
			}

			//if it's not last message, switch to next stage
			if(TIM4->CNT > tab_polecenie[obecny.etap].time_exit){
				if(tab_polecenie[obecny.etap].terminate==0){
					obecny.krok=0;
					obecny.etap++;
				}

				// if it's last message choose next step with dedicated function
				else{
					ChooseNextStep();
					}
			}
		break;
		}

}

/* This function writes WiFi credentials and connection setting
 * to a backup domain in the RTC peripheral.
 */
void save_ssid(char * ssid, char * pass, char * ip){

    // determine the lengths of the SSID, password and IP address
	uint8_t ssid_len= strlen(ssid);
	uint8_t pass_len= strlen(pass);
	uint8_t ip_len= strlen(ip);

    // create memory buffers to hold the SSID, password and IP address
	uint8_t mem_ssid[ssid_len+1];
	uint8_t mem_pass[pass_len+1];
	uint8_t mem_ip[ip_len+1];

    // create a 32-bit value to hold the lengths of the SSID and password, and the WiFi mode.
    // this value will be stored in the backup register RTC_BKP_DR5
	memcpy(mem_ssid, ssid, ssid_len);
	memcpy(mem_pass, pass, pass_len);
	memcpy(mem_ip, ip, ip_len);

	// create a 32-bit value to hold the lengths of the SSID and password
	uint32_t store = (ssid_len<<16) | pass_len;
    // this value will be stored in the backup register RTC_BKP_DR5
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR5, store);
    //store ip length in backup register
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR19, (uint32_t) ip_len);

    // store the SSID in the backup registers, each character is 8 bit value
	for(int i=0; i<((ssid_len+3)/4); i++){
		uint32_t temp = ((mem_ssid[4*i])<<24)|(mem_ssid[4*i+1]<<16)|(mem_ssid[4*i+2]<<8)|(mem_ssid[4*i+3]);
	    HAL_RTCEx_BKUPWrite(&hrtc, (0x06U+(0x01U*i)), temp);
	}

    // store the password in the backup registers, each character is 8 bit value
	for(int i=0; i<((pass_len+3)/4); i++){
		uint32_t temp = (mem_pass[4*i]<<24)|(mem_pass[4*i+1]<<16)|(mem_pass[4*i+2]<<8)|(mem_pass[4*i+3]);
	    HAL_RTCEx_BKUPWrite(&hrtc, (0x0CU+(0x01U*i)), temp);
	}

    // store the ip address in the backup registers, each character is 8 bit value
	for(int i=0; i<((ip_len+3)/4); i++){
		uint32_t temp = (mem_ip[4*i]<<24)|(mem_ip[4*i+1]<<16)|(mem_ip[4*i+2]<<8)|(mem_ip[4*i+3]);
	    HAL_RTCEx_BKUPWrite(&hrtc, (0x01U+(0x01U*i)), temp);
	}

}

/*
 * This function reads WiFi credentials and connection setting
 * from a backup domain in the RTC peripheral.
 */
void read_ssid(){

    /* Read the value from the backup register RTC_BKP_DR5.
       This value contains the lengths of the SSID and the password, and the WiFi mode. */
	uint32_t store= HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR5);

	// extract the length of the SSID, the password and IP address
	uint32_t ssid_len=store>>16;
	uint32_t pass_len=store&0xff;
	uint32_t ip_len= HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR19);

    // read the SSID from the backup registers, each character is 8 bit value
	for(int i=0; i<((ssid_len+3)/4); i++){
		uint32_t temp = HAL_RTCEx_BKUPRead(&hrtc, (0x06U+(0x01U*i)));
		wifi_ssid[0][4*i]=(temp>>24)&0xff;
		wifi_ssid[0][4*i+1]=(temp>>16)&0xff;
		wifi_ssid[0][4*i+2]=(temp>>8)&0xff;
		wifi_ssid[0][4*i+3]=temp&0xff;
	}

    // read the password from the backup registers, each character is 8 bit value
	for(int i=0; i<((pass_len+3)/4); i++){
		uint32_t temp = HAL_RTCEx_BKUPRead(&hrtc, (0x0CU+(0x01U*i)));
		wifi_ssid[1][4*i]=(temp>>24)&0xff;
		wifi_ssid[1][4*i+1]=(temp>>16)&0xff;
		wifi_ssid[1][4*i+2]=(temp>>8)&0xff;
		wifi_ssid[1][4*i+3]=temp&0xff;
	}

    // read the password from the backup registers, each character is 8 bit value
	for(int i=0; i<((ip_len+3)/4); i++){
		uint32_t temp = HAL_RTCEx_BKUPRead(&hrtc, (0x01U+(0x01U*i)));
		wifi_ssid[2][4*i]=(temp>>24)&0xff;
		wifi_ssid[2][4*i+1]=(temp>>16)&0xff;
		wifi_ssid[2][4*i+2]=(temp>>8)&0xff;
		wifi_ssid[2][4*i+3]=temp&0xff;
	}

	//add null characters and the end of tables for reliability
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
