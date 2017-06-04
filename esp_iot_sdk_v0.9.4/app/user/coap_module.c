/*****************************************************
 * author - yoga suhas k.m
 *
 * date - 12 january 2015
 *
 * **************************************************/

#include "osapi.h"
#include "c_string.h"
#include "c_stdlib.h"
#include "c_types.h"
#include "mem.h"
#include "espconn.h"
#include "coap_server.h"
#include "node.h"

coap_queue_t *gQueue = NULL;

coap_packet_t pkt; // received pkt

static void coap_received(void *arg, char *pdata, unsigned short len)
{
  struct espconn *pesp_conn = arg;
  uint8_t buf[MAX_MESSAGE_SIZE+1] = {0}; // +1 for string '\0'
  c_memset(buf, 0, sizeof(buf)); // wipe prev data

  size_t rsplen = coap_server_respond(pesp_conn,pdata, len, buf, MAX_MESSAGE_SIZE+1);
  espconn_sent(pesp_conn, (unsigned char *)buf, rsplen);
}


void coap_create( uint16_t port ) {

  struct espconn *pesp_conn = NULL;

  pesp_conn = NULL;

  // create the espconn struct
  pesp_conn = (struct espconn *)os_zalloc(sizeof(struct espconn));
  pesp_conn->type = ESPCONN_UDP;
  pesp_conn->proto.tcp = NULL;
  pesp_conn->proto.udp = NULL;

  pesp_conn->proto.udp = (esp_udp *)os_zalloc(sizeof(esp_udp));
  pesp_conn->state = ESPCONN_NONE;

  pesp_conn->proto.udp->local_port = port;

  espconn_regist_recvcb((pesp_conn), coap_received);
  espconn_create(pesp_conn);

}

