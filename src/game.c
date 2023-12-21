#include "game.h"
#include "dragndrop.h"

bool Game_on_gem_release(
    void* context, void* object,
    Point abs_pos
) {
    Game* game = (Game*) context;
    Gem* gem = (Gem*) object;
    
    if (Land_on_gem_release(&game->land, gem, abs_pos))
        return true;
    else if (Inventory_on_gem_release(&game->inv, gem, abs_pos))
        return true;

    return false;
}

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

    if ((err = Land_new(&self->land, &self->viewport, self, 28, 22)) < 0) {
        return err;
    }
    Path_print(&self->land.path);

    if ((err = Inventory_new(
        &self->inv, &self->viewport, self, (Size) {.width = 3, .height = 12}
    ) < 0)) {
        return err;
    }

    return 0;
}

void Game_update(Game* self) {
    Land_update(&self->land);
}

void Game_draw(const Game* self) {
    Land_draw(&self->land);
    Inventory_draw(&self->inv);
    DragNDrop_draw();
    // Grid_draw_lines(&self.viewport);
}

void Game_process_event(Game* self) {
    DragNDrop_process_event();
    Inventory_process_event(&self->inv);
    Land_process_event(&self->land);
}
