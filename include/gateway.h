#ifndef GATEWAY_H
#define GATEWAY_H
#include "canfd.h"
#include <stdint.h>
enum { ECU_BODY=0, ECU_POWERTRAIN=1, ECU_ADAS=2, ECU_INFOTAINMENT=3, ECU_UNKNOWN=255 };
enum { SERVICE_VEHICLE=0x1000, SERVICE_DIAGNOSTICS=0x1001 };
enum { METHOD_GET_SPEED=0x0001, METHOD_GET_RPM=0x0002, METHOD_SET_BODY_STATE=0x0003,
       METHOD_READ_DTC=0x0100, METHOD_CLEAR_DTC=0x0101, METHOD_CAN_INJECT=0x0200 };
typedef struct { uint16_t speed_kph_x100, rpm; uint8_t body_state; uint32_t dtc; } GatewayState;
void gateway_init(GatewayState*);
uint8_t gateway_route_can_id(uint32_t);
int gateway_handle_someip(const uint8_t*,uint32_t,GatewayState*,uint8_t*,uint32_t,uint32_t*);
int gateway_translate_can_to_someip(const CanFdFrame*,uint8_t*,uint32_t,uint32_t*);
#endif
