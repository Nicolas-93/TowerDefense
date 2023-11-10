#ifndef PATH_H
#define PATH_H

#include <stdint.h>
#include <stdbool.h>
#include "arraylist.h"
#include "geom.h"

typedef enum PathCase {
    PATH_CASE_EMPTY,
    PATH_CASE_START,
    PATH_CASE_END,
    PATH_CASE
} PathCase;

typedef struct Path {
    PathCase arr2d[22][28];
    uint16_t width;         /*< 28 by default */
    uint16_t height;        /*< 22 by default */
    ArrayList waypoints;    /*< Waypoints of the path */
} Path;

/**
 * @brief Create a new Path object
 * 
 * @param self 
 */
void Path_new(Path* self);

/**
 * @brief Get the start point of the path
 * 
 * @param self 
 * @return Point 
 */
Point Path_get_start(const Path* self);

/**
 * @brief Get the end point of the path
 * 
 * @param self 
 * @return Point 
 */
Point Path_get_end(const Path* self);

/**
 * @brief Print path
 * 
 * @param self 
 */
void Path_print(Path* self);

#endif
