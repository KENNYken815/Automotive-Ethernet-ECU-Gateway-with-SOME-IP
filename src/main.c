#include "gateway.h"
#include "someip.h"
#include <stdio.h>
int main(void){
 GatewayState s;gateway_init(&s);s.speed_kph_x100=8234;s.rpm=2350;s.body_state=1;
 uint8_t req[64],resp[64],body=1;size_t q=0;uint32_t z=0;
 SomeIpMessage m={0};m.header.service_id=SERVICE_VEHICLE;m.header.method_id=METHOD_GET_SPEED;
 m.header.request_id=0x00010001u;m.header.message_type=SOMEIP_REQUEST;m.header.client_id=1;m.header.session_id=1;
 if(someip_serialize(&m,req,sizeof(req),&q))return 1;if(gateway_handle_someip(req,(uint32_t)q,&s,resp,sizeof(resp),&z))return 2;
 SomeIpMessage p;if(someip_parse(resp,z,&p))return 3;uint16_t speed=(uint16_t)(((uint16_t)p.payload[0]<<8)|p.payload[1]);
 printf("SOME/IP gateway demo\nGET_SPEED response: %u.%02u km/h\n",speed/100u,speed%100u);
 m.header.method_id=METHOD_SET_BODY_STATE;m.payload=&body;m.payload_len=1;
 someip_serialize(&m,req,sizeof(req),&q);gateway_handle_someip(req,(uint32_t)q,&s,resp,sizeof(resp),&z);
 printf("SET_BODY_STATE -> %u\n",(unsigned)s.body_state);return 0;
}
