#include "../include/jpeg.h"
#include "../include/huff.h"
#include <stdint.h>
#include <stdio.h>

enum {
    JPEG_ERR = 1,
    JPEG_OK = 0
};



uint16_t read_big_endian_16(FILE *file){
    uint8_t bytes[2];
    if (fread(bytes, 1, 2, file) !=2) return 0;
    return (uint16_t)(bytes[0] << 8) | bytes[1];
}

int parse_frame_header(FILE *file, jpeg_info *info) {
    uint16_t length =  read_big_endian_16(file);
    info->precision = fgetc(file);
    info->height = read_big_endian_16(file);
    info->width = read_big_endian_16(file);

    uint8_t component_count = fgetc(file);

    for (int i = 0; i < component_count ; i++) {
	uint8_t id = fgetc(file);
	uint8_t sampling = fgetc(file);
	uint8_t quant_table = fgetc(file);


	info->componentInfo[id].HSamplingFactor = (sampling >> 4) & 0x0F;
	info->componentInfo[id].VSamplingFactor = sampling & 0x0F;
	info->componentInfo[id].quantizationTableID = quant_table;
    }
    return JPEG_OK;
}

int parse_quantization_table(FILE *file, jpeg_info *info) {

    return JPEG_OK;
}

int parse_huffman_table(FILE *file, jpeg_info *info) {
    uint16_t length = read_big_endian_16(file);
    uint16_t bytes_read = 2;

    while (bytes_read < length) {
	uint8_t ht_info = fgetc(file);
	bytes_read++;

	uint8_t table_class = (ht_info >> 4) & 0x01; // 0=DC, 1=AC
	uint8_t table_id = ht_info & 0x0F;
	
	if (table_id >=2) return JPEG_ERR;
	uint8_t jpeg_data[17+256];
	uint16_t total_symbols = 0;
	for (int i = 0; i < 16; i++) {
	    jpeg_data[i] = fgetc(file);
	    total_symbols += jpeg_data[i];
	    bytes_read++;
	}

	for (int i = 0; i < total_symbols; i++) {
	    jpeg_data[16 + i] = fgetc(file);
	    bytes_read++;
	}

	hufftree_parse_jpeg(&info->huffmanTables[table_class][table_id], jpeg_data);
    }
    return JPEG_OK;
}

int parse_scan_header(FILE *file, scan_info *scan) {

    return JPEG_OK;
}

