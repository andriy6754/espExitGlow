#include "../include/air_alert.h"
#include "../include/display.h"
#include "../include/led.h"
#include "../include/wifi.h"
#include "../include/audio.h"
#include "../include/led_screen.h"
#include "esp_log.h"
#include "esp_netif_sntp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "led_strip"

rgb_t stripArr[8][8];

void app_main(void) {
    configure_led_strip();
    bool wifi_connected = wifi_init();
    audio_init();

    // Sync the system clock, otherwise time(NULL) starts at 1970 and the display shows 0200
    esp_sntp_config_t sntp_cfg = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");
    ESP_ERROR_CHECK(esp_netif_sntp_init(&sntp_cfg));
    if (wifi_connected && esp_netif_sntp_sync_wait(pdMS_TO_TICKS(15000)) != ESP_OK) {
        ESP_LOGW(TAG, "SNTP sync timed out, time may be wrong");
    }

    configASSERT(xTaskCreate(alert_strip_task, "alert_strip", 8192, NULL, 5, NULL) == pdPASS);
    configASSERT(xTaskCreate(tm1637_show_time_task, "tm1637_time", 2048, NULL, 5, NULL) == pdPASS);
    configASSERT(xTaskCreate(led_screen_task, "led_screen", 4096, NULL, 5, NULL) == pdPASS);
}
