#ifndef BMP_H
#define BMP_H
#include "image.h"
#include <stdint.h>
#include <stdio.h>

struct __attribute__((packed)) bmp_header {
    uint16_t bfType;
    uint32_t bfileSize;
    uint32_t bfReserved;
    uint32_t bOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

enum read_status {
    READ_OK = 0,
    READ_INVALID_SIGNATURE,
    READ_IO_ERROR,
};

enum write_status {
    WRITE_OK = 0,
    WRITE_IO_ERROR,
};

static const char* const read_status_messages[] = {
        [READ_OK]="Read successful",
        [READ_INVALID_SIGNATURE]="Error: invalid signature",
        [READ_IO_ERROR]="Error: Unable to read source image."
};

static const char* const write_status_messages[] = {
        [WRITE_OK]="Write successful",
        [WRITE_IO_ERROR]="Error: Unable to write transformed image."
};


enum read_status from_bmp(FILE* in, struct image* img);
enum write_status to_bmp(FILE* out, const struct image* img);
//uint32_t calculate_padding(uint32_t width);
#endif
