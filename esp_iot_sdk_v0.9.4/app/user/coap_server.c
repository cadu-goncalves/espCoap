#include "user_config.h"
#include "c_types.h"
#include "c_stdlib.h"

#include "coap.h"

size_t coap_server_respond(struct espconn *espconn, char *req, unsigned short reqlen, char *rsp, unsigned short rsplen)
{
  size_t rlen = rsplen;
  coap_packet_t pkt;
  pkt.content.p = NULL;
  pkt.content.len = 0;
  uint8_t scratch_raw[4];
  coap_rw_buffer_t scratch_buf = {scratch_raw, sizeof(scratch_raw)};
  int rc;

  if (0 != (rc = coap_parse(&pkt, req, reqlen))){
    return 0;
  }
  else
  {
    coap_packet_t rsppkt;
    rsppkt.content.p = NULL;
    rsppkt.content.len = 0;
    coap_handle_req(espconn,&scratch_buf, &pkt, &rsppkt);
    if (0 != (rc = coap_build(rsp, &rlen, &rsppkt))){
      rlen = 0;
    }
    if(rsppkt.content.p){
      os_free(rsppkt.content.p);
      rsppkt.content.p = NULL;
      rsppkt.content.len = 0;
    }
    return rlen;
  }
}
