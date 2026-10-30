#ifndef ESP_EXIT_GLOW_WEATHER_H
#define ESP_EXIT_GLOW_WEATHER_H

int get_temp(const char *city);
int get_will_it_rain(const char *city);
int get_will_it_snow(const char *city);
int get_cloud(const char *city);

#endif