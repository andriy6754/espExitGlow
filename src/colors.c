#include "../include/led_strip.h"

const struct Colors wSunny = {
	{255, 249, 66}, // sun main color
	{255, 255, 255}, // white
	{255, 233, 189}, // sun traces (duno how it called)
	{255, 251, 145}, // sun secondary color
	{233, 237, 242}, // clouds main color
	{211, 220, 230} // color secondary color
};

const struct Colors wRainy = {
	{52, 63, 77}, // cloud darkest color
	{71, 87, 107}, // cloud dark color
	{84, 109, 142}, // cloud main color
	{95, 126, 163}, // cloud light color
	{122, 166, 214}, // cloud lightest color
	{0, 183, 239}, // droplets color
	{255, 242, 0}, // lightning main color
	{255, 249, 100} // lighting additional color
};

const struct Colors wSnowyNDrizzle = {
	{52, 63, 77}, // cloud darkest color
	{71, 87, 107}, // cloud dark color
	{84, 109, 142}, // cloud main color
	{95, 126, 163}, // cloud light color
	{122, 166, 214}, // cloud lightest color
	{0, 183, 239}, // droplets color
	{255, 255, 255}, // white color
	{200, 200, 200}, // gray color
};
