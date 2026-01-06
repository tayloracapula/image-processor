#ifndef STB_INTERFACE
#define STB_INTERFACE

#include "image-processor.h"

int image_load_stb(const char *path, image_t *out_img);
int image_save_stb(const char *filename, image_t *img);

int get_stb_channels(pixel_format_t format);
#endif
