# Coding Conventions (Phase 0)

## Scope
This document defines baseline C coding and repository conventions for this firmware project.

## Language and Build
1. Language: C11-compatible style.
2. Compiler assumptions: embedded GCC toolchain (RISC-V target for BL616).
3. Keep module public APIs in `.h` and internal logic in `.c`.

## Naming
1. Files: `snake_case.c` and `snake_case.h`.
2. Public functions: `module_name_action()`.
3. Types: `snake_case_t`.
4. Macros: `UPPER_SNAKE_CASE`.
5. Constants: `k` prefix is not used; use `UPPER_SNAKE_CASE`.

## Interface Rules
1. Each module exposes one primary interface header.
2. Headers must be self-contained and include required standard headers.
3. Use include guards, not `#pragma once`.
4. Keep dependencies one-way: `app -> services -> platform adapters`.

## Error Handling
1. Return `int` status from module APIs.
2. `0` means success.
3. Negative values indicate errors from the shared error table.
4. Do not swallow errors; return or log with context.

## Concurrency
1. No shared mutable state without explicit ownership.
2. Use mutex/queue wrappers from platform adapters for cross-task data.
3. Long-running loops must include delay/blocking wait to avoid CPU hogging.

## Logging
1. Log line must include module prefix and error code when failing.
2. Never log secrets (Wi-Fi password, NAS password, auth token).
3. Use consistent log levels: `ERROR`, `WARN`, `INFO`, `DEBUG`.

## Memory and Buffers
1. Avoid heap allocation in high-frequency data paths.
2. Prefer static pools/ring buffers for frame and queue operations.
3. Validate all input pointers and size fields.

## Documentation
1. Public headers should include short API intent comments.
2. Non-trivial behavior must be documented in `docs/` before implementation.
3. Architecture changes should add or update an ADR under `docs/adr`.
