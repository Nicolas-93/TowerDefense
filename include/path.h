#ifndef PATH_H
#define PATH_H

#include <stdint.h>
#include "arraylist.h"

typedef enum PathCase {
    PATH_CASE_EMPTY,
    PATH_CASE_START,
    PATH_CASE_END,
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
 * @brief Generate a path in the land
 * 
 * @param self 
 */
static void _Path_generate_path(Path* self);

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

#endif
