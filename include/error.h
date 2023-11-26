#ifndef ERROR_H
#define ERROR_H

typedef enum Error {
    ERR_ALLOC = -1,
    ERR_CASE_ALREADY_USED = -2,
    INFO_MONSTER_BACK_TO_SPAWN = -3,
    INFO_MONSETR_IS_DEAD = -4,
    INFO_NO_MONSTER_IN_RANGE = -5,
    INFO_MONSTER_TARGETED = -6,
} Error;

#endif
