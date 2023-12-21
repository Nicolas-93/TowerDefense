#include "dragndrop.h"
#include "event.h"
#include <assert.h>

static DragNDrop DRAGGED;

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
) {
    DRAGGED = (DragNDrop) {
        .object = object,
        .draw = draw,
        .context_on_release = context_on_release,
        .on_release = on_release,
        .context_on_release_failure = context_on_release_failure,
        .on_release_failure = on_release_failure,
        .is_dragging = true,
        .origin = Event_get().mouse
    };

    return 0;
}

void DragNDrop_draw(void) {
    static Point last_pos = {0, 0};

    if (Event_get().mouse.x > 0 && Event_get().mouse.y > 0) {
        last_pos = Event_get().mouse;
    }

    if (DRAGGED.is_dragging) {
        DRAGGED.draw(
            DRAGGED.object,
            last_pos
        );
    }
}

void DragNDrop_process_event(void) {
    if (DRAGGED.is_dragging && Event_is_released_click()) {
        if (!DRAGGED.on_release(
            DRAGGED.context_on_release, DRAGGED.object,
            Event_get().mouse
        )) {
            DRAGGED.on_release_failure(
                DRAGGED.context_on_release_failure, DRAGGED.object,
                DRAGGED.origin
            );
        }

        DRAGGED = (DragNDrop) {0};
    }
}
