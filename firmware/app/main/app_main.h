#ifndef APP_MAIN_H
#define APP_MAIN_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} app_main_config_t;

int app_main_init(const app_main_config_t *config);
int app_main_start(void);
int app_main_stop(void);
int app_main_tick(void);

#endif
