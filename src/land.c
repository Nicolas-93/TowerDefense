#include "land.h"
#include "deque.h"
#include "utils.h"
#include <assert.h>

const MonsterWave WAVES[NB_MONSTER_WAVES] = {
    [WAVE_NORMAL] = {
        .type = WAVE_NORMAL,
        .probability = 50,
        .size = 12,
        .speed = 1,
    },
    [WAVE_CROWD] = {
        .type = WAVE_CROWD,
        .probability = 20,
        .size = 24,
        .speed = 1,
    },
    [WAVE_AGILE] = {
        .type = WAVE_AGILE,
        .probability = 20,
        .size = 12,
        .speed = 2,
    },
    [WAVE_BOSS] = {
        .type = WAVE_BOSS,
        .probability = 10,
        .size = 2,
        .speed = 1,
    },
};

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

    return 0;
}

static void _Land_set_grid_color(Land* self) {
    for (int j = 0; j < self->grid.height; j++) {
        for (int i = 0; i < self->grid.width; i++) {
            Cell* cell = Grid_get_cell(
                &self->grid, (Point) {.x = i, .y = j}
            );
            if (Land_is_path(self, i, j)) {
                cell->filled_color = MLV_COLOR_GRAY50;
            }
            else {
                cell->filled_color = MLV_COLOR_WHITE;
            }
        }
    }

    Cell* start_cell = Grid_get_cell(&self->grid, Path_get_start(&self->path));
    Cell* end_cell   = Grid_get_cell(&self->grid, Path_get_end(&self->path));

    start_cell->filled_color = MLV_COLOR_GREEN;
    end_cell->filled_color = MLV_COLOR_RED;
}

Error Land_add_tower(Land* self, Tower* tower) {
    assert(tower);

    if (Land_is_occupied(self, tower->pos.x, tower->pos.y)) {
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
static void _Land_add_monster(Land* self, Monster* monster) {
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
            Timer_new_ms(500 * i) // Make it a little more random : i +- rand()
        );
        _Land_add_monster(self, &monster);
    }
}

void Land_new_random_monster_wave(Land* self) {
    MonsterWaveType wave;

    do {
        static const int probabilities[NB_MONSTER_WAVES] = WAVES_PROBABILITIES;
        wave = weighted_selection(NB_MONSTER_WAVES, probabilities);
    } while (self->wave_counter < 5 && wave == WAVE_BOSS);

    self->wave_counter++;
    Land_new_monster_wave(self, WAVES[wave]);
}

bool Land_is_path(const Land* self, const uint16_t x, const uint16_t y) {
    return Path_is_path(&self->path, x, y);
}

bool Land_is_tower(const Land* self, const uint16_t x, const uint16_t y) {
    DequeNode* entry;

    DEQUE_FOREACH(entry, &self->towers)
        if (Vector2D_equals(Deque_get_elem_v(entry, Point), (Point) {.x = x, .y = y}))
            return true;
    
    return false;
}

bool Land_is_occupied(const Land* self, const uint16_t x, const uint16_t y) {
    return Land_is_path(self, x, y)
        || Land_is_tower(self, x, y);
}

void Land_anim(Land* self) {

}

void Land_draw(const Land* self) {
    Grid_draw_filled_rects(&self->grid);
    Grid_draw_lines(&self->grid);
}

void Land_free(Land* self) {

}
