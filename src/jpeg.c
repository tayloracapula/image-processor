#include "../include/image-processor.h"
#include "../include/jpeg-parse.h"
#include <stdint.h>
#include <stdio.h>

enum {
    JPEG_ERR = 1,
    JPEG_OK = 0
};

int jpeg_load(const char *path, image_t *out_img) {
    FILE *file = fopen(path,"rb");
    if (!file) return JPEG_ERR;

    jpeg_info info = {0};
    scan_info scan = {0};

    while (1) {
	uint16_t marker = read_big_endian_16(file);

	switch (marker) {
	    case SOI:
		break;

	    case APP0:{
		uint16_t length = read_big_endian_16(file);
		fseek(file, length - 2, SEEK_CUR);
		break;
	    }

	    case DQT:
		if (parse_quantization_table(file, &info) != JPEG_OK) {
		    fclose(file);
		    return JPEG_ERR;
		}
		break;
	    
	    case SOF:
		if (parse_frame_header(file, &info) != JPEG_OK) {
		    fclose(file);
		    return JPEG_ERR;
		} 
		break;

	    case DHT:
		if (parse_huffman_table(file, &info) != JPEG_OK) {
		    fclose(file);
		    return JPEG_ERR;
		}
		break;

	    case SOS:
		if (parse_scan_header(file, &scan) != JPEG_OK) {
		    fclose(file);
		    return JPEG_ERR;
		}
		break;

	    case EOI:
		fclose(file);
		return JPEG_OK;

	    default:
		fclose(file);
		return JPEG_ERR;

	}
    }

    return JPEG_OK;
}

int jpeg_write(const char *path, const image_t *img) {

    return JPEG_OK;
}
