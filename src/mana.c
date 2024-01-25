#include "mana.h"
#include "error.h"
#include "utils.h"
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
    self->mana += clamp(amount, 0, self->max_mana);

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

ManaError Mana_upgrade(Mana* self) {
    uint32_t price = 500 * pow(1.4, self->level);

    if (!Mana_have_sufficient_mana(self, price))
        return false;

    self->level++;
    self->max_mana = 2000 * pow(1.4, self->level);

    return Mana_add(self, -price);
}


void Mana_draw(const Mana* self) {
    Rect rect = self->rect;

    // background
    MLV_draw_filled_rectangle(
        rect.a.x, rect.a.y,
        Rect_get_width(rect), Rect_get_height(rect),
        MLV_rgba(44, 39, 68, 255)
    );

    // Draw completed
    MLV_draw_filled_rectangle(
        rect.a.x, rect.a.y,
        (Rect_get_width(rect) * self->mana) / self->max_mana,
        Rect_get_height(rect),
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
