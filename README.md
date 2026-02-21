# WebCameraFirmware

Modern firmware blueprint for **AiPi-CAM-D200**.

Product requirements (unchanged):
1. Camera automatically connects to Wi-Fi.
2. Camera connects to NAS and pushes video data.
3. Users can watch real-time video while upload is running.

## 1. Design Principles

1. Reuse what is proven in the SDK for hardware-critical parts.
2. Replace fragile or legacy patterns with modular, testable services.
3. Keep V1 simple and reliable first, then add advanced protocols.
4. Prefer explicit interfaces between modules over direct global-state coupling.
5. All data delivery must be recoverable after reboot/network failure.

## 2. What We Reuse vs Replace

Reuse from SDK:
1. Camera capture path and MJPEG hardware flow from `AiPi-BL618-Open-SDK-master/AiPi-CAM-D200/d200_camera/d200_camera.c`.
2. Wi-Fi bring-up/events from `AiPi-BL618-Open-SDK-master/AiPi-CAM-D200/bl618_wifi/bl618_wifi.c`.
3. FreeRTOS/lwIP/mbedTLS/FATFS base config from `AiPi-BL618-Open-SDK-master/AiPi-CAM-D200/proj.conf`.
4. SD card filesystem initialization pattern from `AiPi-BL618-Open-SDK-master/AiPi-CAM-D200/filesystem/files.c`.

Replace or redesign:
1. Monolithic HTTP server in `AiPi-BL618-Open-SDK-master/AiPi-CAM-D200/ap_web/mlwip_https.c`.
2. Single-client gating (`mysemaphoreflag`) and mixed responsibilities.
3. No NAS queue/state machine logic.
4. Ad-hoc cross-module globals (`pic_addr`, `pic_size`) without clear ownership contracts.

## 3. Modern Architecture

```text
+---------------------------- Control Plane -----------------------------+
| boot | lifecycle state machine | config manager | watchdog | ota      |
+------------------------------+----------------------------------------+
                               |
                               v
+----------------------------- Media Plane -----------------------------+
| camera driver adapter | frame broker | frame metadata | snapshot API  |
| input: HW MJPEG frames from SDK | output: shared frame channel        |
+------------------------------+----------------------------------------+
                               |
                  +------------+------------+
                  |                         |
                  v                         v
+-----------------------------+   +-------------------------------------+
| Live View Service           |   | Recording & NAS Service             |
| HTTP MJPEG stream (V1)      |   | segment writer -> spool -> uploader |
| auth + session manager      |   | retry, resume, integrity tracking   |
+-----------------------------+   +-------------------------------------+
                  |                         |
                  +------------+------------+
                               v
+------------------------- Persistence Layer ---------------------------+
| FATFS abstraction | queue index log | segment files | config storage |
+---------------------------------------------------------------------+
```

## 4. V1 Technology Decisions

1. Real-time viewing protocol in V1: **HTTP MJPEG**.
Reason: already available in SDK and cheapest latency/complexity for BL616.

2. NAS upload protocol in V1: **WebDAV over HTTPS**.
Reason: widely supported by NAS vendors, simple file PUT semantics, easier than SMB/NFS on constrained firmware.

3. Segment format in V1: **JPEG frame sequence grouped into time-window files**.
Reason: native camera output is MJPEG; avoid on-device transcoding in first version.

4. Optional V2 protocol: RTSP/WebRTC.
Reason: better ecosystem support, but higher implementation cost and memory risk.

## 5. Firmware Module Structure (Target Repository)

