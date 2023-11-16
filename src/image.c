#include "image.h"

char* IMAGES_FILENAMES[] = {
    [IMAGE_MONSTER] = "assets/sheep.png",
};

MLV_Image* IMAGES_LOADED[IMAGE_LEN];

Error Image_load_all(void) {
    for (int i = 0; i < IMAGE_LEN; ++i) {
        IMAGES_LOADED[i] = MLV_load_image(IMAGES_FILENAMES[i]);
    }

    return 0;
}

MLV_Image* Image_get(ImageName name) {
    return IMAGES_LOADED[name];
}
