#include "live_http_mjpeg.h"

#include "../../common/status_codes/status_codes.h"
#include "../../media/frame_broker/frame_broker.h"

static live_http_mjpeg_config_t s_config;
static bool s_initialized = false;
static bool s_started = false;
static uint64_t s_last_frame_id = 0U;

int live_http_mjpeg_init(const live_http_mjpeg_config_t *config)
{
    if (config == NULL) {
        return STATUS_ERR_INVALID_ARG;
    }

    s_config = *config;
    if (s_config.port == 0U) {
        s_config.port = 80U;
    }
    if (s_config.max_clients == 0U) {
        s_config.max_clients = 1U;
    }
    s_initialized = true;
    s_started = false;
    s_last_frame_id = 0U;
    return STATUS_OK;
}

int live_http_mjpeg_start(void)
{
    if (!s_initialized) {
        return STATUS_ERR_NOT_READY;
    }
    s_started = true;
    return STATUS_OK;
}

int live_http_mjpeg_stop(void)
{
    if (!s_initialized) {
        return STATUS_ERR_NOT_READY;
    }
    s_started = false;
    return STATUS_OK;
}

int live_http_mjpeg_tick(void)
{
    frame_broker_frame_t frame;
    int rc;

    if (!s_initialized) {
        return STATUS_ERR_NOT_READY;
    }
    if (!s_started) {
        return STATUS_OK;
    }

    rc = frame_broker_get_latest(&frame);
    if (rc != STATUS_OK) {
        return rc;
    }

    if (frame.frame_id == s_last_frame_id) {
        return STATUS_OK;
    }

    /*
     * Phase 1 wiring point:
     * Here we already consume frames from frame_broker.
     * Socket server/client fan-out will be implemented in next increment.
     */
    s_last_frame_id = frame.frame_id;
    return STATUS_OK;
}

bool live_http_mjpeg_is_started(void)
{
    return s_started;
}

void mhttp_server_init(void)
{
    /* Legacy entry replacement: start modern service once. */
    (void)live_http_mjpeg_start();
}
