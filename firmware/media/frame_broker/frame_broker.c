#include "frame_broker.h"

#include <string.h>

#include "../../common/status_codes/status_codes.h"

#if defined(__has_include)
#if __has_include("semphr.h")
#include "semphr.h"
#define FRAME_BROKER_USE_FREERTOS_MUTEX 1
#endif
#endif

#ifndef FRAME_BROKER_USE_FREERTOS_MUTEX
#define FRAME_BROKER_USE_FREERTOS_MUTEX 0
#endif

#define FRAME_BROKER_DEFAULT_MAX_BYTES (512U * 1024U)

typedef struct {
    uint8_t *storage;
    uint32_t storage_size;
    uint32_t frame_size;
    uint64_t frame_id;
    uint64_t capture_ts_ms;
    uint32_t width;
    uint32_t height;
    uint8_t quality;
    bool has_frame;
    bool initialized;
    bool started;
} frame_broker_state_t;

static uint8_t s_frame_storage[FRAME_BROKER_DEFAULT_MAX_BYTES];
static frame_broker_state_t s_state = {
    .storage = s_frame_storage,
    .storage_size = FRAME_BROKER_DEFAULT_MAX_BYTES
};

#if FRAME_BROKER_USE_FREERTOS_MUTEX
static SemaphoreHandle_t s_lock;
#endif

static int frame_broker_lock(void)
{
#if FRAME_BROKER_USE_FREERTOS_MUTEX
    if (s_lock == NULL) {
        return STATUS_ERR_NOT_READY;
    }
    if (xSemaphoreTake(s_lock, portMAX_DELAY) != pdTRUE) {
        return STATUS_ERR_BUSY;
    }
#endif
    return STATUS_OK;
}

static void frame_broker_unlock(void)
{
#if FRAME_BROKER_USE_FREERTOS_MUTEX
    if (s_lock != NULL) {
        (void)xSemaphoreGive(s_lock);
    }
#endif
}

int frame_broker_init(const frame_broker_config_t *config)
{
    if (config == NULL) {
        return STATUS_ERR_INVALID_ARG;
    }

    s_state.frame_size = 0U;
    s_state.frame_id = 0U;
    s_state.capture_ts_ms = 0U;
    s_state.width = 0U;
    s_state.height = 0U;
    s_state.quality = 0U;
    s_state.has_frame = false;
    s_state.started = false;

    if (config->max_frame_bytes > 0U && config->max_frame_bytes <= FRAME_BROKER_DEFAULT_MAX_BYTES) {
        s_state.storage_size = config->max_frame_bytes;
    } else {
        s_state.storage_size = FRAME_BROKER_DEFAULT_MAX_BYTES;
    }

#if FRAME_BROKER_USE_FREERTOS_MUTEX
    if (s_lock == NULL) {
        s_lock = xSemaphoreCreateMutex();
        if (s_lock == NULL) {
            return STATUS_ERR_NO_MEMORY;
        }
    }
#endif

    s_state.initialized = true;
    return STATUS_OK;
}

int frame_broker_start(void)
{
    if (!s_state.initialized) {
        return STATUS_ERR_NOT_READY;
    }
    s_state.started = true;
    return STATUS_OK;
}

int frame_broker_stop(void)
{
    if (!s_state.initialized) {
        return STATUS_ERR_NOT_READY;
    }
    s_state.started = false;
    return STATUS_OK;
}

int frame_broker_tick(void)
{
    if (!s_state.initialized) {
        return STATUS_ERR_NOT_READY;
    }
    return STATUS_OK;
}

int frame_broker_publish(const uint8_t *data,
                         uint32_t size,
                         uint64_t capture_ts_ms,
                         uint32_t width,
                         uint32_t height,
                         uint8_t quality)
{
    int rc;

    if (data == NULL || size == 0U) {
        return STATUS_ERR_INVALID_ARG;
    }
    if (!s_state.initialized || !s_state.started) {
        return STATUS_ERR_NOT_READY;
    }
    if (size > s_state.storage_size) {
        return STATUS_ERR_NO_MEMORY;
    }

    rc = frame_broker_lock();
    if (rc != STATUS_OK) {
        return rc;
    }

    memcpy(s_state.storage, data, size);
    s_state.frame_size = size;
    s_state.frame_id++;
    s_state.capture_ts_ms = capture_ts_ms;
    s_state.width = width;
    s_state.height = height;
    s_state.quality = quality;
    s_state.has_frame = true;

    frame_broker_unlock();
    return STATUS_OK;
}

int frame_broker_get_latest(frame_broker_frame_t *out_frame)
{
    int rc;

    if (out_frame == NULL) {
        return STATUS_ERR_INVALID_ARG;
    }
    if (!s_state.initialized || !s_state.started) {
        return STATUS_ERR_NOT_READY;
    }

    rc = frame_broker_lock();
    if (rc != STATUS_OK) {
        return rc;
    }

    if (!s_state.has_frame) {
        frame_broker_unlock();
        return STATUS_ERR_NOT_FOUND;
    }

    out_frame->data = s_state.storage;
    out_frame->size = s_state.frame_size;
    out_frame->frame_id = s_state.frame_id;
    out_frame->capture_ts_ms = s_state.capture_ts_ms;
    out_frame->width = s_state.width;
    out_frame->height = s_state.height;
    out_frame->quality = s_state.quality;

    frame_broker_unlock();
    return STATUS_OK;
}

bool frame_broker_has_frame(void)
{
    return s_state.has_frame;
}
