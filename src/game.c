#include "game.h"

Error Game_new(Game* self, Size win_size) {
    Error err = 0;
    
    if ((err = Grid_new(
        &self->viewport, 10, 6, 0.95,
        NULL,
        (Rect) {.ax = 0, .ay = 0, .bx = win_size.width, .by = win_size.height},
        false,
        MLV_COLOR_WHITE, MLV_COLOR_RED
    ) < 0)) {
        return err;
    }

    if ((err = Land_new(&self->land, &self->viewport, 28, 22)) < 0) {
        return err;
    }

    Path_print(&self->land.path);

    // Inventory_new(&self->inv, 3, 12);

    return 0;
}

void Game_update(Game* self) {
    Land_update(&self->land);
}

void Game_draw(const Game* self) {
    Land_draw(&self->land);
    // Grid_draw_lines(&self.viewport);
}

void Game_process_event(Game* self) {
    Land_process_event(&self->land);
}
