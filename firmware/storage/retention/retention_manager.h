#ifndef RETENTION_MANAGER_H
#define RETENTION_MANAGER_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} retention_manager_config_t;

int retention_manager_init(const retention_manager_config_t *config);
int retention_manager_start(void);
int retention_manager_stop(void);
int retention_manager_tick(void);

#endif
