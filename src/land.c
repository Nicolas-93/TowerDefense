#include "land.h"
#include "utils.h"
#include <assert.h>

static void _Land_set_grid_color(Land* self);

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
        true, MLV_COLOR_WHITE, MLV_COLOR_BLACK
    );
    _Land_set_grid_color(self);
    Land_new_random_monster_wave(self);

    return 0;
}

static void _Land_set_grid_color(Land* self) {
    for (int j = 0; j < self->grid.height; j++) {
        for (int i = 0; i < self->grid.width; i++) {
            Point current = (Point) {.x = i, .y = j};
            Cell* cell = Grid_get_cell(&self->grid, current);
            cell->filled_color = Land_is_path(self, current) ? MLV_COLOR_GRAY50
                                                             : MLV_COLOR_WHITE;
        }
    }

    Grid_get_cell(&self->grid, Path_get_start(&self->path))->filled_color = MLV_COLOR_GREEN;
    Grid_get_cell(&self->grid, Path_get_end(&self->path))->filled_color = MLV_COLOR_RED;
}

Error Land_add_tower(Land* self, Tower* tower) {
    assert(tower);

    if (Land_is_occupied(self, tower->pos)) {
        return ERR_CASE_ALREADY_USED;
    }

    Deque_append(&self->towers, tower);

    return 0;
}

/**
 * @brief Add a monster to the land
 * 
 * @param self 
 * @param monster 
 */
static void _Land_add_monster(Land* self, const Monster* monster) {
    Deque_append(&self->monsters, monster);
}

void Land_new_monster_wave(Land* self, MonsterWave wave) {
    for (int i = 0; i < wave.size; i++) {
        Monster monster;
        Monster_new(
            &monster,
            &self->grid,
            &self->path,
            H_PARAMETER,
            wave.speed,
            self->wave_counter,
            Timer_new_ms(1000 * i) // Make it a little more random : i +- rand()
        );
        _Land_add_monster(self, &monster);
    }
}

void Land_new_random_monster_wave(Land* self) {
    MonsterWave wave = MonsterWave_new_random(self->wave_counter);
    MonsterWave_print(wave);
    Land_new_monster_wave(self, wave);
}

bool Land_is_path(const Land* self, Point p) {
    return Path_is_path(&self->path, p);
}

bool Land_is_tower(const Land* self, Point p) {
    DequeNode* entry;

    DEQUE_FOREACH(entry, &self->towers)
        if (Vector2D_equals(Deque_get_elem_v(entry, Point), p))
            return true;
    
    return false;
}

bool Land_is_occupied(const Land* self, Point p) {
    return Land_is_path(self, p) || Land_is_tower(self, p);
}

void Land_anim(Land* self) {
    DequeNode* entry;
    DEQUE_FOREACH(entry, &self->monsters) {
        Monster_anim(Deque_get_elem(entry));
    }
}

void Land_draw(const Land* self) {
    Grid_draw_filled_rects(&self->grid);
    Grid_draw_lines(&self->grid);
    
    DequeNode* entry;
    DEQUE_FOREACH(entry, &self->monsters) {
        Monster_draw(Deque_get_elem(entry));
    }
}

void Land_free(Land* self) {

}
