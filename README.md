# Automotive Ethernet ECU Gateway with SOME/IP

A portable C reference implementation of an automotive Ethernet gateway that demonstrates routing between CAN-FD vehicle data and a lightweight SOME/IP-style service interface.

## What it demonstrates

**CAN-FD frame -> CAN ID routing -> ECU domain -> gateway state**

**SOME/IP request -> parser -> service dispatcher -> vehicle/diagnostic action -> SOME/IP response**

The core logic is hardware-independent so it can be compiled and demonstrated with a normal C11 toolchain before replacing platform boundaries with MCU/SoC, CAN-FD and Ethernet drivers.

## Implemented

- Compact SOME/IP header serializer/parser.
- Request, Response, Notification and Error message types.
- Service IDs, method dispatch and return codes.
- CAN-FD frame model supporting up to 64 data bytes.
- Illustrative routing for Powertrain, Body, ADAS and Infotainment domains.
- Vehicle services for speed, RPM and body state.
- Diagnostic DTC read/clear.
- CAN-to-SOME/IP notification translation.
- Payload-length and routing validation.
- Portable regression tests.
- Deterministic presentation demo.
- Architecture, SOME/IP subset and service-interface documentation.

## Repository structure

```text
.
├── include/
│   ├── canfd.h
│   ├── gateway.h
│   └── someip.h
├── src/
│   ├── gateway.c
│   ├── main.c
│   └── someip.c
├── tests/
│   └── test_gateway.c
├── docs/
│   ├── ARCHITECTURE.md
│   ├── SERVICE_INTERFACE.md
│   ├── SOMEIP.md
│   └── TEST_PLAN.md
├── .gitignore
├── Makefile
└── README.md
```

## Build and run

Requirements: a C11-capable compiler and `make`.

```bash
make
```

Or run the targets separately:

```bash
make demo
make test
make clean
```

Expected demo:

```text
SOME/IP gateway demo
GET_SPEED response: 82.34 km/h
SET_BODY_STATE -> 1
```

Expected test result:

```text
All gateway tests passed.
```

## Service interface

### Vehicle service `0x1000`

- `0x0001 GET_SPEED`: returns speed as uint16 km/h x100.
- `0x0002 GET_RPM`: returns engine speed as uint16 rpm.
- `0x0003 SET_BODY_STATE`: accepts one byte and echoes the state.

### Diagnostics service `0x1001`

- `0x0100 READ_DTC`: returns a uint32 demonstration DTC.
- `0x0101 CLEAR_DTC`: clears that stored DTC.
- `0x0200 CAN_INJECT`: validates a CAN identifier and reports its routed ECU domain.

See [docs/SERVICE_INTERFACE.md](docs/SERVICE_INTERFACE.md) for payload details and return codes.

## Illustrative CAN routing

These IDs are project examples, **not OEM signal definitions**.

| Example CAN ID | Domain |
|---:|---|
| 0x100, 0x101, 0x110 | Powertrain |
| 0x200, 0x210 | Body |
| 0x300, 0x310 | ADAS |
| 0x400, 0x410 | Infotainment |

Unknown identifiers are rejected by the demo router.

## Presentation flow

1. Explain the gateway's role between legacy CAN/CAN-FD networks and Ethernet service communication.
2. Show the SOME/IP serializer/parser and header fields.
3. Walk through CAN ID to ECU-domain routing.
4. Demonstrate `GET_SPEED` and `SET_BODY_STATE`.
5. Demonstrate diagnostic DTC read/clear.
6. Show CAN-to-SOME/IP notification conversion.
7. Discuss how the same interfaces map to real CAN-FD drivers, Ethernet sockets or an AUTOSAR stack.

## Engineering scope and limitations

This is a **software/reference implementation for learning and portfolio presentation**. It is not a production AUTOSAR SOME/IP stack or a validated automotive gateway.

Out of scope:
- real Ethernet socket transport
- SOME/IP Service Discovery
- SOME/IP-TP segmentation
- AUTOSAR SoAd/PduR integration
- E2E protection and production security/authentication
- physical CAN-FD controller/MCAL integration
- OEM ARXML signal databases
- hardware-in-the-loop validation
- functional-safety certification

The project intentionally keeps those platform-specific layers outside the portable gateway core.

## Extension path

A hardware-oriented next stage would add:
1. CAN-FD driver/MCAL adapter.
2. Ethernet socket or AUTOSAR Ethernet adapter.
3. SOME/IP Service Discovery.
4. ARXML-derived service/signal definitions.
5. Watchdog, timeout supervision, E2E and security mechanisms.
6. Target-specific build configuration and HIL tests.

## License

MIT License.
