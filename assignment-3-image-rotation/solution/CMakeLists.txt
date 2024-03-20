file(GLOB_RECURSE sources CONFIGURE_DEPENDS
    src/*.c
    src/*.h
    include/*.h
)



add_executable(image-transformer ${sources} src/main.c src/transform.c src/bmp.c src/image.c
        src/bmp_errors.c src/angle.c include/transform.h include/bmp.h include/image.h
        include/bmp_errors.h include/angle.h
        include/bmp_errors.h
        include/angle.h)
target_include_directories(image-transformer PRIVATE src include)

