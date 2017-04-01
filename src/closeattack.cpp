#include "closeattack.h"
#include <string>
#include "attack.h"
#include "weapon.h"
#include "map.h"

CloseAttack::CloseAttack()
    : Attack() {}

CloseAttack::CloseAttack(const CloseAttack& obj)
    : Attack(obj) {}

CloseAttack::CloseAttack(Weapon* parent, std::string line)
    : Attack(parent, line) {}

Item::ptr CloseAttack::spawn_projectile() {
    Item::ptr item = parent->clone();
    return item;
}

void CloseAttack::make_attack(Actor& user
                              , Coord start
                              , Coord end
                              , Actor* victim
                              , Bodypart* target
                              ) {
    Item::ptr projectile = spawn_projectile();
    // starting momentum of projectile
    int32_t momentum = get_momentum(projectile->get_weight(), start_force);

    Tile::ptr tile = Map::curr_map->at(end);
    if (tile->get_actor().get() == victim)
        victim->get_wound(momentum, *projectile, contact_area, target);
    else if (tile->get_actor())
        tile->get_actor()->get_wound(momentum, *projectile, contact_area, nullptr);
}
