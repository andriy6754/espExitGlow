#include "../include/led_strip.h"

static char prevMode = -1;
static int i = 0;

void changePixels(RGB stripArr[8][8], int mode, int *delay)
{
	if (prevMode != mode && prevMode != -1)
		i = 0;

	for (char i = 0; i < SIZE; i++){
		for (char j = 0; j < SIZE; j++){
			
		}
	}
}
