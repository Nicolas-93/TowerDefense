#ifndef GRID_H
#define GRID_H

#include "error.h"

typedef struct Grid {
    bool* GRID;
    int width;
    int height;
} Grid;

Error Grid_init(Grid* self, int width, int height);

void Grid_generate(Grid* self);

void Grid_draw(Grid* self);

#endif
