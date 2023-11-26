#include "tower.h"
#include "geom.h"

Point Tower_get_pos(const Tower* self) {
    return self->pos;
}

Point Tower_get_abs_pos(const Tower* self) {
    return Grid_get_absolute_coords_C(self->grid, Tower_get_pos(self));
}

void Tower_new(Tower* self, const Grid* grid, Point pos) {
    *self = (Tower) {
        .grid = grid,
        .active_timer = Timer_new_ms(2000),
        .shoot_timer = Timer_new_ms(500),
        .is_empty = false,
        .pos = pos,
        .is_active = false,
        .range = 3 * grid->cell_width,
    };
}

void Tower_set_gem(Tower* self, Gem gem) {
    self->gem = gem;
    self->is_empty = false;
    self->active_timer = Timer_new_ms(2000);
}

/**
 * @brief Get a monster in the tower's range, having the highest HP.
 * 
 * @param tower Tower object
 * @param monsters Deque of monsters
 * @param range Tower's range
 * @return Monster satisfying the condition or NULL if none
 */
static Monster* _Tower_get_monster_target(
    const Tower* tower, const Deque* monsters
) {
    DequeNode* entry;
    int highest_hp = 0;
    Monster* highest_hp_monster = NULL;

    DEQUE_FOREACH(entry, monsters) {
        Monster* monster = Deque_get_elem(entry);
        if (Vector2D_dist(Tower_get_abs_pos(tower), monster->traj.pos) <= tower->range) {
            if (monster->current_hp > highest_hp) {
                highest_hp = monster->current_hp;
                highest_hp_monster = monster;
            }
        }
    }

    return highest_hp_monster;
}

/**
 * @brief Attack the monsters with the towers.
 * Each tower shoot a monster in its range, having the lowest HP.
 * 
 * @param self Tower object
 * @param monsters Monsters to attack
 * @return Error
 * INFO_NO_MONSTER_IN_RANGE if no monster is in range
 * INFO_MONSTER_TARGETED if a monster has been targeted
 */
static Error _Tower_attack(Tower* self, Deque* monsters) {
    Monster* target = _Tower_get_monster_target(self, monsters);
    if (!target) {
        return INFO_NO_MONSTER_IN_RANGE;
    }
    Shot shot;
    Shot_new(&shot, self->grid, Tower_get_abs_pos(self), &target->traj.pos, self->gem);
    Monster_add_future_shot(target, &shot);

    return INFO_MONSTER_TARGETED;
}

/**
 * @brief Update the tower's state, and attack the monsters.
 * Do nothing if the tower is empty, or the shoot timer isn't completed.
 * 
 * @param self Tower object
 * @param monsters Monsters to attack
 * @return Error 0
 */
Error Tower_anim(Tower* self, Deque* monsters) {
    if (self->is_empty) {
        return 0;
    }

    if (Timer_is_over(&self->active_timer)) {
        self->is_active = true;
    } else {
        return 0;
    }

    if (Timer_is_over(&self->shoot_timer)) {
        if (_Tower_attack(self, monsters) == INFO_MONSTER_TARGETED) {
            self->shoot_timer = Timer_new_ms(500);
        }
    }

    return 0;
}

void Tower_draw(const Tower* self) {
    const double margin = 5;
    Point TL = Vector2D_add(
        Grid_get_absolute_coords_TL(self->grid, self->pos),
        Vector2D_new_from_value(margin)
    );
    Point BR = Vector2D_sub(
        Grid_get_absolute_coords_BR(self->grid, self->pos),
        Vector2D_new_from_value(margin)
    );
    MLV_draw_filled_rectangle(
        TL.x, TL.y,
        BR.x - TL.x, BR.y - TL.y,
        MLV_COLOR_BLUE
    );

    // Draw a circle around the tower showing its range
    Point pos = Tower_get_abs_pos(self);
    MLV_draw_circle(
        pos.x, pos.y,
        self->range,
        MLV_rgba(255,0,0,100)
    );
}
