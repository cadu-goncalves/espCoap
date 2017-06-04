/*****************************************************************************************
* coap project
* implementation - user_wifi.c - to connect the wifi network
*                  user_uart_test.c - uart based resource which is observable
*                  coap_module.c - coap server app
*                  endpoints.c - all resource implementation
* used tools - Aneska android based coap client
*              copper plugin of firefox
*              wireshark Version 2.0.2
*
* Tested on esp8266-12 module
*
* Used sdk version - esp_iot_sdk_v0.9.4
*
* This project adresses the coap and observe option implementation.
*
* I have tried to port the coap stack form nodemcu to espressif sdk.
* I wont have any copyright on the code, its open for the use and 
* modification. I present dew credits to nodemcu and espressif.
* I have not used this for any commercial purpose.
*
*
* In this project, concentrated mainly on coap server implementation and observe option
******************************************************************************************/

Notes on the project

1. Started a "test" wifi network using android smart phone
2. in "user_wifi.h" file the wifi networks SSID and PASSWORD has been mentioned ( i used "test" as ssid and "klmnorp" as password, you can change set according to yours)
3. after connecting to wifi, port 5683 is opend to listen any packet for coap
4. implemented resources (check out in endpoints.c)
    1. .well-known/core
    2. led/on
    3. led/off
    4. uart/test
    5. device/status
5. uart/test is a observable resource with NON-confirmable message
6. all other resource uses confirmable message pattern
7. uart_test.c is like dummy stack for uart based sensors, used a pattern such that data is enclosed between two "a"
   eg: from uart send data " a123456789a" our data is 123456789 which will be sent to client if request came.
8. single client support is given for obersvable resource, multiple client support is yet to be done.


procedure to follow

used ubuntu platform for development and testing

1. install xtensa toolchain and flashing python tool
2. comiple the code using ./gen_misc.sh command
3. dump the bin file using ./esptool.py --port /dev/ttyUSB0 write_flash 0x00000 ~/esp_iot_sdk_v0.9.4/app/.output/eagle/debug/image/eagle.app.v6.flash.bin 0x40000 ~/esp_iot_sdk_v0.9.4/app/.output/eagle/debug/image/eagle.app.v6.irom0text.bin
4. open the minicom, set baudrate to 115200 and enable ascii display
5. enable hotspot in android phone with SSID "test" and passowrd "klmnopqr"
6. when esp8266 module is powerd up, it will connect to wifi network with dhcp hostname "iot-coap"
7. open "Aneska" app on your android (you can download it form google play for free)
8. in url bar type "coap://iot-coap:5683/.well-known/core" and send the request
9. check the ack, it will contain all the resource serving by that server
10. you can turn on led by sending "coap://iot-coap:5683/led/on" connect led or check voltage at gpio12 of esp8266 module
11. you can turn off led by sending "coap://iot-coap:5683/led/off" check voltage or led gets off at gpio12 of esp8266 module
12. for observe option, use uart/test resource. you can send only get request or else include observe option into message.
    in minicom type "aqwerty1234a" (uart_test stack built as considering data between two "a", on client in ack you can see
    "qwerty1234". can send any number data, just type "asdfghjka", will get "sdfghjk" on your client window.
   
    to send observe request send "coap:iot-coap:5683/uart/test" and select observe option.
    If you have not selected the observe option then it will be mere get request.

13. to check the device status, whether it is alive or dead in network, send "coap://iot-coap:5683/device/status"
    you will receive "alive" in ack. if ack doesnt come then device should be dead.



