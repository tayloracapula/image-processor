#ifndef HUFF_H
#define HUFF_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum HuffTableClass {
    CLASS_DC = 0,
    CLASS_AC = 1
}HuffTableClass;

typedef struct huffnode {
    struct huffnode *left;
    struct huffnode *right;
    uint8_t symbol;
    bool hasLeft;
    bool hasRight;
    bool hasSymbol;
} huffnode;

typedef struct {
    FILE *_fp;
    unsigned _bitsLeft;
    unsigned char _c;
} bitstream;

size_t hufftree_parse_jpeg(huffnode *buf, const unsigned char *data);
static huffnode *create_huff_node();
void hufftree_destroy(huffnode *node);

#define FAIL(...) { fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); exit(EXIT_FAILURE); }
#define CHECK_ALLOC(varName, name) CHECK_FAIL(varName == NULL, "Could not allocate memory for %s.", name)
#define CHECK_FAIL(cond, ...) if (cond) FAIL(__VA_ARGS__)

#endif
