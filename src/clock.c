#include <time.h>
int hour = 0;
int minute = 0;

void get_current_utc2_time(void) {
    time_t time_epoch = time(NULL) + 2 * 60 * 60;
    struct tm *time_utc = gmtime(&time_epoch);
    hour = time_utc->tm_hour;
    minute = time_utc->tm_min;
}

int get_current_hour(void) {
    get_current_utc2_time();
    return hour;
}

int get_current_minute(void) {
    get_current_utc2_time();
    return minute;
}