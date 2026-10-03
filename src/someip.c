#include "someip.h"
static void p16(uint8_t*p,uint16_t v){p[0]=(uint8_t)(v>>8);p[1]=(uint8_t)v;}
static void p32(uint8_t*p,uint32_t v){p[0]=(uint8_t)(v>>24);p[1]=(uint8_t)(v>>16);p[2]=(uint8_t)(v>>8);p[3]=(uint8_t)v;}
static uint16_t g16(const uint8_t*p){return (uint16_t)(((uint16_t)p[0]<<8)|p[1]);}
static uint32_t g32(const uint8_t*p){return ((uint32_t)p[0]<<24)|((uint32_t)p[1]<<16)|((uint32_t)p[2]<<8)|p[3];}
int someip_serialize(const SomeIpMessage*m,uint8_t*out,size_t cap,size_t*olen){
 if(!m||!out||!olen||m->payload_len>SOMEIP_MAX_PAYLOAD)return -1;
 if(cap<SOMEIP_HEADER_SIZE+m->payload_len)return -2;
 p16(out,m->header.service_id);p16(out+2,m->header.method_id);
 p32(out+4,8u+m->payload_len);p16(out+8,(uint16_t)(m->header.request_id>>16));
 p16(out+10,(uint16_t)m->header.request_id);out[12]=m->header.message_type;
 out[13]=m->header.return_code;out[14]=m->header.client_id;out[15]=m->header.session_id;
 for(uint32_t i=0;i<m->payload_len;i++)out[16+i]=m->payload[i];
 *olen=16u+m->payload_len;return 0;
}
int someip_parse(const uint8_t*d,size_t len,SomeIpMessage*m){
 if(!d||!m||len<16)return -1; uint32_t l=g32(d+4); if(l<8u||l+8u>len)return -2;
 m->header.service_id=g16(d);m->header.method_id=g16(d+2);
 m->header.request_id=((uint32_t)g16(d+8)<<16)|g16(d+10);m->header.message_type=d[12];
 m->header.return_code=d[13];m->header.client_id=d[14];m->header.session_id=d[15];
 m->payload=d+16;m->payload_len=l-8u;return 0;
}
int someip_build_response(const SomeIpMessage*r,uint8_t rc,const uint8_t*p,uint32_t n,uint8_t*out,size_t cap,size_t*olen){
 if(!r)return -1; SomeIpMessage x=*r;x.header.message_type=rc==0?SOMEIP_RESPONSE:SOMEIP_ERROR;
 x.header.return_code=rc;x.payload=p;x.payload_len=n;return someip_serialize(&x,out,cap,olen);
}
