#include "monster.h"
#include <math.h>

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
        .traj = Traject_new_from_points(
            ArrayList_get_v(&path->waypoints, 0, Point),
            ArrayList_get_v(&path->waypoints, 1, Point),
            speed
        ),
        .path = path,
        .grid = grid,
        .target_waypoint = 1,
        .color = (Color) {.hsv = {0}, .mlvrgb = 0}, //Color_new_random(),
        .initial_hp = h * pow(1.2, wave_counter),
        .current_hp = self->initial_hp,
        .last_gem_impact = 0,
        .timer = Timer_new_ms(0),
        .start_timer = start_timer,
    };
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
