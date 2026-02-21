#ifndef SEGMENTER_H
#define SEGMENTER_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} segmenter_config_t;

int segmenter_init(const segmenter_config_t *config);
int segmenter_start(void);
int segmenter_stop(void);
int segmenter_tick(void);

#endif
