#include "gem.h"

void Gem_new(Gem* self, int level) {
    Color color = Color_new_random();
    GemType type = 0;
    
    if (color.hsv.h >= 90 && color.hsv.h <= 150) {
        type = DENDRO;
    }
    else if (color.hsv.h >= 210 && color.hsv.h <= 270) {
        type = HYDRO;
    }
    else {
        type = PYRO;
    }

    *self = (Gem) {
        .color = color,
        .type = type,
        .level = level,
    };
}

void Gem_merge(Gem* self, Gem* other) {
    assert(self->level == other->level);

    ColorHSV new_color = {
        .h = (self->color.hsv.h + other->color.hsv.h) / 2,
        .s = 1,
        .v = 1,
    };

    self->color.hsv = new_color;
    self->level++;
}
