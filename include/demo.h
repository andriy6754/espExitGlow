#ifndef ESP_EXIT_GLOW_DEMO_H
#define ESP_EXIT_GLOW_DEMO_H

#include "esp_random.h"
#include <stdint.h>

// Set to 1 to cycle API values randomly every DEMO_INTERVAL_US; 0 for live APIs.
#ifndef DEMO_MODE
#define DEMO_MODE 1
#endif
#define DEMO_INTERVAL_US (15 * 1000 * 1000)

/**
 * Pick a uniform random index in [0, count) that differs from prev_idx
 * when count > 1 and prev_idx is valid.
 */
static inline int demo_pick_other_index(int prev_idx, int count) {
    if (count <= 1) {
        return 0;
    }
    if (prev_idx < 0 || prev_idx >= count) {
        return (int)(esp_random() % (uint32_t)count);
    }
    int pick = (int)(esp_random() % (uint32_t)(count - 1));
    return (pick >= prev_idx) ? (pick + 1) : pick;
}

#endif // ESP_EXIT_GLOW_DEMO_H
