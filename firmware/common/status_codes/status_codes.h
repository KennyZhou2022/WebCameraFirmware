#ifndef STATUS_CODES_H
#define STATUS_CODES_H

#include <stdint.h>

enum {
    STATUS_OK = 0,
    STATUS_ERR_INVALID_ARG = -1,
    STATUS_ERR_NOT_READY = -2,
    STATUS_ERR_TIMEOUT = -3,
    STATUS_ERR_IO = -4,
    STATUS_ERR_NO_MEMORY = -5,
    STATUS_ERR_BUSY = -6,
    STATUS_ERR_NOT_FOUND = -7,
    STATUS_ERR_STATE = -8,
    STATUS_ERR_NETWORK = -9,
    STATUS_ERR_AUTH = -10,
    STATUS_ERR_STORAGE = -11,
    STATUS_ERR_INTEGRITY = -12
};

typedef struct {
    uint32_t reserved;
} status_codes_config_t;

int status_codes_init(const status_codes_config_t *config);
int status_codes_start(void);
int status_codes_stop(void);
int status_codes_tick(void);

#endif
