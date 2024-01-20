#include "mana.h"

bool Mana_new(Mana* self, uint32_t initial_mana, uint32_t max_mana) {
    *self = (Mana) {
        .level = 0,
        .mana = initial_mana,
        .max_mana = max_mana
    };
    return self->mana > 0;
}

ManaError Mana_add(Mana* self, int amount) {
    self->mana += amount;
    if (self->mana == 0)
        return MANA_BANKRUPT;

    if (self->mana > self->max_mana) {
        self->mana = self->max_mana;
        return MANA_CAPACITY_REACHED;
    }

    return true;
}

bool Mana_have_sufficient_mana(const Mana* self, uint32_t amount) {
    return self->mana >= amount;
}

bool Mana_upgrade(Mana* self) {
    uint32_t price = 500 * pow(1.4, self->level);
    if (Mana_have_sufficient_mana(self, price)) {
        self->level++;
        self->max_mana = 2000 * pow(1.4, self->level);
        return Mana_add(self, -price);
    }
    return false;
}
