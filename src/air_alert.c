#include "../include/air_alert.h"
#include "../include/audio.h"
#include "../include/led.h"
#include "esp_crt_bundle.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <cJSON.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define REGION_SLUG "kyiv" // Change this to your desired region
#define BLINK_PERIOD_MS 1000
#define TAG "air_alert"
#define API_BASE_URL "https://tryvoha.online/api/v1/alerts"
#define BUF_SIZE (4 * 1024)
static const int64_t min_interval_us = (30 * 1000 * 1000); // API caches responses for 30s
static const int64_t error_retry_us = (5 * 1000 * 1000);   // Retry sooner after a failed request

static air_alert_level_t parse_alert_level(cJSON *root) {
    if (!root)
        return AIR_ALERT_ERROR;

    cJSON *active = cJSON_GetObjectItem(root, "active_anywhere");
    if (!cJSON_IsBool(active))
        active = cJSON_GetObjectItem(root, "active");
    if (!cJSON_IsBool(active))
        return AIR_ALERT_ERROR;
    if (!cJSON_IsTrue(active))
        return AIR_ALERT_NONE;

    air_alert_level_t highest = AIR_ALERT_NONE;

    cJSON *level = cJSON_GetObjectItem(root, "level");
    if (cJSON_IsString(level) && level->valuestring) {
        if (strcmp(level->valuestring, "red") == 0)
            return AIR_ALERT_RED;
        if (strcmp(level->valuestring, "yellow") == 0)
            highest = AIR_ALERT_YELLOW;
    }

    cJSON *districts = cJSON_GetObjectItem(root, "districts_active");
    if (cJSON_IsArray(districts)) {
        cJSON *district = NULL;
        cJSON_ArrayForEach(district, districts) {
            cJSON *dlevel = cJSON_GetObjectItem(district, "level");
            if (cJSON_IsString(dlevel) && dlevel->valuestring) {
                if (strcmp(dlevel->valuestring, "red") == 0)
                    return AIR_ALERT_RED;
                if (strcmp(dlevel->valuestring, "yellow") == 0)
                    highest = AIR_ALERT_YELLOW;
            }
        }
    }

    // Fallback: active is true but level wasn't recognized — default to red
    return highest != AIR_ALERT_NONE ? highest : AIR_ALERT_RED;
}

air_alert_level_t air_alert_check(const char *region) {
    static int64_t last_call_us = 0;                        // time of last real API request
    static air_alert_level_t last_result = AIR_ALERT_ERROR; // result of last real API request

    if (!region || region[0] == '\0') {
        ESP_LOGE(TAG, "Invalid region slug");
        return AIR_ALERT_ERROR;
    }

    // Rate limit: API caches responses for 30-60 s (120 req/min limit), retry sooner on error
    int64_t now = esp_timer_get_time();
    int64_t interval = (last_result == AIR_ALERT_ERROR) ? error_retry_us : min_interval_us;
    if (last_call_us != 0 && now - last_call_us < interval)
        return last_result;
    last_call_us = now;

    char url[128];
    int url_len = snprintf(url, sizeof(url), "%s/%s", API_BASE_URL, region);
    if (url_len < 0 || url_len >= (int)sizeof(url)) {
        ESP_LOGE(TAG, "Region slug too long: '%s'", region);
        return AIR_ALERT_ERROR;
    }

    air_alert_level_t result = AIR_ALERT_ERROR;
    char *buf = malloc(BUF_SIZE * sizeof(*buf));

    if (!buf) {
        ESP_LOGE(TAG, "Failed to allocate memory for buffer");
        return AIR_ALERT_ERROR;
    }

    esp_http_client_config_t cfg = {
        .url = url,
        .crt_bundle_attach = esp_crt_bundle_attach, // HTTPS cert validation
        .timeout_ms = 20000,
        .buffer_size = 1024,
    };
    esp_http_client_handle_t c = esp_http_client_init(&cfg); // use pointer to save space
    if (!c) {
        ESP_LOGE(TAG, "Failed to initialize HTTP client");
        free(buf);
        return AIR_ALERT_ERROR;
    }

    ESP_LOGI(TAG, "Fetching %s", url);
    esp_err_t err = esp_http_client_open(c, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "HTTP open failed for '%s': %s", url, esp_err_to_name(err));
    } else {
        int64_t hdr_ret = esp_http_client_fetch_headers(c);
        if (hdr_ret < 0) {
            ESP_LOGE(TAG, "HTTP fetch headers failed for '%s': %lld", url, (long long)hdr_ret);
        } else {
            int status = esp_http_client_get_status_code(c);
            int len = esp_http_client_read_response(c, buf, BUF_SIZE - 1);
            if (status == 200 && len > 0) {
                buf[len] = '\0'; // puts '\0' after the data so the buffer becomes a valid C string.
                cJSON *root = cJSON_Parse(buf);
                result = parse_alert_level(root);
                if (result == AIR_ALERT_ERROR)
                    ESP_LOGW(TAG, "Unexpected response for region '%s': %.200s", region, buf);
                else
                    ESP_LOGI(TAG, "Alert level for '%s': %d (HTTP %d, %d bytes)", region, result, status, len);
                cJSON_Delete(root); // safe on NULL
            } else {
                ESP_LOGW(TAG, "HTTP %d (len=%d) for region '%s'", status, len, region);
            }
        }
    }
    esp_http_client_cleanup(c);
    free(buf);
    last_result = result;
    return result;
}

void alert_strip_task(void *arg) {
    (void)arg;
    air_alert_level_t prev_status = AIR_ALERT_NONE;
    rgb_t green = {0, 255, 0};
    rgb_t yellow = {255, 255, 0};
    rgb_t red = {255, 0, 0};

    while (1) {
        air_alert_level_t alert_status = air_alert_check(REGION_SLUG);

        if (alert_status == AIR_ALERT_NONE) {
            // Turn all LEDs green if no alert
            led_set_color(0, LED_STRIP_NUM_LEDS, green);
            ESP_LOGI(TAG, "LED Strip is green");
        } else if (alert_status == AIR_ALERT_YELLOW) {
            // Turn all LEDs yellow for a yellow alert (drone threat)
            led_set_color(0, LED_STRIP_NUM_LEDS, yellow);
            ESP_LOGI(TAG, "LED Strip is yellow");
        } else if (alert_status == AIR_ALERT_RED) {
            // Turn all LEDs red for a red alert
            led_set_color(0, LED_STRIP_NUM_LEDS, red);
            ESP_LOGI(TAG, "LED Strip is red");
        }

        bool known =
            (alert_status == AIR_ALERT_NONE || alert_status == AIR_ALERT_YELLOW || alert_status == AIR_ALERT_RED);

        if (known) {
            bool to_red = (alert_status == AIR_ALERT_RED && prev_status != AIR_ALERT_RED);
            bool to_yellow = (alert_status == AIR_ALERT_YELLOW && prev_status == AIR_ALERT_NONE);
            bool to_none = (alert_status == AIR_ALERT_NONE && prev_status != AIR_ALERT_NONE);

            if (to_red || to_yellow)
                play_alarm();
            if (to_none)
                play_cancel();

            prev_status = alert_status;
        }

        vTaskDelay(pdMS_TO_TICKS(BLINK_PERIOD_MS));
    }
}