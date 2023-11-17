#include "monster.h"
#include "image.h"
#include <math.h>

static void _Monster_set_speed(Monster* self, double speed) {
    self->traj.speed = speed * self->grid->cell_width / MLV_get_frame_rate();
}

static double _Monster_get_speed(const Monster* self) {
    return self->traj.speed * MLV_get_frame_rate() / self->grid->cell_width;
}

static void _Monster_set_next_traj(Monster* self) {
    self->traj = Traject_new_from_points(
        Grid_get_absolute_coords_C(
            self->grid,
            ArrayList_get_v(&self->path->waypoints, self->target_waypoint, Point)
        ),
        Grid_get_absolute_coords_C(
            self->grid,
            ArrayList_get_v(&self->path->waypoints, self->target_waypoint + 1, Point)
        ),
        self->traj.speed
    );
    self->target_waypoint++;
}

void Monster_new(
    Monster* self,
    Grid* grid,
    Path* path,
    const int h,
    float speed,
    uint16_t wave_counter,
    Timer start_timer
) {
    *self = (Monster) {
        .path = path,
        .grid = grid,
        .target_waypoint = 0,
        .color = (Color) {.hsv = {0}, .mlvrgb = MLV_COLOR_BLACK}, //Color_new_random(),
        .initial_hp = h * pow(1.2, wave_counter),
        .current_hp = self->initial_hp,
        .last_gem_impact = 0,
        .timer = Timer_new_ms(0),
        .start_timer = start_timer,
    };
    _Monster_set_speed(self, speed);
    _Monster_set_next_traj(self);
    Deque_init(&self->future_shots, sizeof(Shot));
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
 */
static void _Monster_anim_shots(Monster* self) {
    
}

static void _Monster_move(Monster* self) {
    if (self->target_waypoint == ArrayList_get_length(&self->path->waypoints)) {
        self->target_waypoint = 0;
        _Monster_set_next_traj(self);
    }

    if (Traject_is_over(
        &self->traj,
        Grid_get_absolute_coords_C(
            self->grid,
            ArrayList_get_v(&self->path->waypoints, self->target_waypoint, Point)
        ))) {
        _Monster_set_next_traj(self);
    }
    else {
        Traject_move(&self->traj);
    }
}

bool Monster_anim(Monster* self) {
    if (Timer_is_over(&self->start_timer)) {
        _Monster_anim_shots(self);
        _Monster_move(self);
    }

    return false;
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
}
