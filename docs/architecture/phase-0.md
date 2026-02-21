# Phase 0 Documentation

## Objective
Establish a clean, modular, implementation-ready project foundation.

## Deliverables Completed
1. Repository folder skeleton created according to README target structure.
2. Interface headers and empty `.c` stubs created for all listed firmware modules.
3. Baseline coding conventions documented.
4. Shared error code table documented.
5. Test plan and execution guide documented.

## Module Scaffold Summary
Each module currently provides:
1. `*_init(const *_config_t *config)`
2. `*_start(void)`
3. `*_stop(void)`
4. `*_tick(void)`

All current implementations return success (`0`) and are placeholders.

## Known Limitations in Phase 0
1. No build system integration for new scaffold yet.
2. No runtime wiring between modules yet.
3. No real functionality implemented in stubs.
4. No automated test scripts yet.

## Next Recommended Step (Phase 1)
1. Integrate scaffold into build targets.
2. Implement `camera_adapter` and `frame_broker` first.
3. Replace legacy HTTP path with modular `live_http_mjpeg` service.
4. Add first unit tests for lifecycle and frame broker interfaces.
