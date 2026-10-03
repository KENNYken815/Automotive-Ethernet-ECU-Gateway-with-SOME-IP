#include "gateway.h"
#include "someip.h"
#include <assert.h>
#include <stdio.h>
static void req(uint16_t s,uint16_t m,const uint8_t*p,uint32_t n,uint8_t*b,size_t c,size_t*l){
 SomeIpMessage x={0};x.header.service_id=s;x.header.method_id=m;x.header.request_id=0x12340001u;x.header.message_type=SOMEIP_REQUEST;x.header.client_id=0x12;x.header.session_id=1;x.payload=p;x.payload_len=n;
 assert(someip_serialize(&x,b,c,l)==0);
}
int main(void){
 assert(gateway_route_can_id(0x100)==ECU_POWERTRAIN);assert(gateway_route_can_id(0x200)==ECU_BODY);
 assert(gateway_route_can_id(0x300)==ECU_ADAS);assert(gateway_route_can_id(0x400)==ECU_INFOTAINMENT);assert(gateway_route_can_id(0x555)==ECU_UNKNOWN);
 GatewayState s;gateway_init(&s);s.speed_kph_x100=12345;s.dtc=0x12345678u;
 uint8_t a[64],b[64];size_t n=0;uint32_t z=0;SomeIpMessage p;
 req(SERVICE_VEHICLE,METHOD_GET_SPEED,0,0,a,sizeof(a),&n);assert(gateway_handle_someip(a,n,&s,b,sizeof(b),&z)==0);assert(someip_parse(b,z,&p)==0);assert(p.payload_len==2&&p.payload[0]==0x30&&p.payload[1]==0x39);
 req(SERVICE_DIAGNOSTICS,METHOD_READ_DTC,0,0,a,sizeof(a),&n);assert(gateway_handle_someip(a,n,&s,b,sizeof(b),&z)==0);assert(someip_parse(b,z,&p)==0);assert(p.payload_len==4&&p.payload[0]==0x12&&p.payload[3]==0x78);
 req(SERVICE_DIAGNOSTICS,METHOD_CLEAR_DTC,0,0,a,sizeof(a),&n);assert(gateway_handle_someip(a,n,&s,b,sizeof(b),&z)==0);assert(s.dtc==0);
 puts("All gateway tests passed.");return 0;
}
