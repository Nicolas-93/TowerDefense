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
    const int h,
    float speed,
    uint16_t wave_counter,
    Timer start_timer
);

/**
 * @brief Update the monster's and shots positions.
 * 
 * @param self Monster object
 * @return true Monster is dead
 * @return false Monster is still alive
 */
bool Monster_anim(Monster* self);

/**
 * @brief Draw the monster.
 * 
 * @param self 
 */
void Monster_draw(const Monster* self);

#endif
