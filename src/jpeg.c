#include "../include/image-processor.h"
#include "../include/huff.h"

#include <stdint.h>
#include <stdio.h>
enum {
    JPEG_ERR = 1,
    JPEG_OK = 0
};

typedef struct {
    struct jpeg_component_info {
	unsigned char HSamplingFactor;
	unsigned char VSamplingFactor;
	unsigned char quantizationTableID;
    }componentInfo[256];
    unsigned char quantizationTables[4][64];
    huffnode *huffmanTables[2][4];
    unsigned short width;
    unsigned short height;
    unsigned char precision;
}jpeg_info;

typedef struct {
    struct scan_component_info {
	unsigned char componentID;
	unsigned char DCHuffmanTableID;
	unsigned char ACHuffmanTableID;
    } componentInfo[4];
    unsigned char maxHSamplingFactor;
    unsigned char maxVSamplingFactor;
    unsigned char componentCount;
}scan_info;

typedef enum SegmentMarker {
    SOI = 0xFFD8, // Start of Image
    SOF = 0xFFC0, // Start of Frame
    DHT = 0xFFC4, // Define Huffman Table
    DQT = 0xFFDB, // Define Quantisation Table
    SOS = 0xFFDA, // Start of Scan
    EOI = 0xFFD9, // End of Image
    APP0 = 0xFFE0, // Application Data
} SegmentMarker;

uint16_t read_big_endian_16(FILE *file){
    uint8_t bytes[2];
    if (fread(bytes, 1, 2, file) !=2) return 0;
    return (uint16_t)(bytes[0] << 8) | bytes[1];
}


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
