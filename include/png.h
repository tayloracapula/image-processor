#ifndef PNG_H
#define PNG_H

#include "image-processor.h"

int png_load(const char *path, image_t *out_img);

int png_write(const char *path, const image_t *img);

#endif
