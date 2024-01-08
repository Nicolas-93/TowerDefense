#include "event.h"

static Event EV = {0};

void Event_pop_event(void) {
    Event ev;
    int x = 0, y = 0;

    ev.type = MLV_get_event(
        &ev.kb_btn, NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        &ev.mouse_btn, &ev.state
    );
    MLV_get_mouse_position(&x, &y);
    ev.mouse = (Point) {.x = x, .y = y};

    EV = ev;
}

Event Event_get(void) {
    return EV;
}

bool Event_is_pressed_click(void) {
    Event ev = Event_get();
    return ev.type == MLV_MOUSE_BUTTON && ev.state == MLV_PRESSED;
}

bool Event_is_released_click(void) {
    Event ev = Event_get();
    return ev.type == MLV_MOUSE_BUTTON && ev.state == MLV_RELEASED;
}