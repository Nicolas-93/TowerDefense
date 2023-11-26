#ifndef TRAJECT_H
#define TRAJECT_H

#include "geom.h"

typedef struct Traject {
    Point pos;              /*< Object position */
    Vector2D dir;           /*< Object direction */
    float base_unit;        /*< Number of pixels by second, usually a cell width */
    float speed_by_unit;    /*< Object speed */
    float speed_by_frame;   /*< Object speed */
} Traject;

/**
 * @brief Create a new Traject object
 * 
 * @param pos Object position
 * @param dir Object direction
 * @param base_unit Number of pixels by second, usually a cell width
 * @return Traject 
 */
Traject Traject_new(Point pos, Vector2D dir, float base_unit);

/**
 * @brief Create a new Traject object without direction and position
 * Direction is set to (0, 0), and should be set later.
 * 
 * @param base_unit Number of pixels by second, usually a cell width
 * @param speed_by_unit Object speed by unit
 * @return Traject 
 */
Traject Traject_new_without_dir_and_pos(float base_unit, float speed_by_unit);

/**
 * @brief Create a new Traject object from two points
 * A vector AB is created, and the direction is normalized.
 * @param a Point A
 * @param b Point B
 * @param base_unit Number of pixels by second, usually a cell width
 * @param speed Object speed
 * @return Traject 
 */
Traject Traject_new_from_points(Point a, Point b, float base_unit, float speed);

/**
 * @brief Update object's position
 * 
 * @param self 
 */
void Traject_move(Traject* self);

/**
 * @brief Update object's direction to reach a target
 * 
 * @param self 
 * @param target Target point
 */
void Traject_pursuit(Traject* self, Point target);

/**
 * @brief Like Traject_pursuit, but ensure that the object
 * start at ``a``.
 * 
 * @param self
 * @param a Start point
 * @param b Target point
 */
void Traject_set_from_points(Traject* self, Point a, Point b);

/**
 * @brief Check if the object has reached a target
 * 
 * @param self 
 * @param target 
 * @return true 
 * @return false 
 */
bool Traject_is_over(const Traject* self, Point target);

/**
 * @brief Speed in units per second
 * 
 * @param self 
 * @param speed 
 */
void Traject_set_speed(Traject* self, float speed);

/**
 * @brief Get the speed in units per second
 * 
 * @param self 
 * @return float 
 */
float Traject_get_speed(const Traject* self);

/**
 * @brief Get unit speed
 * 
 * @param self 
 * @return float 
 */
float Traject_get_base_unit(const Traject* self);

#endif 
