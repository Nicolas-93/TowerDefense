#include "arraylist.h"
#include <string.h>
#include <assert.h>

ArrayListError _ArrayList_realloc(ArrayList* self, size_t new_capacity);

ArrayListError ArrayList_init(
    ArrayList* self,
    size_t element_size,
    size_t initial_capacity,
    int (*cmp)(const void*, const void*)
) {
    *self = (ArrayList) {
        .capacity = initial_capacity,
        .element_size = element_size,
        .len = 0,
        .cmp = cmp,
    };

    self->arr = malloc(element_size * initial_capacity);

    if (self->arr == NULL)
        return ARRAYLIST_ERR_ALLOC;

    return ARRAYLIST_ERR_NONE;
}

ArrayListError _ArrayList_realloc(ArrayList* self, size_t new_capacity) {
    uint8_t* new_arr = realloc(self->arr, new_capacity * self->element_size);

    if (new_arr == NULL)
        return ARRAYLIST_ERR_ALLOC;

    self->arr = new_arr;
    self->capacity = new_capacity;

    return ARRAYLIST_ERR_NONE;
}

ArrayListError ArrayList_append(ArrayList* self, void* elem) {
    if (self->len >= self->capacity) {
        ArrayListError err = _ArrayList_realloc(self, self->capacity + ARRAYLIST_REALLOC_BLOCK);

        if (err < 0)
            return err;
    }

    memcpy(self->arr + self->len * self->element_size, (uint8_t*) elem, self->element_size);
    self->len++;

    return ARRAYLIST_ERR_NONE;
}

void* ArrayList_get(const ArrayList* self, int64_t i) {

    if (i < 0)
        i = self->len + i;

    if (i >= self->len)
        return NULL;

    return self->arr + i * self->element_size;
}

void* ArrayList_pop(ArrayList* self) {
    if (self->len == 0)
        return NULL;

    uint8_t* popped = self->arr + (--(self->len)) * self->element_size;

    return popped;
}

void ArrayList_resize(ArrayList* self, size_t new_size) {
    assert(new_size >= 0);
    self->len = new_size;
}

ArrayListError ArrayList_shrink_to_fit(ArrayList* self) {
    return _ArrayList_realloc(self, self->len);
}

void ArrayList_free(ArrayList* self) {
    free(self->arr);
    *self = (ArrayList) {0};
}

void ArrayList_clear(ArrayList* self) {
    self->len = 0;
}

size_t ArrayList_get_length(const ArrayList* self) {
    return self->len;
}

void ArrayList_sort(ArrayList* self) {
    assert(self->cmp);

    qsort(
        self->arr,
        self->len,
        self->element_size,
        self->cmp
    );
}

void* ArrayList_search(ArrayList* self, void* searched_value) {
    assert(self->cmp);

    return bsearch(
        searched_value,
        self->arr,
        self->len,
        self->element_size,
        self->cmp
    );
}
