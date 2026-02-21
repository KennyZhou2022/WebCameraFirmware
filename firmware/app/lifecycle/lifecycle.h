#ifndef LIFECYCLE_H
#define LIFECYCLE_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} lifecycle_config_t;

int lifecycle_init(const lifecycle_config_t *config);
int lifecycle_start(void);
int lifecycle_stop(void);
int lifecycle_tick(void);

#endif
