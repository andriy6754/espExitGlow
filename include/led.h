#ifndef ESP_EXIT_GLOW_LED_H
#define ESP_EXIT_GLOW_LED_H

#include "led_strip.h"

#define LED_STRIP1_GPIO_PIN 4
#define LED_STRIP2_GPIO_PIN 5

#define LED_STRIP_NUM_LEDS 12

static led_strip_handle_t s_strip1 = NULL;
static led_strip_handle_t s_strip2 = NULL;

void configure_led_strip(void);

// Set the color of a range of LEDs
void led_set_color(unsigned int from, unsigned int to, unsigned int r, unsigned int g, unsigned int b);

#endif // ESP_EXIT_GLOW_LED_H
