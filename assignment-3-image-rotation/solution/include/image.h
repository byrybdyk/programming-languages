#pragma once
#include <stdbool.h>
#include <stdint.h>


struct pixel {
    uint8_t b, g, r;
};

struct image {
    uint64_t width, height;
    struct pixel* data;
};

void initialize_image(struct image* img, uint64_t width, uint64_t height);
void free_image(struct image* img);


