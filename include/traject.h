#ifndef TRAJECT_H
#define TRAJECT_H

#include "geom.h"

typedef struct Traject {
    Point pos;
    Vector2D dir;
    float speed;
} Traject;

/**
 * @brief Create a new Traject object
 * 
 * @param pos Object position
 * @param dir Object direction
 * @return Traject 
 */
Traject Traject_new(Point pos, Vector2D dir);

/**
 * @brief Create a new Traject object from two points
 * A vector AB is created, and the direction is normalized.
 * @param a Point A
 * @param b Point B
 * @param speed Object speed
 * @return Traject 
 */
Traject Traject_new_from_points(Point a, Point b, float speed);

/**
 * @brief Update object's position
 * 
 * @param self 
 */
void Traject_move(Traject* self);

#endif 
