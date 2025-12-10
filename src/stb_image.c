#include <stdint.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image.h"
#include "../include/stb_image_write.h"
#include "../include/image-processor.h"

enum {
    IMG_ERR = 1,
    IMG_OK = 0
};

int get_stb_channels(pixel_format_t format) {
    switch (format) {
	case PIXEL_FORMAT_GREYSCALE: return 1;
	case PIXEL_FORMAT_RGB: return 3;
	case PIXEL_FORMAT_RGBA: return 4;
	case PIXEL_FORMAT_CMYK: return 0; //not supported by stb
	default: return 0;
    }
}

int image_load_stb(const char *path, image_t *out_img, pixel_format_t format) {
    int width, height, channels;
    int req_channels = get_stb_channels(format);

    uint8_t *data = stbi_load(path,&width,&height,&channels,req_channels);
    if (!data) {
	return IMG_ERR;
    }

    out_img->width = width;
    out_img->height = height;
    out_img->format = format;
    out_img->data = data;
    out_img->data_size = width * height * get_bytes_per_pixel(format);

    return IMG_OK;
}

int image_save_stb(const char *filename, image_t *img) {
    int result = 0;
    int channels = get_bytes_per_pixel(img->format);

    if (strstr(filename, ".png")) {

    }
    if (strstr(filename, ".bmp")) {

    }
    if (strstr(filename, ".jpg")) {

    }
}
