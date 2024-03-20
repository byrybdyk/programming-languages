#include "angle.h"

#include <stdio.h>

#define DEGREES_90  90
#define DEGREES_360  360


int normalize_angle(int angle) {
    while (angle <= 0) {
        angle += DEGREES_360;
    }
    angle %= DEGREES_360;


    return angle;
}

bool validate_angle( int angle){

    if (angle % DEGREES_90 != 0) {
        fprintf(stderr, "Error: Angle must be a multiple of 90 degrees.\n");
        return 1;
    }
    return 0;
}

