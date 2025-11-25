#include <stdio.h>
#include <stdlib.h>
#include "../include/bmp.h"

int main(int argc, char **argv) {
    if (argc != 3) {
	fprintf(stderr, "Usage: %s input.bmp output.bmp\n", argv[0]);
	return 1;
    }
    const char *in = argv[1];
    const char *out = argv[2];

    bmp_image_t img;
    if(bmp_load(in, &img) != 0) {
	fprintf(stderr, "Failed to load BMP: %s\n",in);
	return 2;
    }
    if(bmp_invert(&img) != 0) {
	fprintf(stderr, "Failed to edit BMP: %s\n",in);
	return 3;
    }
    if(bmp_write(out,&img) != 0) {
	fprintf(stderr, "Failed to write BMP: %s\n",out);
	bmp_free(&img);
	return 4;
    }
    printf("Copied %dx%d BMP to %s\n", img.width, img.height, out);
    bmp_free(&img);
    return 0;
}
