#ifndef SNAPSHOT_SERVICE_H
#define SNAPSHOT_SERVICE_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} snapshot_service_config_t;

int snapshot_service_init(const snapshot_service_config_t *config);
int snapshot_service_start(void);
int snapshot_service_stop(void);
int snapshot_service_tick(void);

#endif
