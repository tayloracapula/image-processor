#include <stdio.h>
#include <string.h>
#include "../include/image-processor.h"
#include "../include/stb_interface.h"

typedef enum {
    OP_INVERT,
    OP_ROTATE_R,
    OP_ROTATE_L,
    OP_BLUR,
    OP_UNKNOWN
} operation_t;

operation_t parse_operation(const char *op) {
    if (strcmp(op, "invert") == 0) return OP_INVERT;
    if (strcmp(op, "rotate_r") == 0) return OP_ROTATE_R;
    if (strcmp(op, "rotate_l") == 0) return OP_ROTATE_L;
    if (strcmp(op, "blur") == 0) return OP_BLUR;
    return OP_UNKNOWN;
}

int main(int argc, char **argv) {
    if (argc != 4) {
	fprintf(stderr, "Usage: %s (input image) (output image) (operation)\n", argv[0]);
	return 1;
    }
    const char *in = argv[1];
    const char *out = argv[2];
    const char *operation = argv[3];

    image_t img;
    if(image_load_stb(in, &img) != 0) {
	fprintf(stderr, "Failed to load IMG: %s\n",in);
	return 2;
    }

    switch (parse_operation(operation)) {
	case OP_INVERT: 
	    if(img_invert(&img) != 0) {
		fprintf(stderr,"Failed to invert image\n");
		return 3;
	    }
	    break;

	case OP_ROTATE_R:
	    if (img_rotate(&img, 1) != 0) {
		fprintf(stderr,"Failed to rotate image\n");
		return 4;
	    }
	    break;

	case OP_ROTATE_L:
	    if (img_rotate(&img, 0) != 0) {
		fprintf(stderr, "Failed to rotate image\n");
		return 4;
	    }
	    break;
	
	case OP_BLUR:
	    if (img_blur(&img) != 0) {
		fprintf(stderr,"Failed to blur image\n");
		return 5;
	    }
	    break;
	case OP_UNKNOWN:
	    fprintf(stderr, "Unknown operation: %s\n", argv[1]);
	    return 6;
    }

    if(image_save_stb(out,&img) != 0) {
	fprintf(stderr, "Failed to write IMG: %s\n",out);
	image_free(&img);
	return 7;
    }
    printf("Copied %dx%d IMG to %s\n", img.width, img.height, out);
    image_cleanup(&img);
    return 0;
}
