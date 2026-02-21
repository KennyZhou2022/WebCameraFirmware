#ifndef TLS_SERVICE_H
#define TLS_SERVICE_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} tls_service_config_t;

int tls_service_init(const tls_service_config_t *config);
int tls_service_start(void);
int tls_service_stop(void);
int tls_service_tick(void);

#endif
