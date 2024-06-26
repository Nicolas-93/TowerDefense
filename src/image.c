#include "image.h"
#include "geom.h"

char* IMAGES_FILENAMES[] = {
    [IMAGE_TOWER] = "assets/tower.png",
    [IMAGE_MANA_POOL] = "assets/mana_pool.png",
    [IMAGE_GEM_CREATE] = "assets/gem_create.png",
    [IMAGE_GEM_MERGING] = "assets/gem_merge.png",
};

MLV_Image* IMAGES_LOADED[IMAGE_LEN];

Error Image_load_all(void) {
    for (int i = 0; i < IMAGE_LEN; ++i) {
        IMAGES_LOADED[i] = MLV_load_image(IMAGES_FILENAMES[i]);
        if (!IMAGES_LOADED[i])
            return ERR_IMAGE_LOAD;
    }

    return 0;
}

MLV_Image* Image_get(ImageName name, Size size) {
    Size old;
    int width, height;
    MLV_get_image_size(IMAGES_LOADED[name], &width, &height);
    old = (Size) {width, height};

    if (size.width != old.width || size.height != old.height)
        MLV_resize_image_with_proportions(IMAGES_LOADED[name], size.width, size.height);

    return IMAGES_LOADED[name];
}

void Image_free(void) {
    for (int i = 0; i < IMAGE_LEN; ++i) {
        MLV_free_image(IMAGES_LOADED[i]);
    }
}
