#ifndef NTP_TIME_H
#define NTP_TIME_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} ntp_time_config_t;

int ntp_time_init(const ntp_time_config_t *config);
int ntp_time_start(void);
int ntp_time_stop(void);
int ntp_time_tick(void);

#endif
