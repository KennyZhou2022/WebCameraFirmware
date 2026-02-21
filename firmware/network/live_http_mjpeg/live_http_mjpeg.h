#ifndef LIVE_HTTP_MJPEG_H
#define LIVE_HTTP_MJPEG_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint16_t port;
    uint8_t max_clients;
    bool auth_enabled;
} live_http_mjpeg_config_t;

int live_http_mjpeg_init(const live_http_mjpeg_config_t *config);
int live_http_mjpeg_start(void);
int live_http_mjpeg_stop(void);
int live_http_mjpeg_tick(void);
bool live_http_mjpeg_is_started(void);

/* Legacy compatibility hook to replace old SDK mhttp_server_init entry. */
void mhttp_server_init(void);

#endif
