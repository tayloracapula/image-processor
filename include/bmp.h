#ifndef BMP_H
#define BMP_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    int width;
    int height;
    uint16_t bits_per_pixel;
    uint8_t *pixels;
} bmp_image_t;

int bmp_load(const char *path, bmp_image_t *out_img);

int bmp_write(const char *path, const bmp_image_t *img);

void bmp_free(bmp_image_t *img);

int bmp_invert(bmp_image_t *img);

#endif
