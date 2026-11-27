#include "../include/demo.h"
#if !DEMO_MODE
#ifndef WEATHER_API_KEY
#error "WEATHER_API_KEY missing - add it to .env"
#endif
#include "esp_crt_bundle.h"
#include "esp_http_client.h"
#include <cJSON.h>
#endif
#include "esp_log.h"
#include "esp_timer.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define TAG "weather"
#if !DEMO_MODE
#define BUF_SIZE (16 * 1024)
static const int64_t min_interval_us = (15LL * 60 * 1000000); // minimum time between API requests (15m)
#endif

int temp_c = 0;
int will_it_rain = 0;
int will_it_snow = 0;
int cloud = 0;

int fetch_weather_data(const char *city) {
#if DEMO_MODE
    (void)city;
    typedef struct {
        const char *name;
        int temp_min;
        int temp_max;
        int will_it_rain;
        int will_it_snow;
        int cloud;
    } demo_weather_preset_t;

    static const demo_weather_preset_t presets[] = {
        {"sunny", 18, 30, 0, 0, 0},
        {"cloudy", 10, 20, 0, 0, 1},
        {"rain", 4, 15, 1, 0, 1},
        {"snow", -8, 1, 0, 1, 1},
    };
    static int64_t last_call_us = 0;
    static int last_idx = -1;

    int64_t now = esp_timer_get_time();
    if (last_call_us != 0 && now - last_call_us < DEMO_INTERVAL_US)
        return 0;
    last_call_us = now;

    int count = (int)(sizeof(presets) / sizeof(presets[0]));
    last_idx = demo_pick_other_index(last_idx, count);
    const demo_weather_preset_t *p = &presets[last_idx];

    int span = p->temp_max - p->temp_min + 1;
    temp_c = p->temp_min + (int)(esp_random() % (uint32_t)span);
    will_it_rain = p->will_it_rain;
    will_it_snow = p->will_it_snow;
    cloud = p->cloud;

    ESP_LOGI(TAG, "[DEMO] Weather -> %s (temp=%dC, rain=%d, snow=%d, cloud=%d)", p->name, temp_c, will_it_rain,
             will_it_snow, cloud);
    return 0;
#else
    static int64_t last_call_us = 0; // time of last real API request

    // Rate limit: hit the API at most once per min_interval_us; otherwise keep cached values
    int64_t now = esp_timer_get_time();
    if (last_call_us != 0 && now - last_call_us < min_interval_us)
        return -1;
    last_call_us = now;

    char URL[256];
    snprintf(URL, sizeof(URL), "https://api.weatherapi.com/v1/current.json?key=%s&q=%s", WEATHER_API_KEY, city);

    esp_http_client_config_t cfg = {
        .url = URL,
        .crt_bundle_attach = esp_crt_bundle_attach, // HTTPS cert validation
        .timeout_ms = 10000,
    };
    esp_http_client_handle_t c = esp_http_client_init(&cfg);
    char *buf = malloc(BUF_SIZE * sizeof(*buf));

    if (!buf) {
        ESP_LOGE(TAG, "Failed to allocate memory for buffer");
        return -1;
    }

    if (esp_http_client_open(c, 0) == ESP_OK) {
        esp_http_client_fetch_headers(c);
        int len = esp_http_client_read_response(c, buf, BUF_SIZE - 1);
        if (len > 0) {
            buf[len] = '\0'; // puts '\0' after the data so the buffer becomes a valid C string.
            cJSON *root = cJSON_Parse(buf);
            cJSON *st = cJSON_GetObjectItem(cJSON_GetObjectItem(root, "current"), "temp_c");
            cJSON *rain = cJSON_GetObjectItem(cJSON_GetObjectItem(root, "current"), "will_it_rain");
            cJSON *snow = cJSON_GetObjectItem(cJSON_GetObjectItem(root, "current"), "will_it_snow");
            cJSON *cld = cJSON_GetObjectItem(cJSON_GetObjectItem(root, "current"), "cloud");
            temp_c = cJSON_GetNumberValue(st);
            will_it_rain = cJSON_GetNumberValue(rain);
            will_it_snow = cJSON_GetNumberValue(snow);
            cloud = cJSON_GetNumberValue(cld);
            cJSON_Delete(root);
        }
    }
    esp_http_client_cleanup(c);
    free(buf);
    return 0;
#endif
}

int get_temp(const char *city) {
    fetch_weather_data(city);
    return temp_c;
}

int get_will_it_rain(const char *city) {
    fetch_weather_data(city);
    return will_it_rain;
}

int get_will_it_snow(const char *city) {
    fetch_weather_data(city);
    return will_it_snow;
}

int get_cloud(const char *city) {
    fetch_weather_data(city);
    if (cloud >= 50) {
        return 1;
    } else {
        return 0;
    }
}
