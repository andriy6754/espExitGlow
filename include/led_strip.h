#ifndef LEDSTRIP
#define LEDSTRIP

#define SIZE 8

typedef struct{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} RGB;

typedef struct{
	struct RGB n1;
	struct RGB n2;
	struct RGB n3;
	struct RGB n4;
	struct RGB n5;
	struct RGB n6;
	struct RGB n7;
	struct RGB n8;
	struct RGB null;
} Colors;

extern RGB stripArr[SIZE][SIZE];

extern const Colors wSunny;
extern const Colors wRainy;
extern const Colors wSnowyNDrizzle;

extern const RGB rainStart[SIZE][SIZE];
extern const RGB showerStart[SIZE][SIZE];
extern const RGB drizzleStart[SIZE][SIZE];
extern const RGB cloudyStart[SIZE][SIZE];
extern const RGB sunnyStart[SIZE][SIZE];
extern const RGB snowyStart[SIZE][SIZE];

void changePixels(RGB stripArr[SIZE][SIZE], const Colors *palette, int *delay);
void updatePixels(RGB stripArr[SIZE][SIZE]);

#endif
