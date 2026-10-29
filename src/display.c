#include "../include/display.h"
#include "../include/clock.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tm1637.h"

#define TIME_UPDATE_PERIOD_MS 1000

static void display_time(tm1637_handle_t display, int hour, int minute) {
    // Format: HH:MM (use colon segment if available)
    uint8_t time_display[4];
    time_display[0] = tm1637_encode_digit(hour / 10);
    time_display[1] = tm1637_encode_digit(hour % 10) | TM1637_SEG_DP;
    time_display[2] = tm1637_encode_digit(minute / 10);
    time_display[3] = tm1637_encode_digit(minute % 10);

    tm1637_set_segments(display, time_display, 4, 0);
}

void tm1637_show_time_task(void *arg) {
    (void)arg;

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
