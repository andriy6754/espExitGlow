#include "../include/led_screen.h"

const Colors wSunny = {
    {127, 115, 16}, // sun main color
    {127, 127, 127}, // white
    {127, 115, 33}, // sun traces
    {127, 125, 16}, // sun secondary color
    {116, 118, 121}, // cloud main color
    {105, 110, 115}, // cloud secondary color
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};

const Colors wRainy = {
    {5, 8, 14},      // cloud darkest color
    {16, 23, 34},    // cloud dark color
    {35, 49, 69},    // cloud main color
    {65, 91, 122},   // cloud light color
    {100, 135, 180}, // cloud lightest color
    {0, 91, 119},    // droplets color
    {127, 121, 0},   // lightning main color
    {127, 124, 50},  // lightning additional color
    {0, 0, 0}
};

const Colors wSnowyNDrizzly = {
    {5, 8, 14},      // cloud darkest color
    {16, 23, 34},    // cloud dark color
    {35, 49, 69},    // cloud main color
    {65, 91, 122},   // cloud light color
    {100, 135, 180}, // cloud lightest color
    {0, 91, 119},    // droplets color
    {127, 127, 127}, // white color
    {100, 100, 100}, // gray color
    {0, 0, 0}
};
