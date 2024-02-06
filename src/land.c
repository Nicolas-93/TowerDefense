#include "land.h"
#include "utils.h"
#include <assert.h>
#include "dragndrop.h"
#include "game.h"
#include "effects.h"

static void _Land_set_grid_color(Land* self);

static Tower* Land_get_tower(const Land* self, Point p);

bool Land_on_gem_release(
    void* context, void* object,
    Point abs_pos
) {
    Land* land = (Land*) context;
    Gem* gem = (Gem*) object;
    Tower* tower;
    Point pos;

    if (!Grid_absolute_pos_to_relative(&land->grid, abs_pos, &pos))
        return false;

    if ((tower = Land_get_tower(land, pos)) != NULL) {
        if (!Tower_is_empty(tower))
            return false;

        Tower_set_gem(tower, gem);
        Gem_set_grid(gem, &land->grid);
        return true;
    }

    return false;
}

static void _Land_on_gem_release_failure(
    void* context, void* object,
    Point origin
) {
    Land_on_gem_release((Land*) context, (Gem*) object, origin);
}

static void _Land_on_grid_click(Point pos, void* data) {
    Land* self = (Land*) data;

    if (Land_is_occupied(self, pos)) {
        Tower* tower;

        if ((tower = Land_get_tower(self, pos)) != NULL && Tower_has_gem(tower)) {
            DragNDrop_put(
                Tower_pop_gem(tower),
                Gem_draw_dragndrop,
                self->game,
                Game_on_gem_release,
                self,
                _Land_on_gem_release_failure
            );
        }
    }
    else if (self->available_towers > 0) {
        Tower tower;
        Tower_new(&tower, &self->grid, pos);
        Land_add_tower(self, &tower);
        self->available_towers--;
    }
}

Error Land_new(Land* self, Grid* parent, Rect rect, Game* game, uint16_t width, uint16_t height) {
    *self = (Land) {
        .wave_counter = 0,
        .game = game,
        .available_towers = 3,
    };

    Deque_init(&self->monsters, sizeof(Monster));
    Path_new(&self->path);
    Deque_init(&self->towers, sizeof(Tower));
    Grid_new(
        &self->grid,
        width, height, 1,
        parent, rect,
        true, MLV_COLOR_WHITE, MLV_COLOR_BLACK
    );
    Grid_set_on_click_handler(&self->grid, _Land_on_grid_click, self);
    _Land_set_grid_color(self);
    //Land_new_random_monster_wave(self);

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
    return Land_get_tower(self, p) != NULL;
}

/**
 * @brief Get the tower object at position ``p``.
 * 
 * @param self Land object
 * @param p Position of the tower relative to the grid
 * @return Tower* Found tower or NULL if none
 */
static Tower* Land_get_tower(const Land* self, Point p) {
    DequeNode* entry;

    DEQUE_FOREACH(entry, &self->towers)
        if (Vector2D_equals(Deque_get_elem_v(entry, Tower).pos, p))
            return (Tower*) Deque_get_elem(entry);
    
    return NULL;
}

bool Land_is_occupied(const Land* self, Point p) {
    return Land_is_path(self, p) || Land_is_tower(self, p);
}

void Land_process_event(Land* self) {
    Grid_process_event(&self->grid);
}

void Land_update(Land* self) {
    DequeNode* entry;
    DequeNode* tmp;
    Error err = 0;

    if (Timer_is_over(&self->next_wave_timer)) {
        Land_new_random_monster_wave(self);
        self->wave_counter++;
        self->next_wave_timer = Timer_new_ms(35000);
    }

    DEQUE_FOREACH_SAFE(entry, &self->monsters, tmp) {
        Monster* monster = Deque_get_elem(entry);
        err = Monster_update(monster);
        if (err == INFO_MONSTER_IS_DEAD) {
            Mana_add(
                &self->game->mana,
                Mana_get_killed_monster_bonus(&self->game->mana, monster->initial_hp)
            );
            Monster_free(monster);
            Deque_remove(&self->monsters, entry);
            continue;
        }
        else if (err == INFO_MONSTER_BACK_TO_SPAWN) {
            Mana_add(
                &self->game->mana,
                -Mana_get_monster_back_to_spawn_malus(&self->game->mana, monster->initial_hp)
            );
        }
        Effect_update(entry, &self->monsters);
    }
    DEQUE_FOREACH(entry, &self->towers) {
        Tower_update(Deque_get_elem(entry), &self->monsters);
    }
}

void Land_draw(const Land* self) {
    Grid_draw_filled_rects(&self->grid);
    Grid_draw_lines(&self->grid);
    
    DequeNode* entry;
    DEQUE_FOREACH_REVERSE(entry, &self->monsters) {
        Monster_draw((Monster*) Deque_get_elem(entry));
    }
    DEQUE_FOREACH(entry, &self->towers) {
        Tower_draw((Tower*) Deque_get_elem(entry));
    }
}

void Land_free(Land* self) {
    DequeNode* entry;
    DEQUE_FOREACH(entry, &self->monsters) {
        Monster_free(Deque_get_elem(entry));
    }
    Deque_free(&self->monsters);

    DEQUE_FOREACH(entry, &self->towers) {
        Tower_free(Deque_get_elem(entry));
    }
    Deque_free(&self->towers);

    Path_free(&self->path);
    Grid_free(&self->grid);
    *self = (Land) {0};
}
