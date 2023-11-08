#ifndef UTILS_H
#define UTILS_H

/**
 * @brief clamp a value between a min and a max
 * 
 */
#define clamp(value, min, max) ((value) < (min) ? (min) : (((value) > (max) ? (max) : (value))))

/**
 * @brief Minimum of two values
 * 
 */
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#endif
