#include "effects.h"
#include "monster.h"
#include "gem.h"


void Effect_update(DequeNode* monster, Deque* monsters) {
    Effect* effect = (Effect*) &((Monster*) Deque_get_elem(monster))->effect;
    if (effect->type == EFFECT_NONE) {
        return;
    }
    effect->update(effect, monster, monsters);
}

void Effect_reset(Effect* effect, Monster* monster) {
    Monster_set_speed(monster, effect->base_speed);
    *effect = (Effect) {0};
}

static void _apply_damage(int damage, DequeNode* monster, Deque* monsters) {
    if (Monster_apply_damage(Deque_get_elem(monster), damage) == INFO_MONSTER_IS_DEAD) {
        Deque_remove(monsters, monster);
    }
}

/**
 * @brief Apply damage to all neighbours monsters.
 * 
 * @param monster Monster that applies the damage.
 * @param monsters All monsters on the land.
 * @param range Range of the damage.
 * @param damage Damage to apply.
 * @param percent If true, damage is a percent of the neighbour's hp.
 */
static void _apply_neighbours_damage(
    const Monster* monster, Deque* monsters,
    double range, double damage, bool percent
) {
    DequeNode* node;
    DequeNode* tmp;
    DEQUE_FOREACH_SAFE(node, monsters, tmp) {
        Monster* neighbour = Deque_get_elem(node);
        if (neighbour == monster) {
            continue;
        }
        if (Vector2D_dist(Monster_get_pos(monster), Monster_get_pos(neighbour)) <= range) {
            int value = percent ? damage * Monster_get_hp(neighbour) : damage;
            _apply_damage(
                value,
                node,
                monsters
            );
        }
    }
}

/*********************************************************/
/*                       PyroEffect                      */
/*********************************************************/

static void _Pyro_update(Effect* self, DequeNode* monster, Deque* monsters) {
    _apply_neighbours_damage(Deque_get_elem(monster), monsters, 2, self->base_damage * 0.15, false);
    Effect_reset(self, Deque_get_elem(monster));
}

EffectUnion Effect_new_Pyro(double base_damage, double base_speed) {
    return (EffectUnion) {
        .pe = {
            .base = {
                .update                         = _Pyro_update,
                .base_damage                    = base_damage,
                .base_speed                     = base_speed,
                .type                           = EFFECT_PYRO,
            }
        }
    };
}

/*********************************************************/
/*                      DendroEffect                     */
/*********************************************************/

static bool _Dendro_apply_periodic_damage(Effect* self, DequeNode* monster, Deque* monsters) {
    const double damage = self->base_damage * 0.025;
    DendroEffect* dendro = (DendroEffect*) self;

    if (Timer_is_over(&dendro->next_damage)) {
        dendro->next_damage = Timer_new_ms(500);
        _apply_damage(damage, monster, monsters);
    }

    return false;
}

static void _Dendro_update(Effect* self, DequeNode* monster, Deque* monsters) {
    DendroEffect* dendro = (DendroEffect*) self;
    if (Timer_is_over(&dendro->end)) {
        Effect_reset(self, Deque_get_elem(monster));
        return;
    }
    _Dendro_apply_periodic_damage(self, monster, monsters);
}

EffectUnion Effect_new_Dendro(double base_damage, double base_speed) {
    return (EffectUnion) {
        .de = {
            .base = {
                .update                         = _Dendro_update,
                .base_damage                    = base_damage,
                .base_speed                     = base_speed,
                .type                           = EFFECT_DENDRO,
            },
            .next_damage = Timer_new_ms(500),
            .end = Timer_new_ms(10000),
        }
    };
}

/*********************************************************/
/*                       HydroEffect                     */
/*********************************************************/

static void _Hydro_apply_speed(Effect* self, Monster* monster) {
    HydroEffect* hydro = (HydroEffect*) self;
    const double speed = self->base_speed * 0.5;

    if (Timer_is_over(&hydro->end)) {
        Monster_set_speed(monster, self->base_speed);
        return;
    }

    Monster_set_speed(monster, speed);
}

static void _Hydro_update(Effect* self, DequeNode* monster, Deque* monsters) {
    HydroEffect* hydro = (HydroEffect*) self;
    _Hydro_apply_speed(self, Deque_get_elem(monster));

    if (Timer_is_over(&hydro->end)) {
        Effect_reset(self, Deque_get_elem(monster));
    }
}

