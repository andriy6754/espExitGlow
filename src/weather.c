#ifndef WEATHER_API_KEY
#error "WEATHER_API_KEY missing - add it to .env"
#endif
#include "esp_crt_bundle.h"
#include "esp_http_client.h"
#include "esp_timer.h"
#include <cJSON.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE (16 * 1024)
static const int min_interval_us = (30 * 60 * 1000000); // minimum time between API requests (30m)

int temp_c = 0;
int will_it_rain = 0;
int will_it_snow = 0;
int cloud = 0;

void fetch_weather_data(const char *city) {
    static double last_call_us = 0; // time of last real API request

    // Rate limit: hit the API at most once per min_interval_us; otherwise keep cached values
    double now = esp_timer_get_time();
    if (last_call_us != 0 && now - last_call_us < min_interval_us)
        return;
    last_call_us = now;

    char URL[256];
    snprintf(URL, sizeof(URL), "https://api.weatherapi.com/v1/current.json?key=%s&q=%s", WEATHER_API_KEY, city);

    esp_http_client_config_t cfg = {
        .url = URL,
        .crt_bundle_attach = esp_crt_bundle_attach, // HTTPS cert validation
        .timeout_ms = 10000,
    };
    esp_http_client_handle_t c = esp_http_client_init(&cfg);

    static char buf[BUF_SIZE];

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
    return cloud;
}