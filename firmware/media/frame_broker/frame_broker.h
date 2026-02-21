#ifndef FRAME_BROKER_H
#define FRAME_BROKER_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} frame_broker_config_t;

int frame_broker_init(const frame_broker_config_t *config);
int frame_broker_start(void);
int frame_broker_stop(void);
int frame_broker_tick(void);

#endif
