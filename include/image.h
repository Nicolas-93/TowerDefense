#ifndef IMAGE_H
#define IMAGE_H

#include "error.h"
#include <MLV/MLV_image.h>
#include "geom.h"

#define IMAGE_LEN 5

typedef enum ImageName {
    IMAGE_MONSTER = 0,
    IMAGE_TOWER,
    IMAGE_MANA_POOL,
    IMAGE_GEM_CREATE,
    IMAGE_GEM_MERGING,
} ImageName;

extern MLV_Image* IMAGES_LOADED[];

extern char* IMAGES_FILENAMES[];

/**
 * @brief Load images into IMAGES_LOADED array
 * 
 * @return Error 
 */
Error Image_load_all(void);

MLV_Image* Image_get(ImageName name, Size size);

#endif
