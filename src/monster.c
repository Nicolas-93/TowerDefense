#include "monster.h"
#include "image.h"
#include "overlay.h"
#include "gfxutils.h"
#include "utils.h"
#include "effects.h"
#include <math.h>

static void _Monster_set_next_traj(Monster* self) {
    Traject_set_from_points(
        &self->traj,
        Grid_get_absolute_coords_C(
            self->grid,
            ArrayList_get_v(&self->path->waypoints, self->target_waypoint, Point)
        ),
        Grid_get_absolute_coords_C(
            self->grid,
            ArrayList_get_v(&self->path->waypoints, self->target_waypoint + 1, Point)
        )
    );
    self->target_waypoint++;
}

void Monster_new(
    Monster* self,
    Grid* grid,
    Path* path,
    const double h,
    float speed,
    uint16_t wave_counter,
    Timer start_timer
) {
    *self = (Monster) {
        .path = path,
        .grid = grid,
        .target_waypoint = 0,
        .color = Color_new_random(),
        .initial_hp = h * pow(1.2, wave_counter),
        .current_hp = self->initial_hp,
        .last_gem_impact = 0,
        .timer = Timer_new_ms(0),
        .start_timer = start_timer,
        .traj = Traject_new_without_dir_and_pos(grid->cell_width, speed),
    };
    self->current_hp = self->initial_hp;

    _Monster_set_next_traj(self);
    Deque_init(&self->future_shots, sizeof(Shot));
}

Error Monster_add_future_shot(Monster* self, const Shot* shot) {
    return Deque_append(&self->future_shots, shot) == ERR_DEQUE_ALLOC ? ERR_ALLOC : 0;
}

static inline int _Monster_get_base_damage(const Monster* self, const Shot* shot) {
    return 10 * pow(2, shot->gem.level) * (1 - cos(self->color.hsv.h - shot->gem.color.hsv.h) / 2);
}

/**
 * @brief Absorb a shot if it has reached him,
 * and update the monster's HP.
 * 
 * @param self 
 * @param shot 
 * @return true Died
 * @return false Still alive
 */
static bool _Monster_suffer_damages(Monster* self, const Shot* shot) {
    uint32_t damages = _Monster_get_base_damage(self, shot);

    if (damages > self->current_hp)
        return true;

    self->current_hp = clamp(self->current_hp - damages, 0, self->initial_hp);

    return false;
}

/**
 * @brief Attracts the shots that will be absorbed.
 * 
 * @param self 
 * @param gem 
 * @return INFO_MONSTER_IS_DEAD if a shot has reached him
 * and he died.
 */
static Error _Monster_anim_shots(Monster* self) {
    Error err = 0;
    DequeNode* entry, *tmp;
    DEQUE_FOREACH_SAFE(entry, &self->future_shots, tmp) {
        Shot* shot = Deque_get_elem(entry);
        Shot_update(shot);
        if (Shot_has_reached_target(shot)) {
            if (_Monster_suffer_damages(self, shot)) {
                err = INFO_MONSTER_IS_DEAD;
                break;
            }
            Effect_new(
                &self->effect,
                _Monster_get_base_damage(self, shot),
                Traject_get_speed(&self->traj),
                self->last_gem_impact,
                shot->gem.type
            );
            self->last_gem_impact = shot->gem.type;

            Deque_remove(&self->future_shots, entry);
        }
    }
    return err;
}

/**
 * @brief Move the monster on its path.
 * 
 * @param self Monster to move
 * @return Error INFO_MONSTER_BACK_TO_SPAWN if the monster
 * has reached the end of the path and is back to spawn.
 */
static Error _Monster_move(Monster* self) {
    Error err = 0;

    Point next_waypoint = Grid_get_absolute_coords_C(
        self->grid,
        ArrayList_get_v(&self->path->waypoints, self->target_waypoint, Point)
    );
    
    if (Traject_is_over(&self->traj, next_waypoint)) {
        if (self->target_waypoint == ArrayList_get_length(&self->path->waypoints) - 1) {
            self->target_waypoint = 0;
            err = INFO_MONSTER_BACK_TO_SPAWN;
        }
        _Monster_set_next_traj(self);
    }
    else {
        Traject_move(&self->traj);
    }

    return err;
}

Error Monster_update(Monster* self) {
    Error err = 0;
    if (!Timer_is_over(&self->start_timer))
        return 0;

    if ((err = _Monster_anim_shots(self)) == INFO_MONSTER_IS_DEAD) {
        return err;
    }
    if ((err = _Monster_move(self)) == INFO_MONSTER_BACK_TO_SPAWN) {
        return err;
    }

    return 0;
}

void Monster_draw(const Monster* self) {
    Point pos = self->traj.pos;
    const double radius = self->grid->cell_width / 3;

    MLV_draw_filled_circle(
        pos.x,
        pos.y,
        radius,
        self->color.mlvrgb
    );

    // Draw incomming shots
    DequeNode* entry;
    DEQUE_FOREACH(entry, &self->future_shots) {
        Shot* shot = Deque_get_elem(entry);
        Shot_draw(shot);
    }

    const float hp_bar_width = 4 * radius;
    const float percent = (double) self->current_hp / self->initial_hp;

    MLV_Color hp_bar_color = percent < 0.5 ?
                                (percent < 0.2 ?
                                    MLV_COLOR_RED1
                                    : MLV_COLOR_ORANGE1)
                                : MLV_COLOR_GREEN1;

    Gfx_draw_completion_bar(
        (Rect) {
            .a = (Point) {.x = pos.x - hp_bar_width / 2, .y = pos.y - radius - 10},
            .b = (Point) {.x = pos.x + hp_bar_width / 2, .y = pos.y - radius - 5}
        },
        percent,
        MLV_rgba(0, 0, 0, 0),
        hp_bar_color
    );

    if (Point_on_circle(Event_get().mouse, pos, radius)) {
        Overlay_draw(
            (Point) {.x = pos.x, .y = pos.y},
            "HP : %d/%d", self->current_hp, self->initial_hp
        );
    }
}

void Monster_free(Monster* self) {
    Deque_free(&self->future_shots);
    *self = (Monster) {0};
}

Point Monster_get_pos(const Monster* self) {
    return self->traj.pos;
}

int Monster_get_hp(const Monster* self) {
    return self->current_hp;
}

void Monster_set_speed(Monster* self, double speed) {
    Traject_set_speed(&self->traj, speed);
}

Error Monster_apply_damage(Monster* self, double damage) {
    self->current_hp = clamp(self->current_hp - damage, 0, self->initial_hp);
    if (self->current_hp <= 0) {
        return INFO_MONSTER_IS_DEAD;
    }
    return 0;
}
