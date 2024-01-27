#include "error.h"

static const char* ERR_STR[] = {
    [-ERR_ALLOC] = "Allocation error",
    [-ERR_CASE_ALREADY_USED] = "Case already used",
    [-INFO_MONSTER_BACK_TO_SPAWN] = "Monster back to spawn",
    [-INFO_MONSTER_IS_DEAD] = "Monster is dead",
    [-INFO_NO_MONSTER_IN_RANGE] = "No monster in range",
    [-INFO_MONSTER_TARGETED] = "Monster targeted",
    [-ERR_IMAGE_LOAD] = "Image load error",
};

const char* Error_to_string(Error err) {
    return ERR_STR[-err];
}
