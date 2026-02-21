#ifndef REMOTE_LAYOUT_H
#define REMOTE_LAYOUT_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} remote_layout_config_t;

int remote_layout_init(const remote_layout_config_t *config);
int remote_layout_start(void);
int remote_layout_stop(void);
int remote_layout_tick(void);

#endif
