# ADR-0001: Phase 0 Foundation and Modular Scaffolding

## Status
Accepted

## Context
The imported SDK is functional but monolithic for application-level features. To build reliable Wi-Fi + NAS upload + live view firmware, we need a maintainable modular baseline.

## Decision
Adopt a service-oriented firmware structure with explicit module interfaces and placeholder implementations in Phase 0.

## Consequences
1. Positive: Clear ownership boundaries, easier testing, lower coupling.
2. Positive: Incremental implementation can proceed module-by-module.
3. Tradeoff: Additional boilerplate before feature implementation.

## Follow-up
Phase 1 must connect scaffolded modules to actual SDK adapters and begin replacing legacy monolithic service paths.
