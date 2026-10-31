#include "../include/air_alert.h"
#include "../include/led.h"
#include "esp_crt_bundle.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <cJSON.h>
#include <string.h>

#define BLINK_PERIOD_MS 1000
#define TAG "air_alert"
#define URL "https://ubilling.net.ua/aerialalerts/"
#define BUF_SIZE (16 * 1024)
static const int min_interval_us = (1000 * 1000); // minimum time between API requests (1 s)

// Returns 1 = alert, 0 = no alert, -1 = error
int air_alert_check(const char region[]) {
    static double last_call_us = 0; // time of last real API request
    static int last_result = -1;    // result of last real API request

    // Rate limit: hit the API once per second, to not get banned
    double now = esp_timer_get_time();
    if (last_call_us != 0 && now - last_call_us < min_interval_us)
        return last_result;
    last_call_us = now;

    int result = -1;
    char *buf = malloc(BUF_SIZE * sizeof(*buf));

    if (!buf) {
        ESP_LOGE(TAG, "Failed to allocate memory for buffer");
        return -1;
    }

    esp_http_client_config_t cfg = {
        .url = URL,
        .crt_bundle_attach = esp_crt_bundle_attach, // HTTPS cert validation
        .timeout_ms = 10000,
    };
    esp_http_client_handle_t c = esp_http_client_init(&cfg); // use pointer to save space

    if (esp_http_client_open(c, 0) == ESP_OK) {
        esp_http_client_fetch_headers(c);
        int len = esp_http_client_read_response(c, buf, BUF_SIZE - 1);
        if (len > 0) {
            buf[len] = '\0'; // puts '\0' after the data so the buffer becomes a valid C string.
            cJSON *root = cJSON_Parse(buf);
            cJSON *st = cJSON_GetObjectItem(cJSON_GetObjectItem(root, "states"), region);
            cJSON *an = cJSON_GetObjectItem(st, "alertnow");
            if (cJSON_IsBool(an))
                result = cJSON_IsTrue(an);
            else
                ESP_LOGW(TAG, "region '%s' not found", region);
            cJSON_Delete(root); // safe on NULL
        }
    }
    esp_http_client_cleanup(c);
    free(buf);
    last_result = result;
    return result;
}

void alert_strip_task(void *arg) {
    (void)arg;
    const char *oblast = "м. Київ";
    led_strip_handle_t strip = get_led_strip();

    while (1) {
        int alert_status = air_alert_check(oblast);

        if (alert_status == 0) {
            // Turn all LEDs green if no alert
            for (int i = 0; i < LED_STRIP_NUM_LEDS; i++) {
                ESP_ERROR_CHECK(led_strip_set_pixel(strip, i, 0, 255, 0));
            }
            ESP_ERROR_CHECK(led_strip_refresh(strip));
            ESP_LOGI(TAG, "LED Strip is green");
        } else if (alert_status == 1) {
            // Turn all LEDs red if there is an alert
            for (int i = 0; i < LED_STRIP_NUM_LEDS; i++) {
                ESP_ERROR_CHECK(led_strip_set_pixel(strip, i, 255, 0, 0));
            }
            ESP_ERROR_CHECK(led_strip_refresh(strip));
            ESP_LOGI(TAG, "LED Strip is red");
        }

        vTaskDelay(pdMS_TO_TICKS(BLINK_PERIOD_MS));
    }
}