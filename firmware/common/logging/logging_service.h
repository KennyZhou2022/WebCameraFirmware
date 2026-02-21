#ifndef LOGGING_SERVICE_H
#define LOGGING_SERVICE_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} logging_service_config_t;

int logging_service_init(const logging_service_config_t *config);
int logging_service_start(void);
int logging_service_stop(void);
int logging_service_tick(void);

#endif
