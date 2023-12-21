#ifndef DRAGNDROP_H
#define DRAGNDROP_H

#include "geom.h"

typedef struct DragNDrop {
    Point origin;   /*< Dragging start point, memoized to call ``on_release_failure`` */
    void* object;   /*< Object being moved */
    void (*draw)(void* object, Point pos); /*< Draw function to call while dragging */
    void* context_on_release;   /*< Data to pass to ``on_release`` function */
    bool (*on_release)(
        void* context_on_release, void* object,
        Point pos
    );
    void* context_on_release_failure; /*< Data to pass to ``on_release_failure`` function */
    void (*on_release_failure)(
        void* context_on_release_failure, void* object,
        Point origin
    ); /*< Called if ``on_release`` function hasn't accepted the object */
    bool is_dragging;
} DragNDrop;

/**
 * @brief Put an object on cursor and start dragndrop operation
 * 
 * @param object Object moved by the user
 * @param draw Function to call to draw the object being moved
 * @param context_on_release Data to pass to ``on_release``
 * @param on_release Callback function triggered when the user released the object
 * It should return true if the object was accepted, false otherwise.
 * @param context_on_release_failure Data to pass to ``on_release_failure``
 * @param on_release_failure Callback function called if ``on_release`` rejected the object.
 * Functionnaly the same as ``on_release`` but give the ``origin`` location of the object.
 * @return int 
 */
int DragNDrop_put(
    void* object,
    void (*draw)(void* object, Point pos),
    void* context_on_release,
    bool (*on_release)(
        void* context_on_release, void* object,
        Point pos
    ),
    void* context_on_release_failure,
    void (*on_release_failure)(
        void* context_on_release_failure, void* object,
        Point origin
    )
);

/**
 * @brief Draw moving object if any
 * 
 */
void DragNDrop_draw(void);

/**
 * @brief Process release event
 * 
 */
void DragNDrop_process_event(void);

#endif
