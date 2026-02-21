#ifndef UPLOAD_QUEUE_H
#define UPLOAD_QUEUE_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} upload_queue_config_t;

int upload_queue_init(const upload_queue_config_t *config);
int upload_queue_start(void);
int upload_queue_stop(void);
int upload_queue_tick(void);

#endif
