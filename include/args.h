#ifndef ARGS_H
#define ARGS_H

typedef struct Args {
    struct {
        int width;
        int height;
        bool fullscreen;
        int fps;
    } win;
} Args;

#endif
