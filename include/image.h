#ifndef IMAGE_H
#define IMAGE_H

#include "error.h"
#include <MLV/MLV_image.h>
#include "geom.h"

#define IMAGE_LEN 4

typedef enum ImageName {
    IMAGE_TOWER = 0,
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

/**
 * @brief Get the Image object
 * 
 * @param name Image name (see ImageName enum)
 * @param size Requested size (resized if needed)
 * @return MLV_Image* 
 */
MLV_Image* Image_get(ImageName name, Size size);

/**
 * @brief Free all images
 * 
 */
void Image_free(void);

#endif
