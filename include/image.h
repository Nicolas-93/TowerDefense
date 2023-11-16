#ifndef IMAGE_H
#define IMAGE_H

#include "error.h"
#include <MLV/MLV_image.h>

#define IMAGE_LEN 1

typedef enum ImageName {
    IMAGE_MONSTER = 0,
    IMAGE_TOWER,
} ImageName;

extern MLV_Image* IMAGES_LOADED[];

extern char* IMAGES_FILENAMES[];

/**
 * @brief Load images into IMAGES_LOADED array
 * 
 * @return Error 
 */
Error Image_load_all(void);

MLV_Image* Image_get(ImageName name);

#endif
