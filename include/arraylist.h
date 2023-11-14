#include <stdlib.h>
#include <stdint.h>

typedef enum ArrayListError {
    ARRAYLIST_ERR_NONE = 0,
    ARRAYLIST_ERR_ALLOC = -1,
} ArrayListError;

typedef struct ArrayList {
    size_t len;
    size_t capacity;
    size_t element_size;
    int (*cmp)(const void*, const void*);
    uint8_t* arr;
} ArrayList;

#define ARRAYLIST_REALLOC_BLOCK 10

// https://stackoverflow.com/questions/28524896/casting-pointer-to-memory-buffer-to-pointer-to-vla
// int (*arrPointer)[ARRSIZE] = (int(*)[ARRSIZE]) pointer;
#define ARRAYLIST_DECLARE_VLA(self, typename, array_name) \
    typename (*array_name)[self.len] = ((typename(*)[self.len]) (typename*) self.arr)

#define ARRAYLIST_DECLARE_ARRAY(self, typename, array_name) \
    typename* array_name = (typename*) (self).arr

// #define ArrayList_pop_t(self, elem_type) ((elem_type) ArrayList_pop(self))

/**
 * @brief Create a new vector object
 * 
 * @param self ArrayList to initialize
 * @param element_size Size of one element
 * @param initial_capacity Preallocation size
 * @param cmp Comparison function used to sort array if needed,
 * if sorting is not used, set to NULL
 * @return ArrayListError
 * if error while allocation : ARRAYLIST_ERR_ALLOC
 */
ArrayListError ArrayList_init(
    ArrayList* self,
    size_t element_size,
    size_t initial_capacity,
    int (*cmp)(const void*, const void*)
);

/**
 * @brief Add to element end of the vector
 * 
 * @param self ArrayList object
 * @param elem Element to add
 * @return ArrayListError 
 */
ArrayListError ArrayList_append(ArrayList* self, void* elem);

/**
 * @brief Get element's address at index i if positive,
 * len - i, if negative
 * @param self ArrayList object
 * @param i index
 * @return void* 
 */
void* ArrayList_get(const ArrayList* self, int64_t i);

/**
 * @brief Get element's value at index i if positive,
 * len - i, if negative
 * @param self ArrayList object
 * @param i index
 * @param type value's type
 */
#define ArrayList_get_v(self, i, type) (*((type*) ArrayList_get(self, i)))

/**
 * @brief ArrayList pop last element
 * 
 * @param self ArrayList object
 */
void* ArrayList_pop(ArrayList* self);

/**
 * @brief Resize vector
 * old_size <= new_size : Nothing
 * old_size > new_size : Removes elements to fit
 * 
 * @param self 
 * @param new_size new size
 */
void ArrayList_resize(ArrayList* self, size_t new_size);

/**
 * @brief Reduce memory usage by reducing capacity
 * to vector's length
 * 
 * @param self 
 */
ArrayListError ArrayList_shrink_to_fit(ArrayList* self);

/**
 * @brief Free ArrayList object
 * 
 * @param self 
 */
void ArrayList_free(ArrayList* self);

/**
 * @brief Clear ArrayList object
 * 
 * @param self 
 */
void ArrayList_clear(ArrayList* self);

/**
 * @brief Get ArrayList's length
 * 
 * @param self 
 * @return size_t 
 */
size_t ArrayList_get_length(const ArrayList* self);

/**
 * @brief Sort's vector
 * 
 * @param self 
 */
void ArrayList_sort(ArrayList* self);

/**
 * @brief Search value in vector
 * 
 * @param self 
 * @param searched_value 
 * @return void* 
 */
void* ArrayList_search(ArrayList* self, void* searched_value);
