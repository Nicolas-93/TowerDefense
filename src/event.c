#include "event.h"

static Event EV = {0};

void Event_pop_event(void) {
    Event ev;
    int x = 0, y = 0;

    ev.type = MLV_get_event(
        &ev.kb_btn, NULL, NULL,
        NULL, NULL,
        &x, &y,
        &ev.mouse_btn, &ev.state
    );
    ev.mouse = (Point) {.x = x, .y = y};

    EV = ev;
}

Event Event_get(void) {
    return EV;
}

bool Event_is_click(void) {
    return EV.type == MLV_MOUSE_BUTTON && EV.state == MLV_PRESSED;
}
