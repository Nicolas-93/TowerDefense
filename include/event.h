#ifndef EVENT_H
#define EVENT_H

#include <MLV/MLV_all.h>
#include "geom.h"

/**
 * @struct Event
 * @brief Evènement souris
 */
typedef struct _Event {
    Point mouse;                    /**< Mouse position */
    MLV_Event type;                 /**< Event type (MLV_KEY, MLV_MOUSE_BUTTON, MLV_MOUSE_MOTION) */
    MLV_Mouse_button mouse_btn;     /**< Click type : Left, Right, Middle */
    MLV_Keyboard_button kb_btn;     /**< Pressed key */
    MLV_Button_state state;         /**< MLV_PRESSED / MLV_RELEASED */
} Event;

/**
 * @brief Check if it's a mouse click event
 * 
 * @return true 
 * @return false 
 */
bool Event_is_pressed_click(void);

/**
 * @brief Check if it's a mouse release event
 * 
 * @return true 
 * @return false 
 */
bool Event_is_released_click(void);

/**
 * @brief Pop an event from the event queue and
 * store it in event module's static variable
 * Should be called in mainloop
 * 
 */
void Event_pop_event(void);

/**
 * @brief Get last popped event
 * 
 * @return Event 
 */
Event Event_get(void);

#endif
