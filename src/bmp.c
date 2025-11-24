#define _POSIX_C_SOURCE 200112L

#include "../include/bmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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
