/*
 * fr_ESP8266_messages.c
 *
 *  Created on: Jun 23, 2023
 *      Author: Filip
 */
#include "fr_ESP8266_messages.h"

/*
Device can work in to basic modes in direct communication to central device AP
or thought corresponding AP(wi-fi network)
-load_msg- mode used in indirect communication
-load_msg- mode used in direct communication

There are also two additional modes
-wifi_reset- to reset ESP8266 device, used between changes of working mode and in case of communication failure
-chabge_wifi- to change from direct mode to indirect and to switch between AP's in direct mode
*/

//communication thought local wi-fi network
void load_msg(){

	tab_polecenie[0].time_init= 0;
	sprintf(tab_polecenie[0].msg,"AT+CWMODE=1\r\n");
	tab_polecenie[0].time_response = 4000;
	sprintf(tab_polecenie[0].rsp,"OK\r");
	tab_polecenie[0].powrot.etap =0;
	tab_polecenie[0].powrot.krok =0;
	tab_polecenie[0].time_exit =0;
	tab_polecenie[0].iter =2;
	tab_polecenie[0].terminate =0;

	tab_polecenie[1].time_init=0;
	sprintf(tab_polecenie[1].msg,"AT+CIPMUX=1\r\n");
	tab_polecenie[1].time_response = 4000;
	sprintf(tab_polecenie[1].rsp,"OK\r");
	tab_polecenie[1].powrot.etap = 0;
	tab_polecenie[1].powrot.krok = 0;
	tab_polecenie[1].time_exit = 0;
	tab_polecenie[1].iter =3;
	tab_polecenie[1].terminate =0;

	tab_polecenie[2].time_init=200;
	sprintf(tab_polecenie[2].msg,"AT+CWJAP_CUR=\"%s\",\"%s\"\r\n", wifi_ssid[0], wifi_ssid[1]);
	tab_polecenie[2].time_response = 24000;
	sprintf(tab_polecenie[2].rsp,"OK\r");
	tab_polecenie[2].powrot.etap = 2;
	tab_polecenie[2].powrot.krok = 0;
	tab_polecenie[2].time_exit = 500;
	tab_polecenie[2].iter =3;
	tab_polecenie[2].terminate =0;


	tab_polecenie[3].time_init=0;
	sprintf(tab_polecenie[3].msg,"AT+CIPSTART=0,\"TCP\",\"%s\",80\r\n", wifi_ssid[2]);
	tab_polecenie[3].time_response =10000;
	sprintf(tab_polecenie[3].rsp,"CON");
	tab_polecenie[3].powrot.etap =2;
	tab_polecenie[3].powrot.krok =0;
	tab_polecenie[3].time_exit =0;
	tab_polecenie[3].iter =3;
	tab_polecenie[3].terminate =0;

	tab_polecenie[4].time_init= 8000;
	//sprintf(tab_polecenie[4].msg,"AT+CIPSEND=%d,%i\r\n");
	tab_polecenie[4].time_response = 8000;
	sprintf(tab_polecenie[4].rsp,"OK\r");
	tab_polecenie[4].powrot.etap =2;
	tab_polecenie[4].powrot.krok =0;
	tab_polecenie[4].time_exit =0;
	tab_polecenie[4].iter =2;
	tab_polecenie[4].terminate =0;

	tab_polecenie[5].time_init=0;
	//sprintf(tab_polecenie[5].msg,"%s:W%s:T%s:P\r\n");
	tab_polecenie[5].time_response = 15000;
	sprintf(tab_polecenie[5].rsp,"GIT");
	tab_polecenie[5].powrot.etap = 2;
	tab_polecenie[5].powrot.krok = 0;
	tab_polecenie[5].time_exit = 200;
	tab_polecenie[5].iter =3;
	tab_polecenie[5].terminate =0;

	tab_polecenie[6].time_init=0;
	//sprintf(tab_polecenie[6].msg,"AT+CIPCLOSE=%d\r\n");
	tab_polecenie[6].time_response =6000;
	sprintf(tab_polecenie[6].rsp,"OK\r");
	tab_polecenie[6].powrot.etap =3;
	tab_polecenie[6].powrot.krok =0;
	tab_polecenie[6].time_exit =500;
	tab_polecenie[6].iter =20;
	tab_polecenie[6].terminate =1;

	if(wifi_set){

		tab_polecenie[6].time_init=0;
		sprintf(tab_polecenie[6].msg,"AT+CIPSEND=0,3\r\n");
		tab_polecenie[6].time_response =6000;
		sprintf(tab_polecenie[6].rsp,"OK\r");
		tab_polecenie[6].powrot.etap =3;
		tab_polecenie[6].powrot.krok =0;
		tab_polecenie[6].time_exit =100;
		tab_polecenie[6].iter =1;
		tab_polecenie[6].terminate =0;

		tab_polecenie[7].time_init=0;
		sprintf(tab_polecenie[7].msg,"#GT\r\n");
		tab_polecenie[7].time_response =30000;
		sprintf(tab_polecenie[7].rsp,"OK\r");
		tab_polecenie[7].powrot.etap =2;
		tab_polecenie[7].powrot.krok =0;
		tab_polecenie[7].time_exit =500;
		tab_polecenie[7].iter =2;
		tab_polecenie[7].terminate =1;
	}

}

