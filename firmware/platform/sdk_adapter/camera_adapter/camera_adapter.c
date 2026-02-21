#include "camera_adapter.h"

#include <time.h>

#include "../../../common/status_codes/status_codes.h"

/* SDK symbols from imported AiPi-CAM-D200 code path. */
extern uint8_t *d200_camera_output_buff_addr(void);
extern uint32_t d200_camera_output_buff_length(void);
extern void d200_camera_start(void);
extern void d200_camera_stop(void);

static camera_adapter_config_t s_config;
static bool s_initialized = false;
static bool s_started = false;

static uint64_t camera_adapter_now_ms(void)
{
    return (uint64_t)time(NULL) * 1000ULL;
}

int camera_adapter_init(const camera_adapter_config_t *config)
{
    if (config == NULL) {
        return STATUS_ERR_INVALID_ARG;
    }

    s_config = *config;
    s_initialized = true;
    s_started = false;
    return STATUS_OK;
}

int camera_adapter_start(void)
{
    if (!s_initialized) {
        return STATUS_ERR_NOT_READY;
    }

    d200_camera_start();
    s_started = true;
    return STATUS_OK;
}

int camera_adapter_stop(void)
{
    if (!s_initialized) {
        return STATUS_ERR_NOT_READY;
    }

    d200_camera_stop();
    s_started = false;
    return STATUS_OK;
}

int camera_adapter_tick(void)
{
    if (!s_initialized) {
        return STATUS_ERR_NOT_READY;
    }
    return STATUS_OK;
}

int camera_adapter_get_frame(camera_adapter_frame_t *out_frame)
{
    uint8_t *buf;
    uint32_t len;

    if (out_frame == NULL) {
        return STATUS_ERR_INVALID_ARG;
    }
    if (!s_initialized || !s_started) {
        return STATUS_ERR_NOT_READY;
    }

    buf = d200_camera_output_buff_addr();
    len = d200_camera_output_buff_length();
    if (buf == NULL || len == 0U) {
        return STATUS_ERR_NOT_FOUND;
    }

    out_frame->data = buf;
    out_frame->size = len;
    out_frame->width = s_config.width;
    out_frame->height = s_config.height;
    out_frame->quality = s_config.jpeg_quality;
    out_frame->capture_ts_ms = camera_adapter_now_ms();
    return STATUS_OK;
}

bool camera_adapter_is_started(void)
{
    return s_started;
}
