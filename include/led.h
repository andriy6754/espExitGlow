#ifndef ESP_EXIT_GLOW_LED_H
#define ESP_EXIT_GLOW_LED_H

#include "led_strip.h"

#define LED_STRIP_NUM_LEDS 180

void configure_led_strip(void);

// Handle of the strip created by configure_led_strip()
led_strip_handle_t get_led_strip(void);

#endif // ESP_EXIT_GLOW_LED_H
