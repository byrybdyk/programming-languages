#include <stdio.h>
#include <stdlib.h>

#include "angle.h"
#include "bmp.h"
#include "bmp_errors.h"
#include "image.h"
#include "transform.h"



int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <source-image> <transformed-image> <angle>\n", argv[0]);
        return 1;
    }

    int angle = atoi(argv[3]);
    angle = normalize_angle(angle);
    validate_angle( angle);


    const char* source_image_path = argv[1];
    const char* transformed_image_path = argv[2];
    
    struct image input_image;
    FILE* source_file = fopen(source_image_path, "rb");
    enum read_status read_status = from_bmp(source_file, &input_image);
    fclose(source_file);

    processing_read_status(read_status);

    
    struct image output_image = rotate_image(input_image, angle);
    FILE* transformed_file = fopen(transformed_image_path, "wb");


    enum write_status write_status = to_bmp(transformed_file, &output_image);
    fclose(transformed_file);



    if(processing_write_status(write_status)){
        free_image(&input_image);
        free_image(&output_image);
        return 1;
    }

    printf("Rotation complete.\n");
    if (output_image.data != NULL) {
        free(output_image.data);
        output_image.data = NULL;
    }
    return 0;
}
