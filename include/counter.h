#ifndef COUNTER_H
#define COUNTER_H

#include "error.h"
#include "grid.h"

typedef struct Counter {
    int min;
    int max;
    int value;
    Grid grid;
} Counter;

/**
 * @brief Create a new Counter object
 * 
 * @param self Counter object to initialize
 * @param parent Parent grid
 * @param rect Position of the counter in the grid
 * @param min Minimum value
 * @param max Maximum value
 * @return Error 
 */
Error Counter_new(Counter *self, Grid* parent, Rect rect, int min, int max);

/**
 * @brief Free a Counter object
 * 
 * @param self 
 */
void Counter_free(Counter *self);

/**
 * @brief Draw the counter
 * 
 * @param self 
 */
void Counter_draw(const Counter *self);

/**
 * @brief Process an event (update value on click)
 * 
 * @param self 
 */
void Counter_process_event(Counter* self);

#endif
