#include "app_main.h"

#include <stdbool.h>

#include "../../common/status_codes/status_codes.h"
#include "../../media/frame_broker/frame_broker.h"
#include "../../network/live_http_mjpeg/live_http_mjpeg.h"
#include "../../platform/sdk_adapter/camera_adapter/camera_adapter.h"

static bool s_initialized = false;
static bool s_started = false;

int app_main_init(const app_main_config_t *config)
{
    camera_adapter_config_t camera_cfg;
    frame_broker_config_t broker_cfg;
    live_http_mjpeg_config_t http_cfg;
    int rc;

    if (config == NULL) {
        return STATUS_ERR_INVALID_ARG;
    }

    camera_cfg.width = config->frame_width;
    camera_cfg.height = config->frame_height;
    camera_cfg.jpeg_quality = config->jpeg_quality;
    rc = camera_adapter_init(&camera_cfg);
    if (rc != STATUS_OK) {
        return rc;
    }

    broker_cfg.max_frame_bytes = config->frame_max_bytes;
    rc = frame_broker_init(&broker_cfg);
    if (rc != STATUS_OK) {
        return rc;
    }

    http_cfg.port = config->http_port;
    http_cfg.max_clients = config->http_max_clients;
    http_cfg.auth_enabled = false;
    rc = live_http_mjpeg_init(&http_cfg);
    if (rc != STATUS_OK) {
        return rc;
    }

    s_initialized = true;
    s_started = false;
    return STATUS_OK;
}

int app_main_start(void)
{
    int rc;

    if (!s_initialized) {
        return STATUS_ERR_NOT_READY;
    }

    rc = camera_adapter_start();
    if (rc != STATUS_OK) {
        return rc;
    }
    rc = frame_broker_start();
    if (rc != STATUS_OK) {
        return rc;
    }

    s_started = true;
    return STATUS_OK;
}

int app_main_stop(void)
{
    int rc;

    if (!s_initialized) {
        return STATUS_ERR_NOT_READY;
    }

    (void)live_http_mjpeg_stop();
    rc = frame_broker_stop();
    if (rc != STATUS_OK) {
        return rc;
    }
    rc = camera_adapter_stop();
    if (rc != STATUS_OK) {
        return rc;
    }

    s_started = false;
    return STATUS_OK;
}

int app_main_tick(void)
{
    camera_adapter_frame_t frame;
    int rc;

    if (!s_initialized || !s_started) {
        return STATUS_ERR_NOT_READY;
    }

    rc = camera_adapter_tick();
    if (rc != STATUS_OK) {
        return rc;
    }

    rc = camera_adapter_get_frame(&frame);
    if (rc == STATUS_OK) {
        rc = frame_broker_publish(frame.data,
                                  frame.size,
                                  frame.capture_ts_ms,
                                  frame.width,
                                  frame.height,
                                  frame.quality);
        if (rc != STATUS_OK) {
            return rc;
        }
    } else if (rc != STATUS_ERR_NOT_FOUND) {
        return rc;
    }

    rc = live_http_mjpeg_tick();
    if (rc != STATUS_OK && rc != STATUS_ERR_NOT_FOUND) {
        return rc;
    }

    return STATUS_OK;
}

int app_main_on_wifi_ready(void)
{
    if (!s_initialized) {
        return STATUS_ERR_NOT_READY;
    }
    return live_http_mjpeg_start();
}

int app_main_on_wifi_lost(void)
{
    if (!s_initialized) {
        return STATUS_ERR_NOT_READY;
    }
    return live_http_mjpeg_stop();
}
