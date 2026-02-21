#ifndef FRAME_POOL_H
#define FRAME_POOL_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} frame_pool_config_t;

int frame_pool_init(const frame_pool_config_t *config);
int frame_pool_start(void);
int frame_pool_stop(void);
int frame_pool_tick(void);

#endif
