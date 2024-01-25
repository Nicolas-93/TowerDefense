#ifndef INVENTORY_H
#define INVENTORY_H

#include "gem.h"
#include "grid.h"
#include "geom.h"
#include "mana.h"
#include <float.h>

typedef struct Inventory {
    int width;
    int height;
    Gem** gems;
    size_t nb_gems;
    Grid grid;
    void* game;     /*< Parent game context, used for dragndrop */
    Mana* mana;     /*< Mana object */
} Inventory;

/**
 * @brief Create a new Inventory object
 * 
 * @param self Inventory object to initialize
 * @param parent Parent grid
 * @param rect Position of the inventory in the parent grid
 * @param size Width and height (should be 3x12)
 * @param mana Mana object (merge gems)
 * @param game Parent game context (used for dragndrop)
 */
Error Inventory_new(Inventory* self, Grid* parent, Rect rect, Size size, Mana* mana, void* game);

/**
 * @brief Destroy the Inventory object
 * 
 * @param self Inventory object to destroy
 */
void Inventory_free(Inventory* self);

/**
 * @brief Put a gem to the inventory
 * 
 * @param self Inventory object
 * @param gem Gem to add
 * @param pos Position of the gem in the inventory
 * @return true if the gem was added successfully, false otherwise
 * (if the position is already occupied) or if the inventory is full
 */
bool Inventory_put(Inventory* self, Gem* gem, Point pos);

/**
 * @brief Put a gem to a random position in the inventory
 * 
 * @param self Inventory object
 * @param gem Gem to add
 * @return true if the gem was added successfully
 * @return false if the inventory is full
 */
bool Inventory_put_random(Inventory* self, Gem* gem);

/**
 * @brief Pop an item from the inventory
 * 
 * @param self 
 * 
 */
Gem* Inventory_pop(Inventory* self, Point pos);

/**
 * @brief Get the item at the given position
 * 
 * @param self Inventory object
 * @param pos Position of the item
 * @return Item 
 */
Gem* Inventory_get(const Inventory* self, Point pos);

/**
 * @brief Draw the inventory
 * 
 * @param self Inventory object
 */
void Inventory_draw(const Inventory* self);

/**
 * @brief Process user events on the inventory
 * 
 * @param self 
 */
void Inventory_process_event(Inventory* self);

/**
 * @brief Callback called when a gem is released on the inventory
 * 
 * @param context Inventory object
 * @param object Gem object
 * @param abs_pos Release position
 * @return true Gem was added successfully
 * @return false Gem was not added
 */
bool Inventory_on_gem_release(void* context, void* object, Point abs_pos);

/**
 * @brief Check if the inventory is full
 * 
 * @param self 
 * @return true 
 * @return false 
 */
bool Inventory_is_full(const Inventory* self);

#endif
