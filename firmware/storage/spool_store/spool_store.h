#ifndef SPOOL_STORE_H
#define SPOOL_STORE_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} spool_store_config_t;

int spool_store_init(const spool_store_config_t *config);
int spool_store_start(void);
int spool_store_stop(void);
int spool_store_tick(void);

#endif
