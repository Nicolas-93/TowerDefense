#include "game.h"
#include "dragndrop.h"
#include "overlay.h"
#include "mana.h"
#include <stdio.h>

static int _Game_get_tower_cost(const Game* self) {
    return 100 * pow(
        2,
        Deque_get_length(&self->land.towers) + self->land.available_towers - 3
    );
}

static void Game_buy_tower(void* self) {
    Game* game = (Game*) self;
    uint32_t price = _Game_get_tower_cost(game);

    if (Mana_buy(&game->mana, price)) {
        game->land.available_towers++;
    }
}

static void Game_buy_mana_pool(void* self) {
    Game* game = (Game*) self;
    Mana_upgrade(&game->mana);
}

static void Game_buy_gem(void* self) {
    Game* game = (Game*) self;
    uint32_t price = Mana_get_gem_cost(game->counter.value);

    if (!Inventory_is_full(&game->inv) && Mana_buy(&game->mana, price)) {
        Gem* gem = Gem_new(&game->land.grid, game->counter.value);
        Inventory_put_random(&game->inv, gem);
    }
}

static void Game_buy_tower_draw_overlay(Point pos, void* self) {
    Game* game = (Game*) self;

    Overlay_draw(
        pos, "Available: %d\nNew tower cost: %d mana",
        game->land.available_towers,
        _Game_get_tower_cost(game)
    );
}
static void Game_buy_mana_pool_draw_overlay(Point pos, void* self) {
    Game* game = (Game*) self;

    Overlay_draw(
        pos,
        "Upgrade mana pool: %d",
        Mana_get_pool_upgrade_cost(&game->mana)
    );
}
static void Game_buy_gem_draw_overlay(Point pos, void* self) {
    Game* game = (Game*) self;

    Overlay_draw(pos, "Buy gem: %d", Mana_get_gem_cost(game->counter.value));
}

static void Game_buy_gem_merging_draw_overlay(Point pos, void* self) {
    Game* game = (Game*) self;

    Overlay_draw(pos, "Merge gems: %d", Mana_get_gem_cost(game->counter.value));
}

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
        &self->viewport, 32, 20, 0.95,
        NULL,
        (Rect) {.ax = 0, .ay = 0, .bx = win_size.width, .by = win_size.height},
        true,
        MLV_COLOR_WHITE, MLV_COLOR_RED
    ) < 0)) {
        return err;
    }

    if ((err = Land_new(
        &self->land,
        &self->viewport, (Rect) {.ax = 0, .ay = 2, .bx = 25, .by = 19},
        self, 28, 22
    )) < 0) {
        return err;
    }
    // Path_print(&self->land.path);

    if ((err = Mana_new(
        &self->mana, &self->viewport, (Rect) {.ax = 0, .ay = 0, .bx = 25, .by = 0},
        800, 2000
    )) < 0) {
        return err;
    }

    if ((err = Inventory_new(
        &self->inv,
        &self->viewport, (Rect) {.ax = 26, .ay = 8, .bx = 31, .by = 19},
        (Size) {.width = 3, .height = 12},
        &self->mana,
        self
    )) < 0) {
        return err;
    }

    if ((err = Counter_new(
        &self->counter, &self->viewport,
        (Rect) {.ax = 26, .ay = 5, .bx = 31, .by = 8},
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
                .on_hover = Game_buy_gem_draw_overlay,
                .context = self,
            }
        },{ .icon = IMAGE_GEM_MERGING,
            .callback = {
                .on_hover = Game_buy_gem_merging_draw_overlay,
                .context = self,
            }
        },
    }, sizeof(buttons));
    
    Buttons_new(
        &self->buttons,
        &self->viewport,
        (Rect) {.ax = 26, .ay = 1, .bx = 31, .by = 5},
        (Size) {.width = 3, .height = 2},
        buttons, 4
    );

    return 0;
}

bool Game_is_game_over(const Game* self) {
    return Mana_get_value(&self->mana) <= 0;
}

void Game_update(Game* self) {
    Land_update(&self->land);
}

void Game_draw(const Game* self) {
    Land_draw(&self->land);
    Mana_draw(&self->mana);
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

void Game_draw_game_over(const Game* self) {
    Rect rect = self->mana.rect;
    MLV_draw_text_box(
        rect.a.x, rect.a.y,
        Rect_get_width(rect), Rect_get_height(rect),
        "Game Over",
        15,
        MLV_COLOR_RED, MLV_COLOR_WHITE, MLV_COLOR_BLACK,
        MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
    );
}
