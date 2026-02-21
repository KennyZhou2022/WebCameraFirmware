#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} event_bus_config_t;

int event_bus_init(const event_bus_config_t *config);
int event_bus_start(void);
int event_bus_stop(void);
int event_bus_tick(void);

#endif
