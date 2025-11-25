#include <stddef.h>
#include <stdint.h>
#define _POSIX_C_SOURCE 200112L

#include "../include/image-processor.h"
#include "../include/bmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push,1)
typedef struct {
    uint16_t bfType; // "BM" signature (0x4D42)
    uint32_t bfSize; // Total file size in bytes
    uint16_t bfReserved1; // Reserved, must be 0
    uint16_t bfReserved2; // Reserved, must be 0
    uint32_t bfOffbits; // Offset to puxet data from start of file
} bmp_file_header_t;

typedef struct {
    uint32_t biSize; //size of the header (40)         
    int32_t  biWidth; // Image width in pixels
    int32_t  biHeight; // Image height in pixels (positive = bottom-up)
    uint16_t biPlanes; // number of colour planes (must be 1)
    uint16_t biBitCount; // bits per pixel (1,4,8,16,24,32)
    uint32_t biCompression; // Commpression type (0 = uncompressed)
    uint32_t biSizeImage; // Size of pixel data (can be 0 for uncompressed)
    int32_t  biXPelsPerMeter; // Horizontal pixels per meter
    int32_t  biYPelsPerMeter; // Vertical pixels per meter
    uint32_t biClrUsed; // Number of colours used (0 = all)
    uint32_t biClrImportant; // Important colours (0 = all)
} bmp_info_header_t;
#pragma pack(pop)

enum {
    BMP_OK = 0,
    BMP_ERR = 1
};

static size_t row_padded_bytes(int width, int bpp) {
    size_t rowbytes = (size_t)width * (bpp / 8);
    size_t pad = (4 - (rowbytes % 4)) & 3;
    return rowbytes + pad;
}

int bmp_load(const char *path, image_t *out_img) {
    if (!path || !out_img) return BMP_ERR;
    memset(out_img, 0, sizeof(*out_img));
    FILE *f = fopen(path,"rb");
    if(!f) return BMP_ERR;

    bmp_file_header_t fh;
    if(fread(&fh, sizeof(fh), 1, f) != 1) {
	fclose(f);
	return BMP_ERR;
    }

    if(fh.bfType != 0x4D42) {
	fclose(f);
	return BMP_ERR;
    }
    
    bmp_info_header_t ih;
    if (fread(&ih, sizeof(ih), 1, f) != 1) {
	fclose(f);
	return BMP_ERR;
    }
    
    if (ih.biCompression != 0) {
	fclose(f);
	return BMP_ERR;
    }
    if (ih.biBitCount != 24) {
	fclose(f);
	return BMP_ERR;
    }

    int width = ih.biWidth;
    int height = ih.biHeight;
    int top_down = 0;
    if (height < 0) {
	top_down = 1;
	height = -height;
    }
    if (width <= 0 || height <=0) {
	fclose(f);
	return BMP_ERR;
    }

    size_t rowbytes_padded = row_padded_bytes(width, ih.biBitCount);
    size_t image_size = rowbytes_padded * (size_t)height;

    if (fseek(f, (long)fh.bfOffbits, SEEK_SET) != 0) {
	fclose(f);
	return BMP_ERR;
    }

    uint8_t *rowbuf = malloc(rowbytes_padded);
    if (!rowbuf) { fclose(f); return BMP_ERR;}

    uint8_t *pixels = malloc((size_t)width * (size_t)height * 3);
    if (!pixels) { free(rowbuf); fclose(f); return BMP_ERR;}
    
    for (int r = 0; r < height ; ++r) {
	if (fread(rowbuf, 1, rowbytes_padded, f) != rowbytes_padded) {
	    free(rowbuf);
	    free(pixels);
	    fclose(f);
	    return BMP_ERR;
	}
	int bmp_row = top_down ? r : (height - 1 - r);
	uint8_t *dst = pixels + (size_t)bmp_row * (size_t)width * 3;
	uint8_t *src_row = rowbuf;
	for (int x = 0; x < width; ++x) {
	    uint8_t b = src_row[3 * x + 0];	
	    uint8_t g = src_row[3 * x + 1];
	    uint8_t rcol = src_row[3 * x + 2];

	    dst[3 * x + 0] = rcol;
            dst[3 * x + 1] = g;
            dst[3 * x + 2] = b;
	}
	(void)bmp_row;
    }
    free(rowbuf);
    fclose(f);
    
    out_img->width = width;
    out_img->height = height;
    out_img->format = PIXEL_FORMAT_RGB;
    out_img->data = pixels;
    out_img->data_size = image_size;

    return BMP_OK;
}

int bmp_write(const char *path, const image_t *img) {
    if (!path || !img || !img->data) return BMP_ERR;

    FILE *f = fopen(path, "wb");
    if (!f) return BMP_ERR;

    bmp_file_header_t fh;
    bmp_info_header_t ih;
    memset(&fh, 0, sizeof(fh));
    memset(&ih, 0, sizeof(ih));
    
    int width = img->width;
    int height = img->height;
    
    size_t rowbytes = (size_t)width * 3;
    size_t rowbytes_padded = row_padded_bytes(width, 24);
    size_t image_size = rowbytes_padded * (size_t)height;

    fh.bfType = 0x4D42;
    fh.bfOffbits = sizeof(bmp_file_header_t) + sizeof(bmp_info_header_t);
    fh.bfSize = (uint32_t)(fh.bfOffbits + image_size);

    ih.biSize = sizeof(bmp_info_header_t);
    ih.biWidth = width;
    ih.biHeight = height;
    ih.biPlanes = 1;
    ih.biBitCount = 24;
    ih.biCompression = 0;
    ih.biSizeImage = (uint32_t)image_size;
    ih.biXPelsPerMeter = 3780;
    ih.biYPelsPerMeter = 3780;

    if (fwrite(&fh, sizeof(fh), 1, f) != 1) { fclose(f); return BMP_ERR; }
    if (fwrite(&ih, sizeof(ih), 1, f) != 1) { fclose(f); return BMP_ERR; }

    uint8_t *pad = NULL;
    size_t padlen = rowbytes_padded - rowbytes;
    if(padlen){
	pad = calloc(1,padlen);
	if(!pad) {fclose(f); return BMP_ERR;}
    }

    for (int row = 0;  row < height ; ++row) {
	int src_row = height - 1 - row;
	const uint8_t *src = img->data + (size_t)src_row * (size_t)width * 3;
	for (int x = 0; x < width; ++x) {
            uint8_t r = src[3 * x + 0];
            uint8_t g = src[3 * x + 1];
            uint8_t b = src[3 * x + 2];
            uint8_t out[3] = { b, g, r };
            if (fwrite(out, 1, 3, f) != 3) {
                free(pad);
                fclose(f);
                return BMP_ERR;
	    }
	}
	if (padlen) {
	    if (fwrite(pad, 1, padlen, f) != padlen) {
		free(pad);
		fclose(f);
		return BMP_ERR;
	    }
	}
    }
    free(pad);
    fclose(f);
    return BMP_OK;
}

