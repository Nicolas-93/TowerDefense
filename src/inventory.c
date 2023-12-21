#include "inventory.h"
#include "gem.h"
#include "utils.h"
#include "dragndrop.h"
#include "game.h"


bool Inventory_on_gem_release(void* context, void* object, Point abs_pos) {
    Inventory* inv = (Inventory*) context;
    Gem* gem = (Gem*) object;
    Point pos;

    if (!Grid_absolute_pos_to_relative(&inv->grid, abs_pos, &pos))
        return false;

    if (Inventory_put(inv, gem, pos))
        return true;

    return false;
}

static void _Inventory_on_gem_release_failure(
    void* context, void* object,
    Point origin
) {
    Inventory_on_gem_release((Inventory*) context, (Gem*) object, origin);
}

static void _Inventory_on_grid_click(Point pos, void* data) {
    Inventory* self = (Inventory*) data;

    if (!Inventory_get(self, pos)) {
        return;
    }

    Gem* gem = Inventory_pop(self, pos);

    DragNDrop_put(
        gem,
        Gem_draw_dragndrop,
        self->game,
        Game_on_gem_release,
        self,
        _Inventory_on_gem_release_failure
    );
}

Error Inventory_new(Inventory* self, Grid* parent, void* game, Size size) {
    *self = (Inventory) {
        .width = size.width,
        .height = size.height,
        .gems = calloc(size.width * size.height, sizeof(Gem*)),
        .game = game
    };

    if (self->gems == NULL)
        return ERR_ALLOC;

    if (Grid_new(
        &self->grid,
        self->width, self->height, 1,
        parent /*10x6*/, (Rect) {.ax = 8, .ay = 1, .bx = 9, .by = 4},
        true, MLV_COLOR_WHITE, MLV_COLOR_BLACK
    ) < 0) {
        return ERR_ALLOC;
    }

    Grid_set_event_handler(&self->grid, _Inventory_on_grid_click, self);

    return 0;
}

void Inventory_free(Inventory* self) {
    free(self->gems);
    Grid_free(&self->grid);
}

bool Inventory_put(Inventory* self, Gem* gem, Point pos) {
    DECLARE_2D_VLA(gems, Gem*, self->gems, self->width);

    int x = pos.x, y = pos.y;

    if (gems[y][x] != NULL)
        return false;

    gems[y][x] = gem;

    Gem_set_grid(gem, &self->grid);

    return true;
}

Gem* Inventory_pop(Inventory* self, Point pos) {
    DECLARE_2D_VLA(gems, Gem*, self->gems, self->width);
    int x = pos.x, y = pos.y;

    Gem* gem = Inventory_get(self, pos);

    if (gem == NULL)
        return NULL;

    gems[y][x] = NULL;

    return gem;
}

Gem* Inventory_get(const Inventory* self, Point pos) {
    DECLARE_2D_VLA(gems, Gem*, self->gems, self->width);
    int x = pos.x, y = pos.y;

    return gems[y][x];
}

void Inventory_draw(const Inventory* self) {
    DECLARE_2D_VLA(gems, Gem*, self->gems, self->width);
    Grid_draw_filled_rects(&self->grid);
    Grid_draw_lines(&self->grid);

    for (int i = 0; i < self->width; i++) {
        for (int j = 0; j < self->height; j++) {
            if (gems[j][i]) {
                Gem_draw_grid(gems[j][i], (Point) {.x = i, .y = j});
            }
        }
    }
}

void Inventory_process_event(Inventory* self) {
    Grid_process_event(&self->grid);
}
