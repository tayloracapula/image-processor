#ifndef BMP_H
#define BMP_H

#include <stdint.h>
#include <stddef.h>
#include "image-processor.h"

int bmp_load(const char *path, image_t *out_img);

int bmp_write(const char *path, const image_t *img);

#endif
