#include "land.h"
#include "deque.h"


Error Land_new(Land* self, Grid* parent, uint16_t width, uint16_t height) {
    *self = (Land) {
        .wave_counter = 0,
    };

    Deque_init(&self->monsters, sizeof(Monster));
    Path_new(&self->path);
    Deque_init(&self->towers, sizeof(Tower));
    Grid_new(
        &self->grid,
        width, height, 1,
        parent, (Rect) {.ax = 0, .ay = 0, .bx = 7, .by = parent->height - 1},
        true, MLV_COLOR_WHITE, MLV_COLOR_WHITE
    );

    return 0;
}

void Land_add_tower(Land* self, Tower* tower) {

}

void _Land_add_monster(Land* self, Monster* monster) {

}

void Land_add_monster_wave(Land* self) {

}

bool Land_is_path(const Land* self, const uint16_t x, const uint16_t y) {
    return false;
}

bool Land_is_tower(const Land* self, const uint16_t x, const uint16_t y) {
    return false;
}

bool Land_is_occupied(const Land* self, const uint16_t x, const uint16_t y) {
    return false;
}

void Land_anim(Land* self) {

}

void Land_draw(const Land* self) {

}

void Land_free(Land* self) {

}
