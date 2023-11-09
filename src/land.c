#include "land.h"
#include "deque.h"


Error Land_new(Land* self, Grid* parent, uint16_t width, uint16_t height) {
    *self = (Land) {
        .wave_counter = 0,
    };

    Deque_init(&self->monsters, sizeof(Monster));
    // Path_new(&self->path);
    Deque_init(&self->towers, sizeof(Tower));
    Grid_new(
        &self->grid,
        width, height, 0.9,
        parent, (Rect) {.ax = 1, .ay = 1, .bx = 10, .by = 10},
        true, MLV_COLOR_BLACK, MLV_COLOR_BLACK
    );

    return 0;
}
