#include "../include/functions.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "nvs_flash.h"
#include <stdio.h>

#define LED_STRIP_GPIO_PIN 4
#define LED_STRIP_NUM_LEDS 16
#define BLINK_PERIOD_MS 1000

static const char *TAG = "led_strip";
static led_strip_handle_t s_strip = NULL;

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

static EventGroupHandle_t s_wifi_eg;
#define WIFI_CONNECTED_BIT BIT0

// Wi-Fi event handler
static void wifi_evt(void *arg, esp_event_base_t base, int32_t id, void *data) {
    if (base == WIFI_EVENT && id == WIFI_EVENT_STA_START)
        esp_wifi_connect();
    else if (base == WIFI_EVENT && id == WIFI_EVENT_STA_DISCONNECTED)
        esp_wifi_connect();
    else if (base == IP_EVENT && id == IP_EVENT_STA_GOT_IP)
        xEventGroupSetBits(s_wifi_eg, WIFI_CONNECTED_BIT);
}

// Initialize Wi-Fi and wait for connection
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

void app_main(void) {
    wifi_init();
    configure_led_strip();

    bool strip_on = false;

    const char oblast[] = "м. Київ";

    while (1) {
        strip_on = !strip_on;

        if (air_alert_check(oblast) == 0) {
            // Turn all LEDs green if no alert
            for (int i = 0; i < LED_STRIP_NUM_LEDS; i++) {
                ESP_ERROR_CHECK(led_strip_set_pixel(s_strip, i, 0, 255, 0));
            }
            ESP_ERROR_CHECK(led_strip_refresh(s_strip));
            ESP_LOGI(TAG, "LED Strip is green");
        } else if (air_alert_check(oblast) == 1) {
            // Turn all LEDs red if there is an alert
            for (int i = 0; i < LED_STRIP_NUM_LEDS; i++) {
                ESP_ERROR_CHECK(led_strip_set_pixel(s_strip, i, 255, 0, 0));
            }
            ESP_ERROR_CHECK(led_strip_refresh(s_strip));
            ESP_LOGI(TAG, "LED Strip is red");
        }

        vTaskDelay(pdMS_TO_TICKS(BLINK_PERIOD_MS));
    }
}
