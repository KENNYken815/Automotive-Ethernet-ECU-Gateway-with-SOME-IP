#ifndef SOMEIP_H
#define SOMEIP_H
#include <stddef.h>
#include <stdint.h>
#define SOMEIP_HEADER_SIZE 16u
#define SOMEIP_MAX_PAYLOAD 1400u
typedef struct {
    uint16_t service_id, method_id;
    uint8_t client_id, session_id, message_type, return_code;
    uint32_t request_id;
} SomeIpHeader;
typedef struct { SomeIpHeader header; const uint8_t *payload; uint32_t payload_len; } SomeIpMessage;
enum { SOMEIP_REQUEST=0x00, SOMEIP_RESPONSE=0x80, SOMEIP_NOTIFICATION=0x02, SOMEIP_ERROR=0x81 };
int someip_serialize(const SomeIpMessage*, uint8_t*, size_t, size_t*);
int someip_parse(const uint8_t*, size_t, SomeIpMessage*);
int someip_build_response(const SomeIpMessage*, uint8_t, const uint8_t*, uint32_t, uint8_t*, size_t, size_t*);
#endif
