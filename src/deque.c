#include "deque.h"
#include <string.h>
#include <assert.h>

static DequeNode* _Deque_new_node(const Deque* self, const void* elem) {
    assert(self);
    assert(elem);

    DequeNode* node = NULL;

    if (!(node = malloc(sizeof(DequeNode) + self->elem_size))) {
        return NULL;
    }

    memcpy(node->elem, elem, self->elem_size);

    return node;
}

DequeError Deque_init(Deque* self, size_t elem_size) {
    assert(self);

    *self = (Deque) {
        .elem_size = elem_size,
        .len = 0,
        .first = NULL,
        .last = NULL,
    };

    return 0;
}

DequeError Deque_append(Deque* self, const void* elem) {
    assert(self);
    assert(elem);

    DequeNode* new = _Deque_new_node(self, elem);

    if (!new) {
        return ERR_DEQUE_ALLOC;
    }

    new->_next = NULL;
    new->_prev = self->last;

    /* Si la liste est vide, le premier élément
    est le nouvel élément */
    if (self->first == NULL) {
        self->first = new;
    }
    else { /* On chaine le nouvel élement au dernier */
        self->last->_next = new;
    }
    self->last = new;

    self->len++;

    return 0;
}

DequeError Deque_appendleft(Deque* self, const void* elem) {
    assert(self);
    assert(elem);

    DequeNode* new = _Deque_new_node(self, elem);
    
    if (!new) {
        return ERR_DEQUE_ALLOC;
    }

    new->_next = self->first;
    new->_prev = NULL;

    if (self->first == NULL) {
        self->last = new;
    }
    else {
        self->first->_prev = new;
    }
    self->first = new;

    self->len++;

    return 0;
}

DequeError Deque_add_before(Deque* self, DequeNode* listnode, const void* elem) {
    assert(self);
    assert(listnode);
    assert(elem);

    assert(!Deque_is_empty(self));

    DequeNode* new = _Deque_new_node(self, elem);
    
    if (!new) {
        return ERR_DEQUE_ALLOC;
    }

    new->_next = listnode;
    new->_prev = listnode->_prev;

    listnode->_prev->_next = new;
    listnode->_prev = new;

    if (new->_prev == NULL) {
        self->first = new;
    }

    self->len++;

    return 0;
}

DequeError Deque_add_after(Deque *self, DequeNode* listnode, const void *elem) {
    assert(self);
    assert(listnode);
    assert(elem);

    assert(!Deque_is_empty(self));
    
    DequeNode* new = _Deque_new_node(self, elem);
    
    if (!new) {
        return ERR_DEQUE_ALLOC;
    }

    new->_next = listnode->_next;
    new->_prev = listnode;

    listnode->_next->_prev = new;
    listnode->_next = new;

    if (new->_next == NULL) {
        self->last = new;
    }

    self->len++;

    return 0;
}

void* Deque_get_elem(DequeNode* node) {
    assert(node);

    return node->elem;
}

void* Deque_get_index(const Deque* self, int64_t index) {
    DequeNode* node = Deque_get_index_node(self, index);

    if (!node)
        return NULL;

    return node->elem;
}

void* Deque_get_first_elem(const Deque* self) {
    return self->first->elem;
}

void* Deque_get_last_elem(const Deque* self) {
    return self->last->elem;
}

DequeNode* Deque_get_index_node(const Deque* self, int64_t index) {
    assert(self);
    assert(!Deque_is_empty(self) && "deque: indexing on empty deque");
    
    if (index < 0)
        index = self->len + index;

    assert(index < self->len && "deque: index out of range");

    DequeNode* node = NULL;
    int i;

    if (index < self->len / 2) {
        for (i = 0, node = self->first; i != index; node = node->_next, ++i);
    }
    else {
        for (i = self->len - 1, node = self->last; i != index; node = node->_prev, --i);
    }

    return node;
}

DequeNode* Deque_get_first_node(const Deque* self) {
    assert(self);

    return self->first;
}
DequeNode* Deque_get_last_node(const Deque* self) {
    assert(self);

    return self->last;
}

void Deque_remove(Deque* self, DequeNode* node) {
    assert(self);
    assert(node);
    assert(!Deque_is_empty(self));

    if (node->_next) {
        node->_next->_prev = node->_prev;
    }
    else {
        self->last = node->_prev;
    }
    
    if (node->_prev) {
        node->_prev->_next = node->_next;
    }
    else {
        self->first = node->_next;
    }

    self->len--;

    free(node);
}

DequeError Deque_pop(Deque* self, void* dest) {
    assert(self);
    assert(!Deque_is_empty(self));

    DequeNode* popped = Deque_get_last_node(self);
    
    if (dest != NULL) {
        memcpy(dest, popped->elem, self->elem_size);
    }
    
    Deque_remove(self, popped);

    return 0;
}

DequeError Deque_popleft(Deque* self, void* dest) {
    assert(self);
    assert(!Deque_is_empty(self));

    DequeNode* popped = Deque_get_first_node(self);

    if (dest != NULL) {
        memcpy(dest, popped->elem, self->elem_size);
    }

    Deque_remove(self, popped);

    return 0;
}

void Deque_concat(Deque* self, Deque* other) {
    assert(self);
    assert(other);

    if (Deque_is_empty(self)) {
        self->first = other->first;
    }
    else {
        self->last->_next = other->first;
    }
    self->last = other->last;
    
    self->len += other->len;

    Deque_init(other, other->elem_size);
}

size_t Deque_get_length(const Deque* self) {
    assert(self);

    return self->len;
}

bool Deque_is_empty(const Deque* self) {
    assert(self);

    if (self->first == NULL || self->last == NULL || self->len == 0) {
        assert(self->first == NULL && self->last == NULL && self->len == 0);
    }

    return self->first == NULL;
}

void Deque_free(Deque *self) {
    assert(self);

    DequeNode *node, *node2;
    node = Deque_get_first_node(self);

    while (node != NULL) {
        node2 = node->_next;
        free(node);
        node = node2;
    }

    Deque_init(self, self->elem_size);
}
