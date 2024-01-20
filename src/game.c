#include "game.h"
#include "dragndrop.h"
#include "overlay.h"
#include <stdio.h>

static void Game_buy_tower(void* self) {}
static void Game_buy_mana_pool(void* self) {}
static void Game_buy_gem(void* self) {}
static void Game_buy_gem_merging(void* self) {}

static void Game_buy_tower_draw_overlay(Point pos, void* self) {
    // Game* game = (Game*) self;
    Overlay_draw(pos, "New tower\n(cost: %d mana)", 5);
}
static void Game_buy_mana_pool_draw_overlay(Point pos, void* self) {
    // Game* game = (Game*) self;
    Overlay_draw(pos, "Upgrade mana pool\n(cost: %d mana)", 50);
}
static void Game_buy_gem_draw_overlay(void* self) {}
static void Game_buy_gem_merging_draw_overlay(void* self) {}


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
        true,
        MLV_COLOR_WHITE, MLV_COLOR_RED
    ) < 0)) {
        return err;
    }

    if ((err = Land_new(&self->land, &self->viewport, self, 28, 22)) < 0) {
        return err;
    }
    // Path_print(&self->land.path);

    if ((err = Inventory_new(
        &self->inv, &self->viewport, self, (Size) {.width = 3, .height = 12}
    ) < 0)) {
        return err;
    }

    if ((err = Counter_new(
        &self->counter, &self->viewport,
        (Rect) {.ax = 8, .ay = 1, .bx = 9, .by = 1},
        0, 5
    )) < 0) {
        return err;
    }

    static Button buttons[4];
    memcpy(buttons, (Button[]) {{
            .icon = IMAGE_TOWER,
            .callback = {
                .on_click = Game_buy_tower,
                .on_hover = Game_buy_tower_draw_overlay,
                .context = self,
            }
        },{ .icon = IMAGE_MANA_POOL,
            .callback = {
                .on_click = Game_buy_mana_pool,
                .on_hover = Game_buy_mana_pool_draw_overlay,
                .context = self,
            }
        },{ .icon = IMAGE_GEM_CREATE,
            .callback = {
                .on_click = Game_buy_gem,
                .context = self,
            }
        },{ .icon = IMAGE_GEM_MERGING,
            .callback = {
                .on_click = Game_buy_gem_merging,
                .context = self,
            }
        },
    }, sizeof(buttons));
    
    Buttons_new(
        &self->buttons,
        &self->viewport,
        (Rect) {.ax = 8, .ay = 0, .bx = 9, .by = 0},
        (Size) {.width = 3, .height = 2},
        buttons, 4
    );

    return 0;
}

void Game_update(Game* self) {
    Land_update(&self->land);
}

void Game_draw(const Game* self) {
    Land_draw(&self->land);
    Inventory_draw(&self->inv);
    Buttons_draw(&self->buttons);
    Counter_draw(&self->counter);
    DragNDrop_draw();
    // Grid_draw_lines(&self->viewport);
}

void Game_process_event(Game* self) {
    DragNDrop_process_event();
    Inventory_process_event(&self->inv);
    Land_process_event(&self->land);
    Buttons_process_event(&self->buttons);
    Counter_process_event(&self->counter);
}

void Game_free(Game* self) {
    Counter_free(&self->counter);
    Inventory_free(&self->inv);
    Land_free(&self->land);
    Grid_free(&self->viewport);
    Buttons_free(&self->buttons);
}
