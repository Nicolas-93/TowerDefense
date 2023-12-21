#ifndef ARGS_H
#define ARGS_H

#include <stdbool.h>
#include "geom.h"

typedef struct Window {
    Size size;
    bool fullscreen;
    int fps;
} Window;

typedef struct Args {
    Window win;
} Args;

#endif
