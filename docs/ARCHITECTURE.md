# Architecture

The reference gateway separates protocol handling from vehicle routing logic.

## Data flow

CAN-FD frame -> CAN ID router -> ECU domain -> gateway state/service adapter

SOME/IP request -> parser -> service dispatcher -> vehicle/diagnostic action -> SOME/IP response

## Modules

- `include/`: public interfaces and data models.
- `src/someip.c`: compact SOME/IP header serialization/parsing.
- `src/gateway.c`: routing, service dispatch, diagnostics and CAN translation.
- `src/main.c`: deterministic presentation demo.
- `tests/`: regression tests.

The CAN identifiers are illustrative project IDs rather than OEM network definitions.
