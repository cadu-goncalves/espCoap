#ifndef __USER_WIFI_SETTINGS_H__
#define __USER_WIFI_SETTINGS_H__

#include "wifi_module.h"

//#define MY_WIFI_SSID  "Stellaroid"
//#define MY_WIFI_PASSWD  "63KUajN3"
#define MY_WIFI_SSID  "cloudtrak1"
#define MY_WIFI_PASSWD  "dA&zbC4FrT"

//void user_wifi_connect(unsigned char *ssid, unsigned char *passwd);
void user_wifi_connect(struct wifi_configs *ptr);

#endif
