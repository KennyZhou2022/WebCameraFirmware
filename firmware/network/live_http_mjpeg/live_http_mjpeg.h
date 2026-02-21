#ifndef LIVE_HTTP_MJPEG_H
#define LIVE_HTTP_MJPEG_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} live_http_mjpeg_config_t;

int live_http_mjpeg_init(const live_http_mjpeg_config_t *config);
int live_http_mjpeg_start(void);
int live_http_mjpeg_stop(void);
int live_http_mjpeg_tick(void);

#endif
