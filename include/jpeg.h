#ifndef JPEG_H
#define JPEG_H

#include "image-processor.h"

#include <stdint.h>
#include <stdio.h>
int jpeg_load(const char *path, image_t *out_img);

int jpeg_write(const char *path, const image_t *img);


#endif
