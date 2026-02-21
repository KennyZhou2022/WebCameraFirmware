#ifndef WIFI_ADAPTER_H
#define WIFI_ADAPTER_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} wifi_adapter_config_t;

int wifi_adapter_init(const wifi_adapter_config_t *config);
int wifi_adapter_start(void);
int wifi_adapter_stop(void);
int wifi_adapter_tick(void);

#endif
