#ifndef INVENTORY_H
#define INVENTORY_H

#include "gem.h"

typedef enum ItemType {
    ITEM_NONE = 0,
    ITEM_GEM,
} ItemType;

typedef struct Item {
    ItemType type;
    union {
        Gem gem;
    };
} Item;

typedef struct Inventory {
    int width;
    int height;
    Item* items;
} Inventory;

/**
 * @brief Create a new Inventory object
 * 
 * @param self  
 * @param width 
 * @param height 
 */
void Inventory_new(Inventory* self, uint16_t width, uint16_t height);

/**
 * @brief Add an item to the inventory
 * 
 * @param self 
 * @param item 
 */
void Inventory_add(Inventory* self, Item item, uint16_t x, uint16_t y);

/**
 * @brief Remove an item from the inventory
 * 
 * @param self 
 * @param x 
 * @param y 
 */
void Inventory_remove(Inventory* self, uint16_t x, uint16_t y);

/**
 * @brief Get the item at the given position
 * 
 * @param self 
 * @param x 
 * @param y 
 * @return Item 
 */
Item Inventory_get(Inventory* self, uint16_t x, uint16_t y);

#endif
