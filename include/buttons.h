#ifndef BUTTONS_H
#define BUTTONS_H

#include "grid.h"
#include "error.h"
#include "image.h"
#include "geom.h"

typedef struct Button {
    ImageName icon;
    struct {
        void (*func)(void* data);
        void* context;
    } callback;
} Button;

typedef struct Buttons {
    Grid grid;
    Button* buttons;
    int nb_buttons;
} Buttons;

/**
 * @brief Initialize a Buttons object
 * 
 * @param self Buttons object
 * @param parent Parent grid
 * @param rect Position of the Buttons in the parent grid
 * @param buttons Pre-allocated array of buttons
 * @param nb_buttons Number of buttons
 * @return Error 
 */
Error Buttons_new(Buttons* self, Grid* parent, Rect rect, Size grid_size, Button buttons[], int nb_buttons);

/**
 * @brief Draw the Buttons
 * 
 * @param self Buttons object
 */
void Buttons_draw(const Buttons* self);

/**
 * @brief Call the callback of the clicked button
 * 
 * @param self 
 */
void Buttons_process_event(Buttons* self);

#endif
