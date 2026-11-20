#ifndef LEDSTRIP
#define LEDSTRIP

#define SIZE 8

typedef struct{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} rgb_t;

enum{
	DELAY_MAX = 400;
	DELAY_MIN = 200;
}

typedef struct{
	rgb_t n1;
	rgb_t n2;
	rgb_t n3;
	rgb_t n4;
	rgb_t n5;
	rgb_t n6;
	rgb_t n7;
	rgb_t n8;
	rgb_t null;
} Colors;

extern rgb_t stripArr[SIZE][SIZE];

extern const Colors wSunny;
extern const Colors wRainy;
extern const Colors wSnowyNDrizzle;

extern const rgb_t rainStart[SIZE][SIZE];
extern const rgb_t showerStart[SIZE][SIZE];
extern const rgb_t drizzleStart[SIZE][SIZE];
extern const rgb_t cloudyStart[SIZE][SIZE];
extern const rgb_t sunnyStart[SIZE][SIZE];
extern const rgb_t snowyStart[SIZE][SIZE];

void changePixels(rgb_t stripArr[SIZE][SIZE], const Colors *palette, int *delay);
void updatePixels(rgb_t stripArr[SIZE][SIZE]);
#endif
