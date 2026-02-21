#ifndef WEBDAV_CLIENT_H
#define WEBDAV_CLIENT_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} webdav_client_config_t;

int webdav_client_init(const webdav_client_config_t *config);
int webdav_client_start(void);
int webdav_client_stop(void);
int webdav_client_tick(void);

#endif
