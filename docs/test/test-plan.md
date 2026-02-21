# Test Plan and How to Test (Phase 0)

## Goal
Define how to validate required product behavior during future phases:
1. Auto Wi-Fi connect/reconnect
2. NAS upload reliability
3. Real-time viewing while upload runs

## Test Strategy
1. Unit tests for core logic (state machine, queue, retry, config parsing).
2. Integration tests on target hardware for camera/network/storage pipeline.
3. Soak tests for long-run stability.
4. Fault-injection tests for reboot/network/storage failures.

## Environments
1. Host simulation environment for pure logic modules.
2. Real device (AiPi-CAM-D200) + AP + NAS test server on LAN.
3. Optional network impairment setup (packet loss/latency) for robustness tests.

## Test Matrix
| Area | Unit | Integration | Soak | Fault Injection |
|---|---|---|---|---|
| Wi-Fi connect/reconnect | Yes | Yes | Yes | Yes |
| Live MJPEG service | Partial | Yes | Yes | Yes |
| Segmenter and spool | Yes | Yes | Yes | Yes |
| Upload queue state machine | Yes | Yes | Yes | Yes |
| NAS uploader | Partial | Yes | Yes | Yes |
| Config/provisioning | Yes | Yes | Yes | Yes |

## Functional Test Cases

### F1. Boot and Auto Wi-Fi Connect
1. Precondition: valid Wi-Fi credentials stored.
2. Steps:
1. Power cycle device.
2. Observe boot logs.
3. Confirm device obtains IP.
3. Expected:
1. Device reaches RUNNING state.
2. Live endpoint becomes available.

### F2. Wi-Fi Reconnect After AP Drop
1. Precondition: device is streaming/uploading.
2. Steps:
1. Turn off AP for 60 seconds.
2. Turn AP on.
3. Observe reconnect behavior.
3. Expected:
1. Device transitions DEGRADED -> RECOVERY -> RUNNING.
2. Upload resumes without reboot.

### F3. Live View While Uploading
1. Precondition: upload queue active.
2. Steps:
1. Start live MJPEG session.
2. Trigger/continue segment generation and NAS upload.
3. Expected:
1. Live stream remains responsive.
2. Upload continues (may reduce throughput but no deadlock/crash).

### F4. Upload Queue Recovery After Reboot
1. Precondition: pending queue entries exist.
2. Steps:
1. Force reboot during upload.
2. Start device again.
3. Expected:
1. Queue journal replay restores pending jobs.
2. Upload resumes and completes eventually.

### F5. SD Full Retention Behavior
1. Precondition: low SD free space.
2. Steps:
1. Fill spool area to threshold.
2. Continue capture/upload workload.
3. Expected:
1. Retention policy removes oldest uploaded segments first.
2. No queue/index corruption.

## Non-Functional Test Cases

### N1. 24h Soak
1. Keep live session periodically active.
2. Keep upload active to NAS.
3. Pass criteria:
1. No task crash/restart loops.
2. No unbounded memory growth.

### N2. Latency and Recovery Metrics
1. Measure live stream latency on LAN.
2. Measure reconnect recovery time.
3. Measure upload resume time after network restoration.
4. Validate against targets in README.

## How To Execute Tests (Step-by-Step)

### Hardware Setup
1. AiPi-CAM-D200 powered and flashed.
2. One Wi-Fi AP with known SSID/password.
3. One NAS endpoint reachable from same network.
4. One observer machine for logs and HTTP checks.

### Basic Verification Commands (Example)
1. Check live stream endpoint with browser/VLC.
2. Check snapshot endpoint with browser/curl.
3. Verify NAS path receives segment files.
4. Watch serial logs for state transitions and error codes.

Command templates:
```bash
# Replace with actual device IP
DEVICE_IP=192.168.1.100

# Snapshot check
curl -v "http://${DEVICE_IP}/capture" -o capture.jpg

# Live stream header check
curl -v "http://${DEVICE_IP}/live" --max-time 10 -o /dev/null

# If legacy endpoint is still enabled during migration
curl -v "http://${DEVICE_IP}/stream" --max-time 10 -o /dev/null

# NAS path verification example (adjust mount/path)
ls -lah /path/to/nas/camera/<device_id>/$(date +%Y/%m/%d)
```

Serial log capture template:
```bash
# macOS example with screen; replace tty device
screen /dev/tty.usbserial-XXXX 2000000
```

### Evidence Collection
1. Save serial logs per test case.
2. Save NAS directory listing before/after tests.
3. Save timestamps for reconnect and upload resume.
4. Record failures with error code and module name.

## Exit Criteria
1. All P0/P1 critical functional tests pass.
2. No unresolved critical reliability defects.
3. Soak test shows stable behavior and recoverability.
