#ifndef APP_MAIN_H
#define APP_MAIN_H

#include <stdint.h>

typedef struct {
    uint32_t frame_max_bytes;
    uint32_t frame_width;
    uint32_t frame_height;
    uint8_t jpeg_quality;
    uint16_t http_port;
    uint8_t http_max_clients;
} app_main_config_t;

int app_main_init(const app_main_config_t *config);
int app_main_start(void);
int app_main_stop(void);
int app_main_tick(void);
int app_main_on_wifi_ready(void);
int app_main_on_wifi_lost(void);

#endif
