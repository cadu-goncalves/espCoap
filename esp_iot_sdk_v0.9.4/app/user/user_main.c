/******************************************************************************
 * author - yoga suhas K.M
 *
*******************************************************************************/
#include "ets_sys.h"
#include "osapi.h"
#include "driver/uart.h"
#include "user_interface.h"
#include "user_plug.h"


void user_init(void)
{
  uart_init(BIT_RATE_115200, BIT_RATE_115200);
  PIN_FUNC_SELECT(PLUG_LINK_LED_IO_MUX, PLUG_LINK_LED_IO_FUNC);
  gpio_output_set(0, BIT12, BIT12, 0); // to set GPIO12 low
  user_wifi_connect();  
  user_uart_init();
}