//direct communication to control unit AP

void load_hotspot(){

 	tab_polecenie[0].time_init= 0;
 	sprintf(tab_polecenie[0].msg,"AT+CWMODE=1\r\n");
 	tab_polecenie[0].time_response = 4000;
 	sprintf(tab_polecenie[0].rsp,"OK\r");
 	tab_polecenie[0].powrot.etap =0;
 	tab_polecenie[0].powrot.krok =0;
 	tab_polecenie[0].time_exit =200;
	tab_polecenie[0].iter =2;
	tab_polecenie[0].terminate =0;

 	tab_polecenie[1].time_init=0;
 	sprintf(tab_polecenie[1].msg,"AT+CIPMUX=1\r\n");
 	tab_polecenie[1].time_response = 4000;
 	sprintf(tab_polecenie[1].rsp,"OK\r");
 	tab_polecenie[1].powrot.etap = 0;
 	tab_polecenie[1].powrot.krok = 0;
 	tab_polecenie[1].time_exit = 200;
	tab_polecenie[1].iter =2;
	tab_polecenie[1].terminate =0;

 	tab_polecenie[2].time_init=2000;
 	sprintf(tab_polecenie[2].msg,"AT+CWJAP_CUR=\"ESP82661\",\"1234567890\"\r\n");
 	tab_polecenie[2].time_response =60000;
 	sprintf(tab_polecenie[2].rsp,"OK\r");
 	tab_polecenie[2].powrot.etap =2;
 	tab_polecenie[2].powrot.krok =0;
 	tab_polecenie[2].time_exit =200;
	tab_polecenie[2].iter =2;
	tab_polecenie[2].terminate =0;

	tab_polecenie[3].time_init=500;
	sprintf(tab_polecenie[3].msg,"AT+CIPSTART=0,\"TCP\",\"192.168.4.1\",80\r\n");
	tab_polecenie[3].time_response =10000;
	sprintf(tab_polecenie[3].rsp,"OK\r");
	tab_polecenie[3].powrot.etap =2;
	tab_polecenie[3].powrot.krok =0;
	tab_polecenie[3].time_exit =0;
	tab_polecenie[3].iter =2;
	tab_polecenie[3].terminate =0;

	tab_polecenie[4].time_init= 200;
	//sprintf(tab_polecenie[4].msg,"AT+CIPSEND=%d,%i\r\n");
	tab_polecenie[4].time_response = 8000;
	sprintf(tab_polecenie[4].rsp,"OK\r");
	tab_polecenie[4].powrot.etap =2;
	tab_polecenie[4].powrot.krok =0;
	tab_polecenie[4].time_exit =0;
	tab_polecenie[4].iter =2;
	tab_polecenie[4].terminate =0;

	tab_polecenie[5].time_init=0;
	//sprintf(tab_polecenie[5].msg,"%s:W%s:T%s:P\r\n");
	tab_polecenie[5].time_response = 15000;
	sprintf(tab_polecenie[5].rsp,"GIT");
	tab_polecenie[5].powrot.etap = 2;
	tab_polecenie[5].powrot.krok = 0;
	tab_polecenie[5].time_exit = 500;
	tab_polecenie[5].iter =2;
	tab_polecenie[5].terminate =0;

	tab_polecenie[6].time_init=0;
	//sprintf(tab_polecenie[6].msg,"AT+CIPCLOSE=%d\r\n");
	tab_polecenie[6].time_response =30000;
	sprintf(tab_polecenie[6].rsp,"OK\r");
	tab_polecenie[6].powrot.etap =2;
	tab_polecenie[6].powrot.krok =0;
	tab_polecenie[6].time_exit =500;
	tab_polecenie[6].iter =2;
	tab_polecenie[6].terminate =1;

	if(wifi_set){
		tab_polecenie[6].time_init=0;
		sprintf(tab_polecenie[6].msg,"AT+CIPSEND=0,3\r\n");
		tab_polecenie[6].time_response =6000;
		sprintf(tab_polecenie[6].rsp,"OK\r");
		tab_polecenie[6].powrot.etap =3;
		tab_polecenie[6].powrot.krok =0;
		tab_polecenie[6].time_exit =100;
		tab_polecenie[6].iter =1;
		tab_polecenie[6].terminate =0;

		tab_polecenie[7].time_init=0;
		sprintf(tab_polecenie[7].msg,"#GT\r\n");
		tab_polecenie[7].time_response =30000;
		sprintf(tab_polecenie[7].rsp,"OK\r");
		tab_polecenie[7].powrot.etap =2;
		tab_polecenie[7].powrot.krok =0;
		tab_polecenie[7].time_exit =500;
		tab_polecenie[7].iter =2;
		tab_polecenie[7].terminate =1;
	}
 }