```text
WebCameraFirmware/
├── README.md
├── firmware/
│   ├── app/
│   │   ├── main/
│   │   ├── lifecycle/
│   │   ├── config/
│   │   ├── watchdog/
│   │   └── ota/
│   ├── platform/
│   │   ├── sdk_adapter/
│   │   │   ├── camera_adapter/
│   │   │   ├── wifi_adapter/
│   │   │   └── fs_adapter/
│   │   └── board/
│   ├── media/
│   │   ├── frame_broker/
│   │   ├── frame_pool/
│   │   ├── snapshot/
│   │   └── segmenter/
│   ├── network/
│   │   ├── live_http_mjpeg/
│   │   ├── net_monitor/
│   │   ├── ntp_time/
│   │   └── tls/
│   ├── nas/
│   │   ├── webdav_client/
│   │   ├── upload_queue/
│   │   ├── uploader_task/
│   │   └── remote_layout/
│   ├── storage/
│   │   ├── spool_store/
│   │   ├── queue_index/
│   │   └── retention/
│   └── common/
│       ├── event_bus/
│       ├── status_codes/
│       ├── metrics/
│       └── logging/
├── docs/
│   ├── adr/
│   ├── api/
│   ├── architecture/
│   └── test/
└── tests/
    ├── unit/
    ├── integration/
    ├── soak/
    └── fault_injection/
```

## 6. Runtime Services and Contracts

### 6.1 `frame_broker`
1. Owns latest valid frame pointer and metadata.
2. Provides lock-safe read API for consumers.
3. Guarantees frame immutability during reader window.

Output metadata fields:
1. `frame_id` (monotonic)
2. `capture_ts_ms`
3. `jpeg_size`
4. `width`
5. `height`
6. `quality`

### 6.2 `live_http_mjpeg`
1. Serves `/live` stream endpoint.
2. Supports multiple client sessions with per-session backpressure handling.
3. Drops frames for slow clients instead of blocking camera pipeline.

### 6.3 `segmenter`
1. Builds time-window segments from incoming frames.
2. Writes segment files to spool path.
3. Emits queue records for uploader.

### 6.4 `upload_queue`
1. Persistent queue record stored on SD.
2. States: `PENDING`, `UPLOADING`, `RETRY_WAIT`, `DONE`, `FAILED_PERM`.
3. Crash-safe state transitions with append-only journal.

### 6.5 `uploader_task`
1. Picks next `PENDING` job by priority/time.
2. Uploads with timeout, retry, and exponential backoff with jitter.
3. Verifies response and finalizes record atomically.

## 7. Data Layout (Local and NAS)

Local SD spool layout:
```text
/sd/spool/YYYY/MM/DD/HH/
  seg_<epoch_ms>_<seq>.mjpg
  seg_<epoch_ms>_<seq>.meta.json
/sd/queue/
  queue.journal
  queue.snapshot
/sd/config/
  device_config.json
```

NAS layout:
```text
/camera/<device_id>/YYYY/MM/DD/HH/
  seg_<epoch_ms>_<seq>.mjpg
  seg_<epoch_ms>_<seq>.meta.json
```

Metadata file fields (minimum):
1. `device_id`
2. `segment_start_ms`
3. `segment_end_ms`
4. `frame_count`
5. `bytes`
6. `checksum_sha256`

## 8. Connectivity and Provisioning

1. Boot path:
`INIT -> WIFI_CONNECTING -> TIME_SYNC -> SERVICES_STARTING -> RUNNING`

2. Failure path:
`RUNNING -> DEGRADED (network lost) -> RECOVERY (reconnect/upload resume) -> RUNNING`

3. Provisioning:
1. If stored Wi-Fi missing/invalid, start provisioning mode.
2. Provisioning mode provides temporary AP + setup endpoint.
3. Credentials saved encrypted in device config store.

## 9. Security Baseline

1. All NAS upload requests use TLS (mbedTLS).
2. Live view endpoint requires authentication token.
3. Secrets are never logged.
4. Firmware uses signed image verification before OTA apply.
5. Debug interfaces disabled or authenticated in production builds.

## 10. Reliability Rules

