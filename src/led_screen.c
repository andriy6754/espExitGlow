#include "../include/led_screen.h"
#include "../include/led.h"
#include "../include/weather.h"
#include "../include/animation_instructions.h"


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static char prevMode = -1;
static int i = 0;
static char flag = 0;

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
	flag = 0;
	if (prevMode != mode){
		i = 0;
		prevMode = mode;
		flag = 1;
	}
	
	switch (mode){
		case SUNNY:
			if (flag) {fillArr(stripArr, sunnyStart); *delay = DELAY_MAX;}
			sunny(stripArr, &i);
			break;
		case CLOUDY:
			if (flag) {fillArr(stripArr, cloudyStart); *delay = DELAY_MAX;}
			cloudy(stripArr, &i);
			break;
		/*case DRIZZLE:
			if (flag) {fillArr(stripArr, drizzleStart); *delay = DELAY_MIN;}
			drizzle(stripArr, &i);
			break;*/
		case RAIN:
			if (flag) {fillArr(stripArr, rainStart); *delay = DELAY_MIN;}
			rain(stripArr, &i);
			break;
		/*case SHOWER:
			if (flag) {fillArr(stripArr, showerStart); *delay = DELAY_MIN;}
			shower(stripArr, &i);
			break;*/
		/*case THUNDER:
			if (flag) {fillArr(stripArr, showerStart); *delay = DELAY_MIN;}
			thunder(stripArr, &i);
			break;*/
		case SNOW:
			if (flag) {fillArr(stripArr, snowyStart); *delay = DELAY_MAX;}
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

void cloudy(rgb_t stripArr[SIZE][SIZE], int *i)
{
    switch (*i){
        case 0:
            for (int i = 0; i < 6; i++){
                stripArr[cloudyCoords1LG[i].y][cloudyCoords1LG[i].x] = wSunny.n6;
                if (i < 3)
                    stripArr[cloudyCoords1Y[i].y][cloudyCoords1Y[i].x] = wSunny.n5;
            }
            stripArr[6][7] = wSunny.n6;
            stripArr[6][4] = wSunny.n5;
            stripArr[6][0] = wSunny.null;
            break;

        case 1:
            for (int i = 0; i < 6; i++){
                stripArr[cloudyCoords1LG[i].y][cloudyCoords1LG[i].x] = wSunny.n5;
                if (i < 3)
                    stripArr[cloudyCoords1Y[i].y][cloudyCoords1Y[i].x] = wSunny.n4;
            }
            stripArr[6][7] = wSunny.null;
            stripArr[6][4] = wSunny.n6;
            stripArr[6][0] = wSunny.n6;
            break;

        case 2:
            for (int i = 0; i < 3; i++){
                stripArr[cloudyCoords2LG[i].y][cloudyCoords2LG[i].x] = wSunny.n5;
                if (i < 2)
                    stripArr[cloudyCoords2Y[i].y][cloudyCoords2Y[i].x] = wSunny.n4;
            }
            stripArr[6][3] = wSunny.n6;
            break;

        case 3:
            for (int i = 0; i < 3; i++){
                stripArr[cloudyCoords2LG[i].y][cloudyCoords2LG[i].x] = wSunny.n6;
                if (i < 2)
                    stripArr[cloudyCoords2Y[i].y][cloudyCoords2Y[i].x] = wSunny.n5;
            }
            stripArr[6][3] = wSunny.n5;
            break;
    }
    if (++*i == 4)
        *i = 0;
}

void rain(rgb_t stripArr[SIZE][SIZE], int *i)
{
	switch (*i){
		case 0:
			stripArr[7][3] = wRainy.null;
			stripArr[7][6] = wRainy.null;
			break;
		
		case 1:
			stripArr[5][2] = wRainy.n6;
			stripArr[5][5] = wRainy.n6;
			break;
		
		case 2:
			stripArr[6][3] = wRainy.n6;
			stripArr[6][6] = wRainy.n6;
			break;
		
		case 3: 
			stripArr[5][2] = wRainy.null;
			stripArr[5][5] = wRainy.null;
			break;
		
		case 4:
			stripArr[6][3] = wRainy.null;
			stripArr[6][6] = wRainy.null;
			stripArr[7][3] = wRainy.n6;
			stripArr[7][6] = wRainy.n6;
			break;
	}
	if (++*i == 5)
		*i = 0;
}
void snow(rgb_t stripArr[SIZE][SIZE], int *i)
{
	switch (*i){
		case 0:
			stripArr[5][2] = wSnowyNDrizzly.n7;
			stripArr[5][3] = wSnowyNDrizzly.n8;
			stripArr[6][2] = wSnowyNDrizzly.n8;
			break;

		case 1:
			stripArr[5][2] = wSnowyNDrizzly.null;
			stripArr[6][3] = wSnowyNDrizzly.n7;
			stripArr[5][5] = wSnowyNDrizzly.n8;
			stripArr[5][6] = wSnowyNDrizzly.n7;
			stripArr[6][6] = wSnowyNDrizzly.n8;
			break;

		case 2:
			stripArr[5][3] = wSnowyNDrizzly.null;
			stripArr[5][6] = wSnowyNDrizzly.null;
			stripArr[6][5] = wSnowyNDrizzly.n7;
			stripArr[7][3] = wSnowyNDrizzly.n8;
			break;

		case 3:
			stripArr[5][5] = wSnowyNDrizzly.null;
			stripArr[6][3] = wSnowyNDrizzly.null;
			stripArr[7][2] = wSnowyNDrizzly.n7;
			stripArr[7][5] = wSnowyNDrizzly.n8;
			break;

		case 4:
			stripArr[6][2] = wSnowyNDrizzly.null;
			stripArr[6][5] = wSnowyNDrizzly.null;
			stripArr[7][6] = wSnowyNDrizzly.n7;
			break;

		case 5:
			stripArr[6][6] = wSnowyNDrizzly.null;
			stripArr[7][2] = wSnowyNDrizzly.null;
			stripArr[7][3] = wSnowyNDrizzly.null;
			break;

		case 6:
			stripArr[7][5] = wSnowyNDrizzly.null;
			stripArr[7][6] = wSnowyNDrizzly.null;
	}
	if (++*i == 7)
		*i = 0;
}
