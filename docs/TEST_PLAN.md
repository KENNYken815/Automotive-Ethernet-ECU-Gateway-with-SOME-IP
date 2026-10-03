# Test Plan

Run:

```bash
make test
```

Covered:
1. CAN ID to ECU-domain routing.
2. SOME/IP request serialization and parsing.
3. Vehicle speed service response.
4. Diagnostic DTC read.
5. Diagnostic DTC clear.

The deterministic demo is run with:

```bash
make demo
```

Future hardware-integration testing should add actual Ethernet sockets, CAN-FD drivers, malformed-packet fuzzing, service discovery and HIL testing.
