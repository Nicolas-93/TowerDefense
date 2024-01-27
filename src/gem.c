#include "gem.h"
#include "overlay.h"
#include "geom.h"
#include <assert.h>

const char* GEM_TYPES_STR[] = {
    [0]         = "None",
    [PYRO]      = "Pyro",
    [DENDRO]    = "Dendro",
    [HYDRO]     = "Hydro",
    [MIXED]     = "Mixed",
};

Gem* Gem_new(Grid* grid, int level) {
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

    Gem* self = malloc(sizeof(Gem));

    *self = (Gem) {
        .color = color,
        .type = type,
        .level = level,
        .grid = grid,
    };

    return self;
}

bool Gem_is_mergeable(const Gem* self, const Gem* other) {
    return self->level == other->level;
}

void Gem_merge(Gem* self, Gem* other) {
    assert(Gem_is_mergeable(self, other));

    ColorHSV new_color = {
        .h = (self->color.hsv.h + other->color.hsv.h) / 2,
        .s = 1,
        .v = 1,
    };

    self->color.hsv = new_color;
    self->level++;

    Gem_free(other);
}

void Gem_draw_grid(Gem* self, Point pos_rel) {
    Point pos_abs = Grid_get_absolute_coords_C(self->grid, pos_rel);
    Gem_draw_dragndrop(self, pos_abs);
}

const char* Gem_get_type_str(Gem* self) {
    return GEM_TYPES_STR[self->type];
}

void Gem_draw_dragndrop(void* self, Point pos_abs) {
    Gem* gem = (Gem*) self;

    double radius = gem->grid->cell_width / 3.0;

    MLV_draw_filled_circle(
        pos_abs.x, pos_abs.y,
        radius,
        gem->color.mlvrgb
    );

    if (Point_on_circle(Event_get().mouse, pos_abs, radius)) {
        Overlay_draw(
            pos_abs,
            "Level : %d\nType : %s", gem->level, Gem_get_type_str(gem)
        );
    }
}

void Gem_set_grid(Gem* self, Grid* grid) {
    self->grid = grid;
}

void Gem_free(Gem* self) {
    *self = (Gem) {0};
    free(self);
}
