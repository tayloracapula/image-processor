#include "../include/image-processor.h"
#include <stdlib.h>

int get_bytes_per_pixel(pixel_format_t format) {
    switch (format) {
    	case PIXEL_FORMAT_GREYSCALE: return 1;
	case PIXEL_FORMAT_RGB: return 3;
	case PIXEL_FORMAT_RGBA: return 4;
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
    
    int bytes = get_bytes_per_pixel(format);

    size_t data_size = (size_t)width * height * bytes;
    
    img->data = calloc(data_size, 1);
    
    img->height = height;
    img->width = width;
    img->format = format;
    img->data_size = data_size;
    return img;
}

int img_invert(image_t *img) {
    if (!img || !img->data) return 1;
    
    uint8_t *end = img->data+ (size_t)img->width * (size_t)img->height * 3;
    for (uint8_t *p = img->data; p < end ; ++p){
	*p = 255 - *p;
    }
    return 0;
}
int img_rotate(image_t *img, int direction) {
    uint8_t *new_pixels = malloc((size_t)img->height * (size_t)img->width * 3);

    for (int y = 0; y < img->height; ++y){
	for (int x = 0; x < img->width; ++x) {
	    uint8_t *src = img->data + (y * img->width + x) * 3;
	    uint8_t *dst = new_pixels + (x * img->height + (img->height - 1 - y )) * 3;

	    dst[0] = src[0];
	    dst[1] = src[1];
	    dst[2] = src[2];
	}
    }
    free(img->data);
    img->data = new_pixels;
    int temp = img->width;
    img->width = img->height;
    img->height = temp;
    return 0;
}