EffectUnion Effect_new_Hydro(double base_damage, double base_speed) {
    return (EffectUnion) {
        .he = {
            .base = {
                .update                         = _Hydro_update,
                .base_damage                    = base_damage,
                .base_speed                     = base_speed,
                .type                           = EFFECT_HYDRO,
            },
            .end = Timer_new_ms(10000),
        }
    };
}

/*********************************************************/
/*                     PyroHydroEffect                   */
/*********************************************************/

static void _PyroHydro_update(Effect* self, DequeNode* monster, Deque* monsters) {
    PyroHydroEffect* pyro_hydro = (PyroHydroEffect*) self;
    _apply_neighbours_damage(Deque_get_elem(monster), monsters, 3.5, 0.05, true);

    if (Timer_is_over(&pyro_hydro->end)) {
        Effect_reset(self, Deque_get_elem(monster));
    }
}

EffectUnion Effect_new_PyroHydro(double base_damage, double base_speed) {
    return (EffectUnion) {
        .phe = {
            .base = {
                .update                         = _PyroHydro_update,
                .base_damage                    = base_damage,
                .base_speed                     = base_speed,
                .type                           = EFFECT_PYRO_HYDRO,
            },
            .end = Timer_new_ms(5000),
        }
    };
}

/*********************************************************/
/*                     PyroDendroEffect                  */
/*********************************************************/

static void _PyroDendro_apply_damage(Effect* self, DequeNode* monster, Deque* monsters) {
    const double damage = self->base_damage * (3 - 1);
    Monster_apply_damage(Deque_get_elem(monster), damage);
}

static void _PyroDendro_update(Effect* self, DequeNode* monster, Deque* monsters) {
    _PyroDendro_apply_damage(self, Deque_get_elem(monster), monsters);
    Effect_reset(self, Deque_get_elem(monster));
}

EffectUnion Effect_new_PyroDendro(double base_damage, double base_speed) {
    return (EffectUnion) {
        .pde = {
            .base = {
                .update                         = _PyroDendro_update,
                .base_damage                    = base_damage,
                .base_speed                     = base_speed,
                .type                           = EFFECT_PYRO_DENDRO,
            }
        }
    };
}

/*********************************************************/
/*                     DendroHydroEffect                 */
/*********************************************************/

static void _DendroHydro_apply_speed(Effect* self, Monster* monster) {
    const double speed = 0;
    Monster_set_speed(monster, speed);
}

static void _DendroHydro_update(Effect* self, DequeNode* monster, Deque* monsters) {
    DendroHydroEffect* dendro_hydro = (DendroHydroEffect*) self;
    _DendroHydro_apply_speed(self, Deque_get_elem(monster));

    if (Timer_is_over(&dendro_hydro->end)) {
        Effect_reset(self, Deque_get_elem(monster));
    }
}

EffectUnion Effect_new_DendroHydro(double base_damage, double base_speed) {
    return (EffectUnion) {
        .dhe = {
            .base = {
                .update                         = _DendroHydro_update,
                .base_damage                    = base_damage,
                .base_speed                     = base_speed,
                .type                           = EFFECT_DENDRO_HYDRO,
            },
            .end = Timer_new_ms(3000),
        }
    };
}

/*********************************************************/
/*                     EffectFactory                     */
/*********************************************************/

void Effect_new(EffectUnion* effect_u, double base_damage, double base_speed, GemType old_shot, GemType new_shot) {
    if (old_shot == GEM_NONE && new_shot == GEM_PYRO) {
        *effect_u = Effect_new_Pyro(base_damage, base_speed);
    }

    else if (old_shot == GEM_NONE && new_shot == GEM_DENDRO) {
        *effect_u = Effect_new_Dendro(base_damage, base_speed);
    }

    else if (old_shot == GEM_NONE && new_shot == GEM_HYDRO) {
        *effect_u = Effect_new_Hydro(base_damage, base_speed);
    }

    else if (old_shot == GEM_PYRO && new_shot == GEM_HYDRO) {
        *effect_u = Effect_new_PyroHydro(base_damage, base_speed);
    }

    else if (old_shot == GEM_PYRO && new_shot == GEM_DENDRO) {
        *effect_u = Effect_new_PyroDendro(base_damage, base_speed);
    }

    else if (old_shot == GEM_DENDRO && new_shot == GEM_HYDRO) {
        *effect_u = Effect_new_DendroHydro(base_damage, base_speed);
    }
}
