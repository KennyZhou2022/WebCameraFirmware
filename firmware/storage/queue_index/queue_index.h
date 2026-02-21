#ifndef QUEUE_INDEX_H
#define QUEUE_INDEX_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} queue_index_config_t;

int queue_index_init(const queue_index_config_t *config);
int queue_index_start(void);
int queue_index_stop(void);
int queue_index_tick(void);

#endif
