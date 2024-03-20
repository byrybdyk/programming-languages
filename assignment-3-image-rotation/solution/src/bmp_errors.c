#include "stdbool.h"
#include "bmp.h"
#include "bmp_errors.h"

bool processing_img_open(FILE* source_file){
    if (!source_file) {
        perror("Error opening source image file");
        return 1;
    }
    return 0;
}

bool processing_read_status(enum read_status status){
    if (status != READ_OK) {
        perror(read_status_messages[status]);
        return 1;
    }
    return 0;
}

bool processing_write_status(enum write_status status){
    if (status != WRITE_OK) {
        perror(write_status_messages[status]);

        return 1;
    }
    return 0;
}

