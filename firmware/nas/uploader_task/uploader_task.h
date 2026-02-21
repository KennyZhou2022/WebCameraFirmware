#ifndef UPLOADER_TASK_H
#define UPLOADER_TASK_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} uploader_task_config_t;

int uploader_task_init(const uploader_task_config_t *config);
int uploader_task_start(void);
int uploader_task_stop(void);
int uploader_task_tick(void);

#endif
