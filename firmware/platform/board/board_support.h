#ifndef BOARD_SUPPORT_H
#define BOARD_SUPPORT_H

#include <stdint.h>

typedef struct {
    uint32_t reserved;
} board_support_config_t;

int board_support_init(const board_support_config_t *config);
int board_support_start(void);
int board_support_stop(void);
int board_support_tick(void);

#endif
