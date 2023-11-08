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


typedef enum DequeError {
    ERR_DEQUE_ALLOC = -1,
} DequeError;

#define DEQUE_FOREACH(var, deque) \
    for ((var) = ((deque)->first); (var); (var) = ((var)->_next))

#define DEQUE_FOREACH_REVERSE(var, deque) \
    for ((var) = ((deque)->last); (var); (var) = ((var)->_prev))

#define DEQUE_FOREACH_SAFE(var, deque, tmpvar) \
    for ((var) = ((deque)->first); (var) && (((tmpvar) = (var)->_next), 1); (var) = (tmpvar))

#define DEQUE_FOREACH_REVRESE_SAFE(var, deque, tmpvar) \
    for ((var) = ((deque)->last) ; (var) && (((tmpvar) = (var)->_prev), 1); (var) = (tmpvar))

/**
 * @brief Initialize a new deque
 * 
 * @param self Deque object to initialize
 * @param elem_size Stored elements sizes
 * @return DequeError 
 */
DequeError Deque_init(Deque* self, size_t elem_size);

/**
 * @brief Add element on the right end
 * 
 * @param self Deque object
 * @param elem Element's address
 * @return DequeError 
 */
DequeError Deque_append(Deque* self, const void* elem);

/**
 * @brief Add element on the left end
 * 
 * @param self Deque object
 * @param elem Element's address
 * @return DequeError 
 */
DequeError Deque_appendleft(Deque* self, const void* elem);

/**
 * @brief Add element before 
 * 
 * @param self Deque object
 * @param listnode Deque's node
 * @param elem Element's address
 * @return DequeError 
 */
DequeError Deque_add_before(Deque* self, DequeNode* listnode, const void* elem);

/**
 * @brief Add element after
 * 
 * @param self Deque object
 * @param listnode Deque's node
 * @param elem Element's address
 * @return DequeError 
 */
DequeError Deque_add_after(Deque *self, DequeNode* listnode, const void *elem);

/**
 * @brief Get element's address from node
 * 
 * @param node Deque's node
 * @return void* 
 */
void* Deque_get_elem(DequeNode* node);

/**
 * @brief Get element from node
 * 
 * @param node Deque's node
 * @param type Stored type
 */
#define Deque_get_elem_v(node, type) \
    (*((type*) (Deque_get_elem(node))))

/**
 * @brief Get element's address at index i if positive,
 * len - i, if negative
 * 
 * @param self Deque object
 * @param i index
 * @return void* 
 */
void* Deque_get_index(const Deque* self, int64_t i);

/**
 * @brief Get element at index i if positive,
 * len - i, if negative
 * 
 * @param self Deque object
 * @param i index
 * @param type Stored type
 */
#define Deque_get_index_v(self, i, type) \
    (*((type*) (Deque_get_index(self, i))))

/**
 * @brief Get last element address
 * 
 * @param self 
 * @return void* 
 */
void* Deque_get_last_elem(const Deque* self);

/**
 * @brief Get last element address
 * 
 * @param self Deque object
 * @param type Stored type
 */
#define Deque_get_last_elem_v(self, type) \
    (*((type*) (Deque_get_last_elem(self))))

/**
 * @brief Get first element address
 * 
 * @param self 
 * @return void* 
 */
void* Deque_get_first_elem(const Deque* self);

/**
 * @brief Get first element address
 * 
 * @param self Deque object
 * @param type Stored type
 */
#define Deque_get_first_elem_v(self, type) \
    (*((type*) (Deque_get_first_elem(self))))

/**
 * @brief Get deque's node address at index i if positive,
 * len - i, if negative
 * 
 * @param self Deque object
 * @param i index
 * @return DequeNode* 
 */
DequeNode* Deque_get_index_node(const Deque* self, int64_t i);

/**
 * @brief Get last node
 * 
 * @param self 
 * @return DequeNode* 
 */
DequeNode* Deque_get_last_node(const Deque* self);

/**
 * @brief Get first node
 * 
 * @param self 
 * @return DequeNode* 
 */
DequeNode* Deque_get_first_node(const Deque* self);

/**
 * @brief Remove node from deque
 * and free's node afterwards
 * 
 * @param node Deque's node 
 */
void Deque_remove(Deque* self, DequeNode* node);

/**
 * @brief Pop deque's right element
 * 
 * @param self Deque object
 * @param dest Address to store popped value, if NULL,
 * only remove element
 */
DequeError Deque_pop(Deque* self, void* dest);

/**
 * @brief Pop deque's left element
 * 
 * @param self Deque object
 * @param dest Address to store popped value, if NULL,
 * only remove element
 */
DequeError Deque_popleft(Deque* self, void* dest);

/**
 * @brief Move elems of other deque onto
 * self's end
 * 
 * @param self Deque object getting from other
 * @param other Deque object being empty
 */
void Deque_concat(Deque* self, Deque* other);

/**
 * @brief Get deque's length
 * 
 * @param self Deque object
 * @return size_t 
 */
size_t Deque_get_length(const Deque* self);

/**
 * @brief Return true if the deque is empty
 * 
 * @param self Deque object
 * @return true 
 * @return false 
 */
bool Deque_is_empty(const Deque* self);

/**
 * @brief Free all nodes
 * 
 * @param self Deque object
 */
void Deque_free(Deque* self);

#endif
