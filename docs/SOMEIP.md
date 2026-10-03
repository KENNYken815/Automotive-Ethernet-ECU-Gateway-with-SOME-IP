# SOME/IP Subset

Implemented reference fields:
- Service ID
- Method ID
- Length
- Request ID
- Message Type
- Return Code
- Client ID
- Session ID

Implemented message types:
- Request `0x00`
- Notification `0x02`
- Response `0x80`
- Error `0x81`

Multi-byte values are serialized in network byte order.

This project demonstrates a small SOME/IP-style wire layer; it is not a complete AUTOSAR SOME/IP stack. Service Discovery, SOME/IP-TP, E2E protection, authentication, socket transport and production conformance are outside scope.
