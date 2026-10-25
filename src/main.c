#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
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

void app_main(void) {
    configure_led_strip();

    bool strip_on = false;

    while (1) {
        strip_on = !strip_on;

        if (strip_on) {
            // Turn all LEDs ON
            for (int i = 0; i < LED_STRIP_NUM_LEDS; i++) {
                ESP_ERROR_CHECK(led_strip_set_pixel(s_strip, i, 0, 255, 0));
            }
            ESP_ERROR_CHECK(led_strip_refresh(s_strip));
            ESP_LOGI(TAG, "LED Strip: ON");
        } else {
            // Turn all LEDs OFF
            ESP_ERROR_CHECK(led_strip_clear(s_strip));
            ESP_LOGI(TAG, "LED Strip: OFF");
        }

        vTaskDelay(pdMS_TO_TICKS(BLINK_PERIOD_MS));
    }
}
