#include "mana.h"
#include "error.h"
#include "utils.h"
#include "gfxutils.h"
#include <MLV/MLV_all.h>

Error Mana_new(Mana* self, Grid* parent, Rect rect, uint32_t initial_mana, uint32_t max_mana) {
    *self = (Mana) {
        .level = 0,
        .mana = initial_mana,
        .max_mana = max_mana,
        .grid = parent,
        .rect = (Rect) {
            .a = Grid_get_cell_rect(parent, rect.a).a,
            .b = Grid_get_cell_rect(parent, rect.b).b,
        },
    };
    return 0;
}

ManaError Mana_add(Mana* self, int amount) {
    self->mana += amount;
    self->mana = clamp(self->mana, 0, self->max_mana);

    if (self->mana <= 0)
        return MANA_BANKRUPT;

    if (self->mana == self->max_mana) {
        return MANA_CAPACITY_REACHED;
    }

    return 0;
}

bool Mana_have_sufficient_mana(const Mana* self, uint32_t amount) {
    return self->mana >= amount;
}

bool Mana_buy(Mana* self, uint32_t amount) {
    if (!Mana_have_sufficient_mana(self, amount))
        return false;

    Mana_add(self, -amount);

    return true;
}

int Mana_get_pool_upgrade_cost(const Mana* self) {
    return 500 * pow(1.4, self->level);
}

int Mana_get_gem_cost(int level) {
    return 100 * pow(2, level);
}

int Mana_get_gem_merging_cost(void) {
    return 100;
}

ManaError Mana_upgrade(Mana* self) {
    uint32_t price = Mana_get_pool_upgrade_cost(self);

    if (!Mana_have_sufficient_mana(self, price))
        return false;

    self->level++;
    self->max_mana = 2000 * pow(1.4, self->level);

    return Mana_add(self, -price);
}


void Mana_draw(const Mana* self) {
    Rect rect = self->rect;

    Gfx_draw_completion_bar(
        rect,
        (float) self->mana / self->max_mana,
        MLV_rgba(44, 39, 68, 255),
        MLV_rgba(120, 182, 250, 255)
    );

    MLV_draw_text_box(
        rect.a.x, rect.a.y,
        Rect_get_width(rect), Rect_get_height(rect),
        "Mana : %d/%d",
        1,
        MLV_COLOR_BLACK, MLV_COLOR_WHITE, MLV_rgba(0, 0, 0, 0),
        MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER,
        self->mana, self->max_mana
    );
}
