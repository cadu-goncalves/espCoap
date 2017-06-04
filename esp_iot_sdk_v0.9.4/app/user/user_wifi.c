/***************************************
 *
 * author - yoga suhas k.m.
 * date - 12 january 2015
 *
 * To join into the given wifi network
 * *************************************/


#include "ets_sys.h"
#include "osapi.h"
//#include "driver/uart.h"
#include "user_interface.h"
#include "coap_module.h"
#include "user_wifi.h"

ETSTimer ptimer;
struct station_config wificonfig;
struct softap_config config;

void onSpiData(uint8_t *data, uint8_t len){
  coap_create(5683);
}



void user_wifi_connect() {
  os_memcpy(&wificonfig.ssid, MY_WIFI_SSID, os_strlen(MY_WIFI_SSID));
  os_memcpy(&wificonfig.password, MY_WIFI_PASSWD, os_strlen(MY_WIFI_PASSWD));
  
  wificonfig.bssid_set = 0;
  wifi_set_opmode(0x01);  //STATION MODE
  

  wifi_station_set_config(&wificonfig);
  
  os_timer_setfn(&ptimer, (os_timer_func_t *)onSpiData, NULL);
  os_timer_arm(&ptimer, 3000, 0);
}

