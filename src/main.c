#include <stdio.h>
#include <stdlib.h>
#include "../include/image-processor.h"
#include "../include/bmp.h"

int main(int argc, char **argv) {
    if (argc != 3) {
	fprintf(stderr, "Usage: %s input.bmp output.bmp\n", argv[0]);
	return 1;
    }
    const char *in = argv[1];
    const char *out = argv[2];

    image_t img;
    if(bmp_load(in, &img) != 0) {
	fprintf(stderr, "Failed to load BMP: %s\n",in);
	return 2;
    }
    img_invert(&img);

    if(bmp_write(out,&img) != 0) {
	fprintf(stderr, "Failed to write BMP: %s\n",out);
	image_free(&img);
	return 4;
    }
    printf("Copied %dx%d BMP to %s\n", img.width, img.height, out);
    image_cleanup(&img);
    return 0;
}
