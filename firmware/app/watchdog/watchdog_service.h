#ifndef WATCHDOG_SERVICE_H
#define WATCHDOG_SERVICE_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} watchdog_service_config_t;

int watchdog_service_init(const watchdog_service_config_t *config);
int watchdog_service_start(void);
int watchdog_service_stop(void);
int watchdog_service_tick(void);

#endif
