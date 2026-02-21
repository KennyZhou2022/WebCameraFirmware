#ifndef OTA_SERVICE_H
#define OTA_SERVICE_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} ota_service_config_t;

int ota_service_init(const ota_service_config_t *config);
int ota_service_start(void);
int ota_service_stop(void);
int ota_service_tick(void);

#endif
