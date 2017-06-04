/*********************************************
 * author - yoga suhas k.m.
 * date - 12 january 2014
 *
 * ******************************************/


#include "c_string.h"
#include "c_stdlib.h"
#include "coap.h"
#include "user_uart_test.h"
#include "node.h"
#include "coap_timer.h"
#include "coap_io.h"
#include "os_type.h"

uint8_t obs_data[MAX_DATA_SIZE],obs_data_size,i = 0;
obs_strt wgm_obs;
uint8_t obs_req_weg = 0;
struct espconn *send_rsp_pesp_conn; // endpoint udp info to be send
coap_packet_t pkt;
uint8_t msg_id_msb = 0x23;
uint8_t msg_id_lsb = 0x56;
uint8_t buf[MAX_MESSAGE_SIZE+1] = {0}; // +1 for string '\0' , post build coap message
unsigned int bufLen = MAX_MESSAGE_SIZE+1;


void build_well_known_rsp(char *rsp, uint16_t rsplen);

void endpoint_setup(void)
{
    coap_setup();
}

static const coap_endpoint_path_t path_well_known_core = {2, {".well-known", "core"}};
static int handle_get_well_known_core(struct espconn *espconn,const coap_endpoint_t *ep, coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo,uint8_t obs)
{
    outpkt->content.p = (uint8_t *)os_zalloc(MAX_PAYLOAD_SIZE);      // this should be free-ed when outpkt is built in coap_server_respond()
    if(outpkt->content.p == NULL){
        return COAP_ERR_BUFFER_TOO_SMALL;
    }
    outpkt->content.len = MAX_PAYLOAD_SIZE;
    build_well_known_rsp(outpkt->content.p, outpkt->content.len);
    return coap_make_response(scratch, outpkt, (const uint8_t *)outpkt->content.p, c_strlen(outpkt->content.p), id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_APPLICATION_LINKFORMAT);
}

static const coap_endpoint_path_t path_id = {2, {"led", "on"}};
static int handle_get_id(struct espconn *espconn,const coap_endpoint_t *ep, coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo, uint8_t obs)
{
    gpio_output_set(BIT12, 0, BIT12, 0);
    return coap_make_response(scratch, outpkt, (const uint8_t *)NULL, NULL, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_TEXT_PLAIN);
}

static const coap_endpoint_path_t path_id_1 = {2, {"led", "off"}};
static int handle_get_id_1(struct espconn *espconn,const coap_endpoint_t *ep, coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo,uint8_t obs)
{
    gpio_output_set(0,BIT12, 0, BIT12);

    return coap_make_response(scratch, outpkt, (const uint8_t *)NULL, NULL, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_TEXT_PLAIN);
}

void send_rsp() {
  static int firstTime_exec = 0;

  if(firstTime_exec == 0) {
    send_rsp_pesp_conn = NULL;
    send_rsp_pesp_conn = (struct espconn *)os_zalloc(sizeof(struct espconn));
    send_rsp_pesp_conn->proto.udp = (esp_udp *)os_zalloc(sizeof(esp_udp));
    send_rsp_pesp_conn->type = ESPCONN_UDP;
    send_rsp_pesp_conn->state = ESPCONN_NONE;
    firstTime_exec++;
  }
  coap_pdu_t *pdu = coap_new_pdu();

  coap_make_response_obs (&(pdu->scratch), pdu->pkt, COAP_TYPE_NONCON ,(const uint8_t*) obs_data,(size_t) obs_data_size, (uint8_t) msg_id_msb++, (uint8_t) msg_id_lsb++, (uint8_t) wgm_obs.tkl,(uint8_t*) &(wgm_obs.tkn), COAP_RSPCODE_CONTENT,COAP_CONTENTTYPE_TEXT_PLAIN);
  coap_build(pdu->msg.p, &(pdu->msg.len), pdu->pkt);
  send_rsp_pesp_conn->proto.udp->remote_port = wgm_obs.port;
  send_rsp_pesp_conn->proto.udp->local_port = espconn_port();
  for(i=0;i<4;i++){
    send_rsp_pesp_conn->proto.udp->remote_ip[i] = wgm_obs.ip[i];
  }
  send_rsp_pesp_conn->type = ESPCONN_UDP;
  send_rsp_pesp_conn->state = ESPCONN_NONE;
  sint8_t con = espconn_create(send_rsp_pesp_conn);

  coap_tid_t tid = COAP_INVALID_TID;
  if (pdu->pkt->hdr.t == COAP_TYPE_CON){
     tid = coap_send_confirmed(send_rsp_pesp_conn, pdu);
  }else {
     tid = coap_send(send_rsp_pesp_conn, pdu);
  }

  if (pdu->pkt->hdr.t != COAP_TYPE_CON || tid == COAP_INVALID_TID){
    coap_delete_pdu(pdu);
  }
}

