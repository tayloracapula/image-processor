#ifndef HUFF_H
#define HUFF_H

#include <stdint.h>
#include <stdbool.h>

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
#endif
