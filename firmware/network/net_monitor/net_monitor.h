#ifndef NET_MONITOR_H
#define NET_MONITOR_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} net_monitor_config_t;

int net_monitor_init(const net_monitor_config_t *config);
int net_monitor_start(void);
int net_monitor_stop(void);
int net_monitor_tick(void);

#endif
