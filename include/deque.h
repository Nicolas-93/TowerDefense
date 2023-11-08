#ifndef DEQUE_H
#define DEQUE_H

#include <stdint.h>
#include <stdlib.h>

typedef struct DequeNode {
    struct DequeNode* _prev;
    struct DequeNode* _next;
    uint8_t elem[]; // Flexible Array Member
} DequeNode;

typedef struct Deque {
    size_t len;
    size_t elem_size;
    DequeNode* first;
    DequeNode* last;
} Deque;

#endif
