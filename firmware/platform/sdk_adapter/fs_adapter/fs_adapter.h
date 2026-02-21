#ifndef FS_ADAPTER_H
#define FS_ADAPTER_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} fs_adapter_config_t;

int fs_adapter_init(const fs_adapter_config_t *config);
int fs_adapter_start(void);
int fs_adapter_stop(void);
int fs_adapter_tick(void);

#endif
