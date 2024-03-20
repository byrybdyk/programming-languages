#include "transform.h"

#include <stdlib.h>

static struct image rotate_image_90degrees(const struct image* image) {

    struct image output;

    if (image->width == 0 || image->height == 0) {
        return (struct image) {.height = 0, .width = 0, .data = NULL};
    }

    output.width = image->height;
    output.height = image->width;
    output.data = malloc(sizeof(struct pixel) * output.width * output.height);
    if (output.data == NULL) return (struct image) {.height = 0, .width = 0, .data = NULL};

    for (size_t i = 0; i < output.width; i++) {
        for (size_t j = 0; j < output.height; j++) {
            output.data[(output.height - (j + 1)) * output.width + i] = image->data[i * image->width + j];
        }
    }

    return output;
}

struct image rotate_image(struct image image, int angle) {
    struct image temp_image = image;
    for (int8_t i = 0; i < (int8_t)(angle / 90); i++) {
        struct image output_image = rotate_image_90degrees(&temp_image);

        if (temp_image.data != NULL) {
            free(temp_image.data);
            temp_image.data = NULL;
        }

        temp_image = output_image;
        output_image.data = NULL;
    }
    return temp_image;
}


