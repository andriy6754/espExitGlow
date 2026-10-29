#include "../include/functions.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_netif_sntp.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "nvs_flash.h"
#include "tm1637.h"
#include <stdio.h>

#define LED_STRIP_GPIO_PIN 4
#define LED_STRIP_NUM_LEDS 16
#define BLINK_PERIOD_MS 1000
#define TIME_UPDATE_PERIOD_MS 1000

static const char *TAG = "led_strip";
static led_strip_handle_t s_strip = NULL;

static void configure_led_strip(void);

static EventGroupHandle_t s_wifi_eg;
#define WIFI_CONNECTED_BIT BIT0

// Wi-Fi event handler
static void wifi_evt(void *arg, esp_event_base_t base, int32_t id, void *data);

// Initialize Wi-Fi and wait for connection
static void wifi_init(void);

static void alert_strip_task(void *arg);
static void tm1637_show_time_task(void *arg);

void tm1637_show_time(void);

void alert_strip(const int led_number, const char oblast[]);

void app_main(void) {
    configure_led_strip();
    wifi_init();

    // Sync the system clock, otherwise time(NULL) starts at 1970 and the display shows 0200
    esp_sntp_config_t sntp_cfg = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");
    ESP_ERROR_CHECK(esp_netif_sntp_init(&sntp_cfg));
    if (esp_netif_sntp_sync_wait(pdMS_TO_TICKS(15000)) != ESP_OK) {
        ESP_LOGW(TAG, "SNTP sync timed out, time may be wrong");
    }

    configASSERT(xTaskCreate(alert_strip_task, "alert_strip", 4096, NULL, 5, NULL) == pdPASS);
    configASSERT(xTaskCreate(tm1637_show_time_task, "tm1637_time", 2048, NULL, 5, NULL) == pdPASS);
}

static void configure_led_strip(void) {
    led_strip_config_t strip_config = {.strip_gpio_num = LED_STRIP_GPIO_PIN,
                                       .max_leds = LED_STRIP_NUM_LEDS,
                                       .led_model = LED_MODEL_WS2812,
                                       .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
                                       .flags = {
                                           .invert_out = false,
                                       }};

    led_strip_rmt_config_t rmt_config = {.clk_src = RMT_CLK_SRC_DEFAULT,
                                         .resolution_hz = 10 * 1000 * 1000,
                                         .mem_block_symbols = 0,
                                         .flags = {
                                             .with_dma = false,
                                         }};

    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &s_strip));
    ESP_LOGI(TAG, "LED strip initialized on GPIO %d (%d LEDs)", LED_STRIP_GPIO_PIN, LED_STRIP_NUM_LEDS);
}

static void wifi_evt(void *arg, esp_event_base_t base, int32_t id, void *data) {
    if (base == WIFI_EVENT && id == WIFI_EVENT_STA_START)
        esp_wifi_connect();
    else if (base == WIFI_EVENT && id == WIFI_EVENT_STA_DISCONNECTED)
        esp_wifi_connect();
    else if (base == IP_EVENT && id == IP_EVENT_STA_GOT_IP)
        xEventGroupSetBits(s_wifi_eg, WIFI_CONNECTED_BIT);
}

static void wifi_init(void) {
    esp_err_t r = nvs_flash_init();
    if (r == ESP_ERR_NVS_NO_FREE_PAGES || r == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        r = nvs_flash_init();
    }
    ESP_ERROR_CHECK(r);
    s_wifi_eg = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_evt, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_evt, NULL));
    wifi_config_t wc = {.sta = {.ssid = "Wokwi-GUEST", .password = ""}};
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wc));
    ESP_ERROR_CHECK(esp_wifi_start());
    xEventGroupWaitBits(s_wifi_eg, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
}

void alert_strip(const int led_number, const char oblast[]) {
    while (1) {
        int alert_status = air_alert_check(oblast);

        if (alert_status == 0) {
            // Turn all LEDs green if no alert
            for (int i = 0; i < led_number; i++) {
                ESP_ERROR_CHECK(led_strip_set_pixel(s_strip, i, 0, 255, 0));
            }
            ESP_ERROR_CHECK(led_strip_refresh(s_strip));
            ESP_LOGI(TAG, "LED Strip is green");
        } else if (alert_status == 1) {
            // Turn all LEDs red if there is an alert
            for (int i = 0; i < led_number; i++) {
                ESP_ERROR_CHECK(led_strip_set_pixel(s_strip, i, 255, 0, 0));
            }
            ESP_ERROR_CHECK(led_strip_refresh(s_strip));
            ESP_LOGI(TAG, "LED Strip is red");
        }

        vTaskDelay(pdMS_TO_TICKS(BLINK_PERIOD_MS));
    }
}

static void alert_strip_task(void *arg) {
    (void)arg;
    alert_strip(LED_STRIP_NUM_LEDS, "м. Київ");
}

void display_time(tm1637_handle_t display, int hour, int minute) {
    // Format: HH:MM (use colon segment if available)
    uint8_t time_display[4];
    time_display[0] = tm1637_encode_digit(hour / 10);
    time_display[1] = tm1637_encode_digit(hour % 10) | TM1637_SEG_DP;
    time_display[2] = tm1637_encode_digit(minute / 10);
    time_display[3] = tm1637_encode_digit(minute % 10);

    tm1637_set_segments(display, time_display, 4, 0);
}

void tm1637_show_time(void) {
    // Configure pins
    tm1637_config_t config = {.clk_pin = GPIO_NUM_16, .dio_pin = GPIO_NUM_17, .bit_delay_us = 100};

    // Initialize display
    tm1637_handle_t display;
    tm1637_init(&config, &display);

    // Set brightness (0-7)
    tm1637_set_brightness(display, 5, true);

    while (1) {
        display_time(display, get_current_hour(), get_current_minute());
        vTaskDelay(pdMS_TO_TICKS(TIME_UPDATE_PERIOD_MS));
    }
}

static void tm1637_show_time_task(void *arg) {
    (void)arg;
    tm1637_show_time();
}