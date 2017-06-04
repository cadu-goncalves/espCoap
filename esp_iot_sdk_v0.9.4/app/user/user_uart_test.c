/**********************************************************
 *
 * author - yoga suhas k.m
 *
 * date - 12 january 2015
 *
 * ********************************************************/

#include "user_uart_test.h"
#include "driver/uart.h"
#include "endpoints.h"

uint8_t data_array[MAX_DATA_SIZE];
uint8_t data_size = 0;
uint8_t rx_count = 0;
uint8_t rx_state = 0, rxd_data = 0;

void user_uart_init() {
 rx_state = RX_SOP;
}

void rxd_cb(uint8_t data) {

    rxd_data = data;
    switch(rx_state) {

      case RX_SOP:
      if(rxd_data == START_OF_PKT) {
         rx_state = RX_DATA;
         data_size = 0;
         rx_count = 0;
      } else {
         rx_state = RX_NULL;
         rx_count = 0;
      }
      break;
      case RX_DATA:
      if(rxd_data != END_OF_PKT) {
        data_array[rx_count++] = rxd_data;
        data_size = rx_count;
      } else {
         rx_state = RX_SOP;
         rx_count = 0;
         uart0_tx_buffer(data_array,data_size);
         uart0_tx_buffer("\r\n",2);
         update_sensor_value(data_array, data_size);
      }
      break;
      case RX_NULL:
         rx_state = RX_SOP;
         data_size = 0;
         rx_count = 0;
      break;

    }
}
