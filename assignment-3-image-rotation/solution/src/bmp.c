#include "bmp.h"
#include "bmp_errors.h"

#include <stdio.h>
#include <string.h>


#define BMP_SIGNATURE       0x4D42
#define BITS_PER_PIXEL      24
#define PADDING             4
#define BPM_HEADER_SIZE     40
#define SIZE_OF_BPM_HEADER  sizeof(struct bmp_header)
#define PIXEL_SIZE          sizeof(struct pixel)


static uint32_t calculate_padding(uint32_t width) {
    return (PADDING - (width * PIXEL_SIZE) % PADDING) % PADDING;
}

enum read_status from_bmp(FILE* in, struct image* img) {
    if( processing_img_open(in)){
        return READ_IO_ERROR;
    }

    struct bmp_header header;
    if (fread(&header, SIZE_OF_BPM_HEADER, 1, in) != 1) {
        return READ_IO_ERROR;
    }

    if (header.bfType != 0x4D42) {
        return READ_INVALID_SIGNATURE;
    }

    initialize_image(img, header.biWidth, header.biHeight);

    uint32_t width = header.biWidth;
    uint32_t height = header.biHeight;
    uint32_t padding = calculate_padding(width);

    for (uint64_t i = 0; i < height; ++i) {
        if (!fread(img->data + i * width, PIXEL_SIZE * width, 1, in)) {
            return READ_IO_ERROR;
        }
        if (fseek(in, padding, SEEK_CUR)) {
            return READ_IO_ERROR;
        }
    }

    return READ_OK;
}

enum write_status to_bmp(FILE* out, const struct image* img) {

    if( processing_img_open(out)){
     return WRITE_IO_ERROR;
    }

    struct bmp_header header;
    header.bfType = BMP_SIGNATURE;
    header.bfileSize = SIZE_OF_BPM_HEADER + img->width * img->height * PIXEL_SIZE * 2;
    header.bfReserved = 0;
    header.bOffBits = SIZE_OF_BPM_HEADER;
    header.biSize = BPM_HEADER_SIZE;
    header.biWidth = img->width;
    header.biHeight = img->height;
    header.biPlanes = 1;
    header.biBitCount = BITS_PER_PIXEL;
    header.biCompression = 0;
    header.biSizeImage = 0;
    header.biXPelsPerMeter = 0;
    header.biYPelsPerMeter = 0;
    header.biClrUsed = 0;
    header.biClrImportant = 0;

    if (fwrite(&header, SIZE_OF_BPM_HEADER, 1, out) != 1) {
        return WRITE_IO_ERROR;
    }


    uint32_t padding = calculate_padding(img->width);
    uint8_t padBytes[padding];
    memset(padBytes, 0, padding);
    for (uint64_t y = 0; y < img->height; ++y) {

        if (fwrite(img->data + y * img->width, PIXEL_SIZE,
                img->width, out) != img->width) {
            return WRITE_IO_ERROR;
        }

        if (fwrite(padBytes, 1, padding, out) != padding) {
            return WRITE_IO_ERROR;
        }
    }

    return WRITE_OK;
}