//used to reset ESP8266 device

void wifi_reset(){
	tab_polecenie[0].time_init= 0;
	sprintf(tab_polecenie[0].msg,"AT+RST\r\n");
	tab_polecenie[0].time_response = 18000;
	sprintf(tab_polecenie[0].rsp,"rea");
	tab_polecenie[0].powrot.etap =0;
	tab_polecenie[0].powrot.krok =0;
	tab_polecenie[0].time_exit =0;
	tab_polecenie[0].iter =2;
	tab_polecenie[0].terminate =1;
}

//communication used to change intermediary AP

void change_wifi(){
	tab_polecenie[0].time_init= 0;
 	sprintf(tab_polecenie[0].msg,"AT+CWMODE=1\r\n");
 	tab_polecenie[0].time_response = 4000;
 	sprintf(tab_polecenie[0].rsp,"OK\r");
 	tab_polecenie[0].powrot.etap =0;
 	tab_polecenie[0].powrot.krok =0;
 	tab_polecenie[0].time_exit =0;
	tab_polecenie[0].iter =2;
	tab_polecenie[0].terminate =0;

 	tab_polecenie[1].time_init=0;
 	sprintf(tab_polecenie[1].msg,"AT+CIPMUX=1\r\n");
 	tab_polecenie[1].time_response = 4000;
 	sprintf(tab_polecenie[1].rsp,"OK\r");
 	tab_polecenie[1].powrot.etap = 0;
 	tab_polecenie[1].powrot.krok = 0;
 	tab_polecenie[1].time_exit = 0;
	tab_polecenie[1].iter =2;
	tab_polecenie[1].terminate =0;

 	tab_polecenie[2].time_init=0;
 	sprintf(tab_polecenie[2].msg,"AT+CWJAP_CUR=\"ESP8266_CHANGE\",\"1234567890\"\r\n");
 	tab_polecenie[2].time_response =40000;
 	sprintf(tab_polecenie[2].rsp,"OK\r");
 	tab_polecenie[2].powrot.etap =2;
 	tab_polecenie[2].powrot.krok =0;
 	tab_polecenie[2].time_exit =0;
	tab_polecenie[2].iter =2;
	tab_polecenie[2].terminate =0;

	tab_polecenie[3].time_init=500;
	sprintf(tab_polecenie[3].msg,"AT+CIPSTART=0,\"TCP\",\"192.168.4.1\",80\r\n");
	tab_polecenie[3].time_response =10000;
	sprintf(tab_polecenie[3].rsp,"OK\r");
	tab_polecenie[3].powrot.etap =2;
	tab_polecenie[3].powrot.krok =0;
	tab_polecenie[3].time_exit =0;
	tab_polecenie[3].iter =2;
	tab_polecenie[3].terminate =0;

	tab_polecenie[4].time_init= 200;
	sprintf(tab_polecenie[4].msg,"AT+CIPSEND=0,3\r\n");
	tab_polecenie[4].time_response = 8000;
	sprintf(tab_polecenie[4].rsp,"OK\r");
	tab_polecenie[4].powrot.etap =2;
	tab_polecenie[4].powrot.krok =0;
	tab_polecenie[4].time_exit =0;
	tab_polecenie[4].iter =2;
	tab_polecenie[4].terminate =0;

	tab_polecenie[5].time_init=0;
	sprintf(tab_polecenie[5].msg,"RDY");
	tab_polecenie[5].time_response = 6000;
	sprintf(tab_polecenie[5].rsp,"RDY");
	tab_polecenie[5].powrot.etap = 2;
	tab_polecenie[5].powrot.krok = 0;
	tab_polecenie[5].time_exit = 0;
	tab_polecenie[5].iter =2;
	tab_polecenie[5].terminate =0;

	tab_polecenie[6].time_init=0;
	sprintf(tab_polecenie[6].msg,"AT\r\n");
	tab_polecenie[6].time_response =30000;
	sprintf(tab_polecenie[6].rsp,"SSI");
	tab_polecenie[6].powrot.etap =2;
	tab_polecenie[6].powrot.krok =0;
	tab_polecenie[6].time_exit =500;
	tab_polecenie[6].iter =2;
	tab_polecenie[6].terminate =0;

	tab_polecenie[7].time_init=0;
	sprintf(tab_polecenie[7].msg,"AT\r\n");
	tab_polecenie[7].time_response =30000;
	sprintf(tab_polecenie[7].rsp,"IPI");
	tab_polecenie[7].powrot.etap =2;
	tab_polecenie[7].powrot.krok =0;
	tab_polecenie[7].time_exit =1000;
	tab_polecenie[7].iter =2;
	tab_polecenie[7].terminate=0;

	tab_polecenie[8].time_init=0;
	sprintf(tab_polecenie[8].msg,"AT+CWJAP_CUR=\"%s\",\"%s\"\r\n", wifi_list[0], wifi_list[1]);
	tab_polecenie[8].time_response =40000;
	sprintf(tab_polecenie[8].rsp,"OK\r");
	tab_polecenie[8].powrot.etap =8;
	tab_polecenie[8].powrot.krok =0;
	tab_polecenie[8].time_exit =1000;
	tab_polecenie[8].iter =2;
	tab_polecenie[8].terminate =0;

	tab_polecenie[9].time_init=500;
	sprintf(tab_polecenie[9].msg,"AT+CIPSTART=1,\"TCP\",\"%s\",80\r\n", wifi_list[2]);
	tab_polecenie[9].time_response =20000;
	sprintf(tab_polecenie[9].rsp,"OK\r");
	tab_polecenie[9].powrot.etap =8;
	tab_polecenie[9].powrot.krok =0;
	tab_polecenie[9].time_exit =0;
	tab_polecenie[9].iter =3;
	tab_polecenie[9].terminate =0;

	tab_polecenie[10].time_init= 200;
	sprintf(tab_polecenie[10].msg,"AT+CIPSEND=1,3\r\n");
	tab_polecenie[10].time_response = 8000;
	sprintf(tab_polecenie[10].rsp,"OK\r");
	tab_polecenie[10].powrot.etap =8;
	tab_polecenie[10].powrot.krok =0;
	tab_polecenie[10].time_exit =0;
	tab_polecenie[10].iter =3;
	tab_polecenie[10].terminate =0;

	tab_polecenie[11].time_init=0;
	sprintf(tab_polecenie[11].msg,"##G");
	tab_polecenie[11].time_response = 6000;
	sprintf(tab_polecenie[11].rsp,"OK\r");
	tab_polecenie[11].powrot.etap = 8;
	tab_polecenie[11].powrot.krok = 0;
	tab_polecenie[11].time_exit = 0;
	tab_polecenie[11].iter =3;
	tab_polecenie[11].terminate =1;
}
