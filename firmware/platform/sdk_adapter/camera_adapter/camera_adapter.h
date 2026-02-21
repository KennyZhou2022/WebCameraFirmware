#ifndef CAMERA_ADAPTER_H
#define CAMERA_ADAPTER_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} camera_adapter_config_t;

int camera_adapter_init(const camera_adapter_config_t *config);
int camera_adapter_start(void);
int camera_adapter_stop(void);
int camera_adapter_tick(void);

#endif
