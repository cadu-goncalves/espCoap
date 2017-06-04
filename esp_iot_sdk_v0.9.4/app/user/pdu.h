#ifndef _PDU_H
#define _PDU_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include "coap.h"

/** Header structure for CoAP PDUs */
typedef struct {
  coap_rw_buffer_t scratch;
  coap_packet_t *pkt;
  coap_rw_buffer_t msg;   /**< the CoAP msg to send */
} coap_pdu_t;

coap_pdu_t ICACHE_FLASH_ATTR *coap_new_pdu(void);

void ICACHE_FLASH_ATTR coap_delete_pdu(coap_pdu_t *pdu);

#ifdef __cplusplus
}
#endif

#endif
