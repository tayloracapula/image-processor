#ifndef JPEG_H
#define JPEG_H

#include "image-processor.h"
#include "huff.h"

#include <stdint.h>
#include <stdio.h>
typedef struct {
    struct jpeg_component_info {
	unsigned char HSamplingFactor;
	unsigned char VSamplingFactor;
	unsigned char quantizationTableID;
    }componentInfo[256];
    unsigned char quantizationTables[4][64];
    huffnode huffmanTables[2][4];
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
    APP0 = 0xFFE0, // Application Data
    DQT = 0xFFDB, // Define Quantisation Table
    SOF = 0xFFC0, // Start of Frame
    DHT = 0xFFC4, // Define Huffman Table
    SOS = 0xFFDA, // Start of Scan
    EOI = 0xFFD9, // End of Image
} SegmentMarker;


int jpeg_load(const char *path, image_t *out_img);

int jpeg_write(const char *path, const image_t *img);


#endif
