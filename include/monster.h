#ifndef MONSTER_H
#define MONSTER_H

#include <stdint.h>
#include "deque.h"
#include "shot.h"
#include "traject.h"
#include "color.h"
#include "timer.h"
#include "path.h"
#include "grid.h"
#include "effects.h"

typedef struct Monster {
    Traject traj;               /*< Trajectory */
    Path* path;                 /*< Path */
    Grid* grid;                 /*< Land's grid */
    uint16_t target_waypoint;   /*< Next waypoint compared to path's list */
    Color color;                /*< Color */
    int32_t initial_hp;         /*< Initial HP */
    int32_t current_hp;         /*< Current HP */
    GemType last_gem_impact;    /*< Last gem that impacted the monster */
    Timer timer;                /*< Timer for gems effects */
    Timer start_timer;          /*< Timer to wait before starting */
    Deque future_shots;         /*< Shots he will sufffer */
    EffectUnion effect;         /*< Effect applied to the monster */
} Monster;

/**
 * @brief Create a new Monster object
 * 
 * @param self 
 * @param grid Grid of the land
 * @param path Path followed by the monster
 * @param h Difficulty of the monster
 * @param speed Speed of the monster
 * used to compute its initial HP.
 * Formula: h * (1.2)^i, where i is the i-th monster wave (wave counter).
 * @param wave_counter Wave counter
 * @param start_timer Timer to wait before the monster exits the spawn
 */
void Monster_new(
    Monster* self,
    Grid* grid,
    Path* path,
    const double h,
    float speed,
    uint16_t wave_counter,
    Timer start_timer
);

/**
 * @brief Add a shot that will hit the monster in the future.
 * 
 * @param self Monster object
 * @param shot Shot object to add
 * @return Error 
 */
Error Monster_add_future_shot(Monster* self, const Shot* shot);

/**
 * @brief Update the monster's and shots positions.
 * 
 * @param self Monster object
 * @return true Monster is dead
 * @return Error
 * INFO_MONSTER_BACK_TO_SPAWN if the monster has reached the end of the path
 * and is back to spawn.
 * INFO_MONSTER_IS_DEAD if the monster a shot has reached him
 * and he died.
 */
Error Monster_update(Monster* self);

/**
 * @brief Draw the monster.
 * 
 * @param self 
 */
void Monster_draw(const Monster* self);

/**
 * @brief Free the monster.
 * 
 * @param self 
 */
void Monster_free(Monster* self);

/**
 * @brief Get the monster's position.
 * 
 * @param self 
 * @return Point 
 */
Point Monster_get_pos(const Monster* self);

/**
 * @brief Set the monster's speed.
 * 
 * @param self 
 * @param speed 
 */
void Monster_set_speed(Monster* self, double speed);

/**
 * @brief Apply damages to the monster.
 * 
 * @param self 
 * @param damage 
 * @return true Monster is dead
 * @return false Monster is still alive
 */
Error Monster_apply_damage(Monster* self, double damage);

/**
 * @brief Get the monster's HP.
 * 
 * @param self Monster object
 * @return int32_t Monster's HP
 */
int Monster_get_hp(const Monster* self);

/**
 * @brief Check if the monster is dead.
 * 
 * @param self Monster object
 * @return true is dead
 * @return false is still alive
 */
bool Monster_is_dead(const Monster* self);

#endif
