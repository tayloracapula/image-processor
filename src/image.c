#include "../include/image-processor.h"
#include <stdlib.h>

int get_bytes_per_pixel(pixel_format_t format) {
    switch (format) {
    	case PIXEL_FORMAT_GREYSCALE: return 1;
	case PIXEL_FORMAT_RGB: return 3;
	case PIXEL_FORMAT_RGBA: return 4;
	case PIXEL_FORMAT_CMYK: return 4;
	default: return 0;
    }
}

void image_free(image_t *img){
    if(!img) return;
    free(img->data);
    free(img);
}

void image_cleanup(image_t *img){
    if(!img) return;
    free(img->data);
    img->data = NULL;
    img->data_size = 0;
}

image_t* create_image(int width, int height, pixel_format_t format) {
    image_t *img = malloc(sizeof(image_t));
    if (!img) return NULL;

    int bytes = get_bytes_per_pixel(format);

    size_t data_size = (size_t)width * height * bytes;
    
    img->data = calloc(data_size, 1);
    if (!img->data) {
	free(img);
	return NULL;
    }
    img->height = height;
    img->width = width;
    img->format = format;
    img->data_size = data_size;
    return img;
}

int img_invert(image_t *img) {
    if (!img || !img->data) return 1;
    
    int bpp = get_bytes_per_pixel(img->format);

    uint8_t *end = img->data+ (size_t)img->width * (size_t)img->height * bpp;
    for (uint8_t *p = img->data; p < end ; ++p){
	*p = 255 - *p;
    }
    return 0;
}

int img_rotate(image_t *img, int direction) {
    int bpp = get_bytes_per_pixel(img->format);
    uint8_t *new_pixels = malloc((size_t)img->height * (size_t)img->width * bpp);
    for (int y = 0; y < img->height; ++y){
	for (int x = 0; x < img->width; ++x) {
	    uint8_t *src = img->data + (y * img->width + x) * bpp;

	    int new_y, new_x;

	    if (direction > 0) { //clockwise
		new_x = img->height - 1 - y;
		new_y = x;
	    } else { //anti clockwise
		new_x = y;
		new_y = img->width - 1 - x;
	    }

	    uint8_t *dst = new_pixels + (new_y * img->height + new_x) * bpp;

	    for (int c = 0; c < bpp; c++){
		dst[c] = src[c];
	    }
	}
    }
    free(img->data);
    img->data = new_pixels;
    int temp = img->width;
    img->width = img->height;
    img->height = temp;
    return 0;
}
