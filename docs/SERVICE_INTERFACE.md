# Service Interface

## Vehicle service 0x1000

| Method | ID | Request | Response |
|---|---:|---|---|
| GET_SPEED | 0x0001 | none | uint16, km/h x100 |
| GET_RPM | 0x0002 | none | uint16, rpm |
| SET_BODY_STATE | 0x0003 | uint8 | echoed uint8 |

## Diagnostics service 0x1001

| Method | ID | Request | Response |
|---|---:|---|---|
| READ_DTC | 0x0100 | none | uint32 DTC |
| CLEAR_DTC | 0x0101 | none | empty |
| CAN_INJECT | 0x0200 | CAN ID(4) + DLC(1) + data | ECU-domain byte |

Return codes:
- 0x00 success
- 0x02 unknown service/method
- 0x22 invalid payload
- 0x31 unknown routing target

All identifiers and vehicle values are illustrative.
