#ifndef EFFECTS_H
#define EFFECTS_H

#include "timer.h"
#include "gem.h"
#include "deque.h"

typedef enum EffectType {
    EFFECT_NONE = 0,
    EFFECT_PYRO = 1,
    EFFECT_DENDRO,
    EFFECT_HYDRO,
    EFFECT_PYRO_HYDRO,
    EFFECT_PYRO_DENDRO,
    EFFECT_DENDRO_HYDRO,
} EffectType;


typedef struct Monster Monster; // Forward declaration to avoid circular dependency
typedef struct Effect {
    /* Virtual Table */
    void (*update)(
        struct Effect* self, DequeNode* monster, Deque* monsters
    );
    double base_damage;
    double base_speed;
    EffectType type;
} Effect;

typedef struct PyroEffect {
    Effect base;
} PyroEffect;

typedef struct DendroEffect {
    Effect base;
    Timer next_damage;
    Timer end;
} DendroEffect;

typedef struct HydroEffect {
    Effect base;
    Timer end;
} HydroEffect;

typedef struct PyroHydroEffect {
    Effect base;
    Timer end;
} PyroHydroEffect;

typedef struct PyroDendroEffect {
    Effect base;
} PyroDendroEffect;

typedef struct DendroHydroEffect {
    Effect base;
    Timer end;
} DendroHydroEffect;

typedef union EffectUnion {
    PyroEffect pe;
    HydroEffect he;
    DendroEffect de;
    PyroHydroEffect phe;
    PyroDendroEffect pde;
    DendroHydroEffect dhe;
} EffectUnion;

/**
 * @brief Update the effect
 * 
 * @param effect_u EffectUnion object
 * @param monsters All monsters of the land
 */
void Effect_update(DequeNode* monster, Deque* monsters);

/**
 * @brief Create a new Effect object
 * 
 * @param effect_u EffectUnion object
 * @param base_damage Base damage of the shot
 * @param base_speed Initial speed of the monster
 * @param old_shot Type of the gem dust
 * @param new_shot Type of the newer applied shot
 */
void Effect_new(EffectUnion* effect_u, double base_damage, double base_speed, GemType old_shot, GemType new_shot);

#endif
