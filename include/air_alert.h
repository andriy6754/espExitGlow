#ifndef ESP_EXIT_GLOW_AIR_ALERT_H
#define ESP_EXIT_GLOW_AIR_ALERT_H

typedef enum {
    AIR_ALERT_ERROR = -1,
    AIR_ALERT_NONE = 0,
    AIR_ALERT_YELLOW = 1,
    AIR_ALERT_RED = 2,
} air_alert_level_t;

// Check alert status for a Tryvoha.online region slug (e.g. "kyiv", "kyivska").
air_alert_level_t air_alert_check(const char *region);

// FreeRTOS task: colors the LED strip according to the air alert status
void alert_strip_task(void *arg);
#endif
