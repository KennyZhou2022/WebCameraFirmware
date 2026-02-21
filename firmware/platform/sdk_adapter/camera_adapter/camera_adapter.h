#ifndef CAMERA_ADAPTER_H
#define CAMERA_ADAPTER_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t width;
    uint32_t height;
    uint8_t jpeg_quality;
} camera_adapter_config_t;

typedef struct {
    const uint8_t *data;
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint8_t quality;
    uint64_t capture_ts_ms;
} camera_adapter_frame_t;

int camera_adapter_init(const camera_adapter_config_t *config);
int camera_adapter_start(void);
int camera_adapter_stop(void);
int camera_adapter_tick(void);
int camera_adapter_get_frame(camera_adapter_frame_t *out_frame);
bool camera_adapter_is_started(void);

#endif
