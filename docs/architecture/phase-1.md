# Phase 1 Documentation

## Objective
Wire core runtime path:
1. `camera_adapter` pulls frames from SDK camera output.
2. `frame_broker` provides shared latest-frame handoff.
3. `live_http_mjpeg` replaces legacy HTTP entry point at API level.

## Implemented Modules
1. `firmware/platform/sdk_adapter/camera_adapter`
2. `firmware/media/frame_broker`
3. `firmware/network/live_http_mjpeg`
4. `firmware/app/main` (wiring orchestration)
5. `firmware/common/status_codes` (shared status constants)

## Wiring Flow
1. `app_main_start()` starts camera adapter and frame broker.
2. `app_main_tick()` calls `camera_adapter_get_frame()`.
3. On frame availability, it publishes frame bytes and metadata to `frame_broker_publish()`.
4. `live_http_mjpeg_tick()` consumes latest frame metadata from broker.
5. Legacy symbol `mhttp_server_init()` now maps to `live_http_mjpeg_start()`.

## Legacy Replacement Strategy
`mhttp_server_init` compatibility function is now in:
- `firmware/network/live_http_mjpeg/live_http_mjpeg.c`

This allows migration from the old call site while moving implementation into the new service module.

## Current Scope Boundary
1. Socket accept/send loop is not implemented yet in new service.
2. Phase 1 focuses on data-path wiring and replacement boundary.
3. Full HTTP client handling is Phase 2 work.

## Integration Note
To fully switch from old implementation in build:
1. Include new `firmware/network/live_http_mjpeg/*.c` in build.
2. Exclude old `AiPi-CAM-D200/ap_web/mlwip_https.c` from build to avoid duplicate symbol definitions.
