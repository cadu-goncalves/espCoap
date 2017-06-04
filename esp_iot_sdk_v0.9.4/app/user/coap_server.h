#ifndef _COAP_SERVER_H
#define _COAP_SERVER_H 1

#ifdef __cplusplus
extern "C" {
#endif

size_t ICACHE_FLASH_ATTR coap_server_respond(struct espconn *espconn,char *req, unsigned short reqlen, char *rsp, unsigned short rsplen);

#ifdef __cplusplus
}
#endif

#endif
