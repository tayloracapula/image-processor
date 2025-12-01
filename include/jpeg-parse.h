#ifndef JPEG_PARSE_H
#define JPEG_PARSE_H

#include <stdint.h>
#include <stdio.h>
#include "jpeg.h"
uint16_t read_big_endian_16(FILE *file);
int parse_frame_header(FILE *file, jpeg_info *info);
int parse_quantization_table(FILE *file, jpeg_info *info);
int parse_huffman_table(FILE *file, jpeg_info *info); 
int parse_scan_header(FILE *file, scan_info *scan);
#endif
