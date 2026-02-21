#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} config_manager_config_t;

int config_manager_init(const config_manager_config_t *config);
int config_manager_start(void);
int config_manager_stop(void);
int config_manager_tick(void);

#endif
