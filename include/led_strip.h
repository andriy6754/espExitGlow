#ifndef LEDSTRIP
#define LEDSTRIP

struct RGB {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

struct Colors {
	struct RGB n1;
	struct RGB n2;
	struct RGB n3;
	struct RGB n4;
	struct RGB n5;
	struct RGB n6;
	struct RGB n7;
	struct RGB n8;
	struct RGB null;
};

extern struct RGB stripArr[8][8];

extern const struct Colors wSunny;
extern const struct Colors wRainy;
extern const struct Colors wSnowy;

void changePixels(struct RGB stripArr[][8], const struct Colors *palette, int *delay);
void updatePixels(struct RGB stripArr[][8]);

#endif
