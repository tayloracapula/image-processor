#ifndef  IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <stdint.h>
#include <stddef.h>
typedef enum {
    PIXEL_FORMAT_GREYSCALE, //1 byte per pixel
    PIXEL_FORMAT_RGB, //3 byte per pixel
    PIXEL_FORMAT_RGBA, //4 byte per pixel
    PIXEL_FORMAT_CMYK, //4 byte per pixel
} pixel_format_t;

typedef struct {
    int height;
    int width;
    pixel_format_t format;
    uint8_t *data;
    size_t data_size;
} image_t;

int get_bytes_per_pixel(pixel_format_t format);
image_t* create_image(int width, int height, pixel_format_t format);
void image_free(image_t *img);
void image_cleanup(image_t *img);
int img_invert(image_t *img);
int img_rotate(image_t *img, int direction);
int img_blur(image_t *img);

#endif // IMAGE-PROCESSOR_H
