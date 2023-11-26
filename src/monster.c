#include "monster.h"
#include "image.h"
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
    self->current_hp = self->initial_hp; // Should be done in the initializer list but it doesn't work

    _Monster_set_next_traj(self);
    Deque_init(&self->future_shots, sizeof(Shot));
}

Error Monster_add_future_shot(Monster* self, const Shot* shot) {
    return Deque_append(&self->future_shots, shot);
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
        Shot_anim(shot);
        if (Shot_has_reached_target(shot)) {
            if (_Monster_suffer_damages(self, shot)) {
                err = INFO_MONSETR_IS_DEAD;
                break;
            }
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

Error Monster_anim(Monster* self) {
    Error err = 0;
    if (!Timer_is_over(&self->start_timer))
        return 0;

    if ((err = _Monster_anim_shots(self)) == INFO_MONSETR_IS_DEAD) {
        return err;
    }
    if ((err = _Monster_move(self)) == INFO_MONSTER_BACK_TO_SPAWN) {
        return err;
    }

    return 0;
}

void Monster_draw(const Monster* self) {
    /*static bool resized = false;
    if (!resized) {
        MLV_resize_image_with_proportions(
            Image_get(IMAGE_MONSTER),
            self->grid->cell_width,
            self->grid->cell_height
        );
        resized = true;
    }
    MLV_draw_image(Image_get(IMAGE_MONSTER), self->traj.pos.x, self->traj.pos.y);*/
    MLV_draw_filled_circle(
        self->traj.pos.x,
        self->traj.pos.y,
        self->grid->cell_width / 2,
        self->color.mlvrgb
    );

    // Draw incomming shots
    DequeNode* entry;
    DEQUE_FOREACH(entry, &self->future_shots) {
        Shot* shot = Deque_get_elem(entry);
        Shot_draw(shot);
    }
}
