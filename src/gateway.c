#include "gateway.h"
#include "someip.h"
#include <string.h>
static void p16(uint8_t*p,uint16_t v){p[0]=(uint8_t)(v>>8);p[1]=(uint8_t)v;}
static int resp(const SomeIpMessage*r,uint8_t rc,const uint8_t*p,uint32_t n,uint8_t*out,uint32_t cap,uint32_t*olen){
 size_t z=0;int e=someip_build_response(r,rc,p,n,out,cap,&z);*olen=(uint32_t)z;return e;
}
void gateway_init(GatewayState*s){if(s)memset(s,0,sizeof(*s));}
uint8_t gateway_route_can_id(uint32_t id){
 switch(id&0x7FFu){case 0x100:case 0x101:case 0x110:return ECU_POWERTRAIN;
 case 0x200:case 0x210:return ECU_BODY;case 0x300:case 0x310:return ECU_ADAS;
 case 0x400:case 0x410:return ECU_INFOTAINMENT;default:return ECU_UNKNOWN;}
}
int gateway_handle_someip(const uint8_t*reqbuf,uint32_t req_len,GatewayState*s,uint8_t*out,uint32_t cap,uint32_t*olen){
 if(!s||!out||!olen)return -1;SomeIpMessage r;if(someip_parse(reqbuf,req_len,&r))return -2;
 uint8_t p[8]={0},rc=0;uint32_t n=0;
 if(r.header.service_id==SERVICE_VEHICLE){
  switch(r.header.method_id){
   case METHOD_GET_SPEED:p16(p,s->speed_kph_x100);n=2;break;
   case METHOD_GET_RPM:p16(p,s->rpm);n=2;break;
   case METHOD_SET_BODY_STATE:if(r.payload_len!=1)rc=0x22;else{s->body_state=r.payload[0];p[0]=s->body_state;n=1;}break;
   default:rc=0x02;
  }
 }else if(r.header.service_id==SERVICE_DIAGNOSTICS){
  switch(r.header.method_id){
   case METHOD_READ_DTC:p[0]=(uint8_t)(s->dtc>>24);p[1]=(uint8_t)(s->dtc>>16);p[2]=(uint8_t)(s->dtc>>8);p[3]=(uint8_t)s->dtc;n=4;break;
   case METHOD_CLEAR_DTC:s->dtc=0;break;
   case METHOD_CAN_INJECT:
    if(r.payload_len<5)rc=0x22;else{uint32_t id=((uint32_t)r.payload[0]<<24)|((uint32_t)r.payload[1]<<16)|((uint32_t)r.payload[2]<<8)|r.payload[3];uint8_t dlc=r.payload[4];
     if(dlc>CANFD_MAX_DATA||r.payload_len!=5u+dlc)rc=0x22;else if(gateway_route_can_id(id)==ECU_UNKNOWN)rc=0x31;else{p[0]=gateway_route_can_id(id);n=1;}}
    break;
   default:rc=0x02;
  }
 }else rc=0x02;
 return resp(&r,rc,p,n,out,cap,olen);
}
int gateway_translate_can_to_someip(const CanFdFrame*f,uint8_t*out,uint32_t cap,uint32_t*olen){
 if(!f||!out||!olen||f->dlc>CANFD_MAX_DATA)return -1;uint8_t p[69];p[0]=(uint8_t)(f->id>>24);p[1]=(uint8_t)(f->id>>16);p[2]=(uint8_t)(f->id>>8);p[3]=(uint8_t)f->id;p[4]=f->dlc;
 for(uint8_t i=0;i<f->dlc;i++)p[5+i]=f->data[i];
 SomeIpMessage m={0};m.header.service_id=SERVICE_VEHICLE;m.header.method_id=METHOD_CAN_INJECT;m.header.request_id=0x00010001u;m.header.message_type=SOMEIP_NOTIFICATION;m.header.client_id=1;m.header.session_id=1;m.payload=p;m.payload_len=5u+f->dlc;
 size_t z=0;int e=someip_serialize(&m,out,cap,&z);*olen=(uint32_t)z;return e;
}
