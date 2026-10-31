#ifndef ESP_EXIT_GLOW_LED_H
#define ESP_EXIT_GLOW_LED_H

#include "led_strip.h"

#define LED_STRIP_NUM_LEDS 180

void configure_led_strip(void);

// Set the color of a range of LEDs
void led_set_color(unsigned int from, unsigned int to, unsigned int r, unsigned int g, unsigned int b);

#endif // ESP_EXIT_GLOW_LED_H
