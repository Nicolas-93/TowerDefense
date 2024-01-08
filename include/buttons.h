#ifndef BUTTONS_H
#define BUTTONS_H

#include "grid.h"
#include "error.h"
#include "image.h"
#include "geom.h"

typedef struct Button {
    ImageName icon;
    struct {
        void (*on_click)(void* data); /*< callled on button click */
        void (*on_hover)(Point pos, void* data); /*< called on hovering, should be
        used for drawing overlays, ``pos`` represents cell's absolute position */
        void* context; /*< context passed to the callbacks */
    } callback;
    bool _is_hovered;
} Button;

typedef struct Buttons {
    Grid grid; /*< grid containing the buttons */
    Button* buttons; /*< Pointer to the array of buttons statically allocated */
    int nb_buttons; /*< Number of buttons */
} Buttons;

/**
 * @brief Initialize a Buttons object
 * Each button have to be created in buttons array.
 * A button is a struct containing an icon and two callbacks with a context.
 * The first callback is called when the button is clicked.
 * The second callback is called when the button is hovered,
 * it is called by Buttons_draw, and should be used to draw overlays.
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
 * @brief Draw the Buttons, and call the on_hover callback of the hovered button
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
