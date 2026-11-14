#include "../include/led_strip.h"

static char prevMode = -1;
static int i = 0;

enum {
	SUNNY = 0,
	CLOUDY = 1,
	DRIZZLE = 2,
	RAIN = 3,
	SHOWER = 4,
	THUNDER = 5,
	SNOW = 6
};

void fillArr(RGB stripArr[SIZE][SIZE], RGB reference[SIZE][SIZE]);

void sunny(RGB stripArr[SIZE][SIZE], RGB reference[SIZE][SIZE], int i);
void cloudy(RGB stripArr[SIZE][SIZE], RGB reference[SIZE][SIZE], int i);
void drizzle(RGB stripArr[SIZE][SIZE], RGB reference[SIZE][SIZE], int i);
void rain(RGB stripArr[SIZE][SIZE], RGB reference[SIZE][SIZE], int i);
void shower(RGB stripArr[SIZE][SIZE], RGB reference[SIZE][SIZE], int i);
void thunder(RGB stripArr[SIZE][SIZE], RGB reference[SIZE][SIZE], int i);
void snow(RGB stripArr[SIZE][SIZE], RGB reference[SIZE][SIZE], int i);

void changePixels(RGB stripArr[SIZE][SIZE], int mode, int *delay)
{
	if (prevMode != mode && prevMode != -1){
		i = 0;
		prevMode = mode;
	}
	switch (mode){
		case SUNNY:
			sunny(stripArr, sunnyStart, i);
			break;
		case CLOUDY:
			cloudy(stripArr, cloudyStart, i);
			break;
		case DRIZZLE:
			drizzle(stripArr, drizzleStart, i);
			break;
		case RAIN:
			rain(stripArr, rainStart);
			break;
		case SHOWER:
			shower(stripArr, showerStart, i);
			break;
		case THUNDER:
			thunder(stripArr, showerStart, i);
			break;
		case SNOW:
			snow(stripArr, snowyStart, i);
			break;
	}
}


void fillArr(RGB stripArr[SIZE][SIZE], RGB reference[SIZE][SIZE])
{
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			stripArr[i][j] = reference[i][j];
}
