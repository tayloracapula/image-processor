#ifndef BMP_H
#define BMP_H

#include <stdint.h>
#include <stddef.h>
#include "image-processor.h"

typedef struct {
    int width;
    int height;
    uint16_t bits_per_pixel;
    uint8_t *pixels;
} bmp_image_t;

int bmp_load(const char *path, image_t *out_img);

int bmp_write(const char *path, const image_t *img);

void bmp_free(bmp_image_t *img);


#endif