1. No data deletion before successful NAS ACK.
2. On reboot, queue journal replay restores pending jobs.
3. If SD is full, apply retention policy to remove oldest uploaded segments first.
4. If network is unstable, keep live view responsive and let uploader degrade gracefully.
5. Watchdog must supervise critical tasks: camera, live server, uploader, queue manager.

## 11. Performance Targets (Initial)

1. Wi-Fi reconnect recovery: < 30 seconds typical.
2. Live view latency target (LAN): < 800 ms.
3. Upload resume after reconnect: < 20 seconds.
4. 24-hour continuous run without task crash or memory growth trend.
5. Spool integrity after forced reboot during upload.

## 12. Configuration Schema (Versioned)

`config_version = 1` fields:
1. `device_id`
2. `wifi.ssid`
3. `wifi.passphrase`
4. `live.auth_enabled`
5. `live.max_clients`
6. `capture.jpeg_quality`
7. `segment.duration_sec`
8. `nas.endpoint_url`
9. `nas.username`
10. `nas.password`
11. `nas.base_path`
12. `retention.max_spool_mb`
13. `upload.retry_max`
14. `upload.base_backoff_ms`

Rule:
1. Add migration handler for every new `config_version`.

## 13. SDK Reference Map (For Future Coding)

1. Camera capture and ISR reference:
`AiPi-BL618-Open-SDK-master/AiPi-CAM-D200/d200_camera/d200_camera.c`

2. Wi-Fi events and state callbacks:
`AiPi-BL618-Open-SDK-master/AiPi-CAM-D200/bl618_wifi/bl618_wifi.c`

3. Existing HTTP MJPEG behavior reference:
`AiPi-BL618-Open-SDK-master/AiPi-CAM-D200/ap_web/mlwip_https.c`

4. FATFS mount and write reference:
`AiPi-BL618-Open-SDK-master/AiPi-CAM-D200/filesystem/files.c`

5. Build integration baseline:
`AiPi-BL618-Open-SDK-master/AiPi-CAM-D200/CMakeLists.txt`

6. Feature toggles baseline:
`AiPi-BL618-Open-SDK-master/AiPi-CAM-D200/proj.conf`

## 14. Step-by-Step Development Plan

Phase 0:
1. Create repository folder skeleton.
2. Add module interface headers with empty implementations.
3. Add coding conventions and error code table.

Phase 1:
1. Wrap SDK camera into `camera_adapter`.
2. Implement `frame_broker` with thread-safe readers.
3. Replace old HTTP code with `live_http_mjpeg` service.

Phase 2:
1. Build spool writer and queue journal.
2. Add segment metadata generation and checksum.
3. Add retention manager.

Phase 3:
1. Implement WebDAV client for NAS upload.
2. Implement uploader state machine + retry/backoff.
3. Add reboot recovery replay for queue.

Phase 4:
1. Add configuration manager and provisioning flow.
2. Add auth for live view.
3. Add metrics and health endpoints.

Phase 5:
1. Add OTA pipeline and signature verification.
2. Run soak/fault-injection tests.
3. Prepare release checklist and production profile.

## 15. Definition of Done for V1

1. Device can boot and auto-connect to known Wi-Fi.
2. Device provides stable real-time MJPEG live view to at least one client.
3. Device continuously writes segments and uploads to NAS.
4. Device recovers from Wi-Fi outage and resumes pending uploads automatically.
5. Device survives power loss without queue corruption.
6. Unit + integration + soak tests pass for critical flows.

## 16. Phase 0 Artifacts

1. Phase 0 delivery summary:
`docs/architecture/phase-0.md`
2. Coding conventions:
`docs/architecture/coding-conventions.md`
3. Error code table:
`docs/api/error-codes.md`
4. Testing and execution guide:
`docs/test/test-plan.md`
5. Foundation ADR:
`docs/adr/ADR-0001-phase0-foundation.md`

## 17. Phase 1 Artifacts

1. Phase 1 wiring summary:
`docs/architecture/phase-1.md`
