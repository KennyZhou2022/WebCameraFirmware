#ifndef STATUS_CODES_H
#define STATUS_CODES_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} status_codes_config_t;

int status_codes_init(const status_codes_config_t *config);
int status_codes_start(void);
int status_codes_stop(void);
int status_codes_tick(void);

#endif
