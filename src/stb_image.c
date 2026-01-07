#include <stdint.h>
#include <stdio.h>
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

int image_load_stb(const char *path, image_t *out_img) {
    int width, height, channels;

    uint8_t *data = stbi_load(path,&width,&height,&channels,0);
    if (!data) {
	return IMG_ERR;
    }

    pixel_format_t format;
    switch (channels) {
	case 1: format = PIXEL_FORMAT_GREYSCALE; break;
	case 3: format = PIXEL_FORMAT_RGB; break;
	case 4: format = PIXEL_FORMAT_RGBA; break;
	default:
	    stbi_image_free(data);
	return IMG_ERR;
    }

    out_img->width = width;
    out_img->height = height;
    out_img->format = format;
    out_img->data = data;
    out_img->data_size = width * height * get_bytes_per_pixel(format);
    printf("Image Loaded\n");
    return IMG_OK;
}

int image_save_stb(const char *filename, image_t *img) {
    int result = 0;
    int channels = get_bytes_per_pixel(img->format);

    printf("Saving Image");

    if (strstr(filename, ".png")) {
	int comp = get_stb_channels(img->format);
	return stbi_write_png(filename, img->width, img->height, comp, img->data, (img->width * comp)) ? IMG_OK : IMG_ERR;
    }
    if (strstr(filename, ".bmp")) {
	int comp = get_stb_channels(img->format);
	return stbi_write_bmp(filename, img->width, img->height, comp, img->data) ? IMG_OK : IMG_ERR;
    }
    if (strstr(filename, ".jpg")) {
	int comp = get_stb_channels(img->format);
	int quality = 95;
	return stbi_write_jpg(filename, img->width, img->height, comp, img->data, quality) ? IMG_OK : IMG_ERR;
    }
    return IMG_ERR;
}
