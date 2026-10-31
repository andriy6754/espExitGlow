#include "../include/led.h"
#include "esp_log.h"

#define LED_STRIP_GPIO_PIN 4

#define TAG "led_strip"
static led_strip_handle_t s_strip = NULL;

void configure_led_strip(void) {
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

void led_set_color(unsigned int from, unsigned int to, unsigned int r, unsigned int g, unsigned int b) {
    if (to > LED_STRIP_NUM_LEDS)
        to = LED_STRIP_NUM_LEDS;

    for (unsigned int i = from; i < to; i++) {
        ESP_ERROR_CHECK(led_strip_set_pixel(s_strip, i, r, g, b));
    }
    ESP_ERROR_CHECK(led_strip_refresh(s_strip));
}
