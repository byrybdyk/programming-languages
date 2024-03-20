#include "image.h"

#include <stdlib.h>

void initialize_image(struct image* img, uint64_t width, uint64_t height) {
    img->width = width;
    img->height = height;
    if (width == 0 || height == 0) {
        img->data = NULL;
    } else img->data = malloc(width * height * sizeof(struct pixel));

}

void free_image(struct image* img) {
    free(img->data);
}