void update_sensor_value( uint8_t *data, uint8_t len) {
  for(i = 0; i < len; i++ )
      obs_data[i] = data[i];
  obs_data_size = len;
  if (obs_req_weg) 
    send_rsp();
}

static const coap_endpoint_path_t path_id_2 = {2, {"uart","test"}};
static int handle_get_id_2(struct espconn *espconn,const coap_endpoint_t *ep, coap_rw_buffer_t *scratch,const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo,uint8_t obs){
    if (obs == 0) 
      return coap_make_response(scratch, outpkt, (const uint8_t *)obs_data, obs_data_size, id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_TEXT_PLAIN);
    else{
      wgm_obs.port = espconn->proto.udp->remote_port;
      for(i=0;i<4;i++){
        wgm_obs.ip[i] = espconn->proto.udp->remote_ip[i];
      }
      wgm_obs.tkl =  inpkt->hdr.tkl;
      for(i=0;i<inpkt->hdr.tkl;i++){
        wgm_obs.tkn[i] = (inpkt->tok.p[i]);
      }
      obs_req_weg = 1;
      return coap_make_response_obs(scratch,outpkt,COAP_TYPE_ACK, (const uint8_t *)obs_data, (size_t) obs_data_size, id_hi, id_lo, (const uint8_t) inpkt->hdr.tkl, (const uint8_t*) inpkt->tok.p, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_TEXT_PLAIN);
    }
}

static char payload[5] = "alive";
static const coap_endpoint_path_t path_id_3 = {2, {"device", "status"}};
static int handle_get_id_3(struct espconn *espconn,const coap_endpoint_t *ep, coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo,uint8_t obs)
{
        return coap_make_response(scratch, outpkt, (const uint8_t *)(&payload), strlen(payload), id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_TEXT_PLAIN);
}


const coap_endpoint_t endpoints[] =
{
    {COAP_METHOD_GET, handle_get_well_known_core, &path_well_known_core, "ct=40",NULL},
    {COAP_METHOD_GET, handle_get_id, &path_id, "ct=0",NULL},
    {COAP_METHOD_GET, handle_get_id_1, &path_id_1, "ct=0",NULL},
    {COAP_METHOD_GET, handle_get_id_2, &path_id_2, "ct=0",NULL},
    {COAP_METHOD_GET, handle_get_id_3, &path_id_3, "ct=0",NULL},
    {(coap_method_t)0, NULL, NULL}
};


void build_well_known_rsp(char *rsp, uint16_t rsplen)
{
    const coap_endpoint_t *ep = endpoints;
    int i;
    uint16_t len = rsplen;

    c_memset(rsp, 0, len);

    len--; // Null-terminated string

    while(NULL != ep->handler)
    {
        if (NULL == ep->core_attr) {
            ep++;
            continue;
        }
        if (NULL == ep->user_entry){
            if (0 < c_strlen(rsp)) {
                c_strncat(rsp, ",", len);
                len--;
            }
        
            c_strncat(rsp, "<", len);
            len--;

            for (i = 0; i < ep->path->count; i++) {
                c_strncat(rsp, "/", len);
                len--;

                c_strncat(rsp, ep->path->elems[i], len);
                len -= c_strlen(ep->path->elems[i]);
            }

            c_strncat(rsp, ">;", len);
            len -= 2;

            c_strncat(rsp, ep->core_attr, len);
            len -= c_strlen(ep->core_attr);
        } else {
            coap_luser_entry *h = ep->user_entry->next;     // ->next: skip the first entry(head)
            while(NULL != h){
                if (0 < c_strlen(rsp)) {
                    c_strncat(rsp, ",", len);
                    len--;
                }
            
                c_strncat(rsp, "<", len);
                len--;

                for (i = 0; i < ep->path->count; i++) {
                    c_strncat(rsp, "/", len);
                    len--;

                    c_strncat(rsp, ep->path->elems[i], len);
                    len -= c_strlen(ep->path->elems[i]);
                }

                c_strncat(rsp, "/", len);
                len--;

                c_strncat(rsp, h->name, len);
                len -= c_strlen(h->name);

                c_strncat(rsp, ">;", len);
                len -= 2;

                c_strncat(rsp, ep->core_attr, len);
                len -= c_strlen(ep->core_attr);  

                h = h->next;
            }
        }
        ep++;
    }
}

