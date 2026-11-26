#include "../include/led_screen.h"
#include "../include/led.h"
#include "../include/weather.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static char prevMode = -1;
static int i = 0;

int mode = 0;

enum {
	SUNNY = 0,
	CLOUDY = 1,
	DRIZZLE = 2,
	RAIN = 3,
	SHOWER = 4,
	THUNDER = 5,
	SNOW = 6
};

typedef struct {
    int x;
    int y;
} Coords;

const Coords sunnyCoords[8] = {
    {0, 2}, {0, 5},
    {2, 0}, {2, 7},
    {5, 0}, {5, 7},
    {7, 2}, {7, 5}
};

void sunny(rgb_t stripArr[SIZE][SIZE], int *i);
void cloudy(rgb_t stripArr[SIZE][SIZE], int *i);
//void drizzle(rgb_t stripArr[SIZE][SIZE], int *i);
void rain(rgb_t stripArr[SIZE][SIZE], int *i);
//void shower(rgb_t stripArr[SIZE][SIZE], int *i);
//void thunder(rgb_t stripArr[SIZE][SIZE], int *i);
void snow(rgb_t stripArr[SIZE][SIZE], int *i);

void changePixels(rgb_t stripArr[SIZE][SIZE], int mode, int *delay);
void updatePixels(rgb_t stripArr[SIZE][SIZE]);
void fillArr(rgb_t stripArr[SIZE][SIZE], const rgb_t *reference[SIZE][SIZE]);

void led_screen_task(void*)
{
	int delay = DELAY_MAX;
	ESP_LOGI("screen", "task started");
	changePixels(stripArr, SNOW, &delay);   // draw immediately, no network
	updatePixels(stripArr);
	ESP_LOGI("screen", "first frame drawn");
	while (true){
		delay = DELAY_MAX;
		if (get_will_it_rain(CITY) == 1)
			mode = RAIN;
		else if(get_will_it_snow(CITY) == 1)
			mode = SNOW;
		else if (get_cloud(CITY) == 1)
			mode = CLOUDY;
		else
			mode = SUNNY;
		changePixels(stripArr, mode, &delay);
		updatePixels(stripArr);
		vTaskDelay(pdMS_TO_TICKS(delay));
	}
}

void changePixels(rgb_t stripArr[SIZE][SIZE], int mode, int *delay)
{
	if (prevMode != mode){
		i = 0;
		prevMode = mode;
	}
	
	switch (mode){
		case SUNNY:
			if (i == 0) {fillArr(stripArr, sunnyStart);}
			sunny(stripArr, &i);
			break;
		case CLOUDY:
			if (i == 0) {fillArr(stripArr, cloudyStart);}
			cloudy(stripArr, &i);
			break;
		/*case DRIZZLE:
			if (i == 0) {fillArr(stripArr, drizzleStart);}
			drizzle(stripArr, &i);
			break;*/
		case RAIN:
			if (i == 0) {fillArr(stripArr, rainStart);}
			rain(stripArr, &i);
			break;
		/*case SHOWER:
			if (i == 0) {fillArr(stripArr, showerStart);}
			shower(stripArr, &i);
			break;*/
		/*case THUNDER:
			if (i == 0) {fillArr(stripArr, showerStart);}
			thunder(stripArr, &i);
			break;*/
		case SNOW:
			if (i == 0) {fillArr(stripArr, snowyStart);}
			snow(stripArr, &i);
			break;
	}
}


void fillArr(rgb_t stripArr[SIZE][SIZE], const rgb_t *reference[SIZE][SIZE])
{
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			stripArr[i][j] = *reference[i][j];
}


void updatePixels(rgb_t stripArr[SIZE][SIZE])
{
	for (int i = 0; i < 64; i++){
		int y = i / 8;
		int reverseState = y % 2;
		int x;
		if (reverseState)
			x = 7 - (i % 8);
		else
			x = i % 8;
		ESP_ERROR_CHECK(led_strip_set_pixel(s_strip2, i, stripArr[y][x].r, stripArr[y][x].g, stripArr[y][x].b));
	}
	ESP_ERROR_CHECK(led_strip_refresh(s_strip2));
}

void sunny(rgb_t stripArr[SIZE][SIZE], int *i)
{
	switch (*i){
		case 0:
			for (int i = 0; i < 8; i++)
				stripArr[sunnyCoords[i].x][sunnyCoords[i].y] = wSunny.null;
			break;
		case 1:
			for (int i = 0; i < 8; i++)
				stripArr[sunnyCoords[i].x][sunnyCoords[i].y] = wSunny.n3;
			break;
		case 2:
			stripArr[3][4] = wSunny.n2;
			break;
		case 3:
			stripArr[3][4] = wSunny.n1;
			break;
	}
	if (++*i == 4) 
		*i = 0;
}
void cloudy(rgb_t stripArr[SIZE][SIZE], int *i){}
void rain(rgb_t stripArr[SIZE][SIZE], int *i){}
void snow(rgb_t stripArr[SIZE][SIZE], int *i){}
