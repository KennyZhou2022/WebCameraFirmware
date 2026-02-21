#ifndef METRICS_SERVICE_H
#define METRICS_SERVICE_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} metrics_service_config_t;

int metrics_service_init(const metrics_service_config_t *config);
int metrics_service_start(void);
int metrics_service_stop(void);
int metrics_service_tick(void);

#endif
