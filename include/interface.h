#ifndef INTERFACE_H
#define INTERFACE_H

#include "grid.h"
#include "error.h"

typedef struct Interface {
    Grid grid;
} Interface;

/**
 * @brief Create a new Interface object
 * 
 * @param self 
 * @return Error 
 */
Error Interface_new(Interface* self);

/**
 * @brief Draw the interface
 * 
 * @param self 
 */
void Interface_draw(Interface* self);

#endif
