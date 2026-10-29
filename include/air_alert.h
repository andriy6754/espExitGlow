#ifndef ESP_EXIT_GLOW_AIR_ALERT_H
#define ESP_EXIT_GLOW_AIR_ALERT_H

int air_alert_check(const char *region);

// FreeRTOS task: colors the LED strip according to the air alert status
void alert_strip_task(void *arg);
#endif
