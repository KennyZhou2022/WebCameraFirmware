#ifndef FRAME_BROKER_H
#define FRAME_BROKER_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t max_frame_bytes;
} frame_broker_config_t;

typedef struct {
    const uint8_t *data;
    uint32_t size;
    uint64_t frame_id;
    uint64_t capture_ts_ms;
    uint32_t width;
    uint32_t height;
    uint8_t quality;
} frame_broker_frame_t;

int frame_broker_init(const frame_broker_config_t *config);
int frame_broker_start(void);
int frame_broker_stop(void);
int frame_broker_tick(void);
int frame_broker_publish(const uint8_t *data,
                         uint32_t size,
                         uint64_t capture_ts_ms,
                         uint32_t width,
                         uint32_t height,
                         uint8_t quality);
int frame_broker_get_latest(frame_broker_frame_t *out_frame);
bool frame_broker_has_frame(void);

#endif
